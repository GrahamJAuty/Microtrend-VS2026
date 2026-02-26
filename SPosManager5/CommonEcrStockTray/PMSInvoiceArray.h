#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
#include "PMSBookingCSVArray.h"
#include "PMSDefines.h"
#include "PMSInvoiceLineInfo.h"
#include "PMSRoomStatusInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CConsolPMSInvoiceGroup
{
public:
	CConsolPMSInvoiceGroup();

public:
	void Reset();
	int Compare ( CConsolPMSInvoiceGroup& source, int nHint = 0 );
	void Add	( CConsolPMSInvoiceGroup& source );
	
public:
	int m_nGroupNo;

public:
	double m_dQty;
	double m_dVal;
};

/**********************************************************************/

class CPMSInvoiceArray 
{
public:
	CPMSInvoiceArray();
	
public:
	void Reset();
	void CopyInvoiceItems( CPMSInvoiceArray& source );

public:
	int GetInvoiceItemCount();
	void GetInvoiceItem( int n, CPMSInvoiceLineInfo& info );
	void SetInvoiceItem( int n, CPMSInvoiceLineInfo& info );
	void RemoveInvoiceItem( int n );

	int GetPaymentTotalCount();
	void GetPaymentTotal( int n, CPMSInvoiceLineInfo& info );
	void RemovePaymentTotal( int n );

	int GetAllowanceTotalCount();
	void GetAllowanceTotal( int n, CPMSInvoiceLineInfo& info );
	void RemoveAllowanceTotal( int n );

public:
	int AddInvoiceItem( CPMSInvoiceLineInfo& item );
	bool FindInvoiceItem( CPMSInvoiceLineInfo& item, int& nPos );

	int AddPaymentTotal( CPMSInvoiceLineInfo& item );
	bool FindPaymentTotal( CPMSInvoiceLineInfo& item, int& nPos );

	int AddAllowanceTotal( CPMSInvoiceLineInfo& item );
	bool FindAllowanceTotal( CPMSInvoiceLineInfo& item, int& nPos );

public:
	void BuildInvoiceArray( int nBookingId, int nAccountNo, int nInvoiceType );
	void BuildInvoiceArray( CPMSBookingCSVRecord& BookingRecord, int nAccountNo, int nInvoiceType, bool bDBBCredit = FALSE );
	void GetInvoiceChanges( CPMSInvoiceArray& arrayInput1, CPMSInvoiceArray& arrayInput2 );
	void WriteInvoice( int nBookingId, int nAccountNo, bool bStock = FALSE );
	
	void PrepareDBBCreditsItem( CPMSBookingCSVRecord& BookingRecord, int nDinnerTaxBand, double& dItemSpend, double& dPostCheckInPaid, double& dAllowance, CReportConsolidationArray<CConsolidatedDoubleByString>& arraySales, bool bAddCredits );
	void PrepareDBBCreditsGroup( CPMSBookingCSVRecord& BookingRecord, int nDinnerTaxBand );

	void CreateEPOSReportFile( int nRoomGridRoomIdx, int nRoomGridAccountIdx, bool bForReport = FALSE );
	void DeleteEPOSReportFile( int nRoomGridRoomIdx, int nRoomGridAccountIdx );
	void CreateEPOSReportData( int nRoomGridRoomIdx, int nRoomGridAccountIdx, CStringArray& arrayReportTran, CStringArray& arrayReportSale, CStringArray& arrayReportTerm, int& nDepoMinDay, int& nDepoMaxDay, int& nTranMinDay, int& nTranMaxDay );
	void ProcessEPOSDiscounts( CStringArray& arrayReportSale, CReportConsolidationArray<CCashRSPImporterDiscountHelperSales>& arraySales, CReportConsolidationArray<CCashRSPImporterDiscountHelperPendingItem>& arrayDiscounts, double dSales, double dDiscount, int nPMSDayNumber );

public:
	double GetInvoiceTaxContent(){ return m_dInvoiceTaxContent; }
	static void RebuildTaxBands(int nBookingId, int nAccountNo);

private:
	void PurgeBlankItemLines();
	void PurgeBlankPaymentLines();
	void PurgeBlankAllowanceLines();
	void PurgeEmptyTransactions();
	void BuildInvoiceArrayItem( CSSFile& fileTrans, int nInvoiceType, int BookingId );
	void FinaliseInvoiceArrayGroup();
	void FinaliseInvoiceArrayTransaction();

private:
	bool ProcessTransactionLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale );
	bool ProcessItemLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale );
	bool ProcessItemDiscountLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale );
	bool ProcessSubtotalDiscountLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale, int nLineType );
	void ProcessGroupTransaction( CPMSInvoiceArray& arrayInvoice, CPMSInvoiceLineInfo& infoTransaction, CReportConsolidationArray<CConsolPMSInvoiceGroup>& arraySales );
	
	bool ProcessTransactionEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale );
	bool ProcessItemEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale, int nLineType );
	bool ProcessItemDiscountEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale );
	bool ProcessSubtotalDiscountEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale, int nLineType );
	bool ProcessPaymentEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale );

	void ProcessInvoiceTax();
	void ProcessTransactionTax( CPMSInvoiceLineInfo& infoTransaction );

private:
	bool GetPMSSalesLine( CPMSInvoiceLineInfo& infoSale, CStringArray& arraySales, int nCovers );
	bool GetPMSDiscountLine( CPMSInvoiceLineInfo& infoSale, CString& strLine );
	bool GetPMSPaymentLine( CPMSInvoiceLineInfo& infoSale, CString& strLine );

private:
	bool GetCASHRSPSalesLine( CPMSInvoiceLineInfo& infoSale, CStringArray& arraySales, int nCovers );
	bool GetCASHRSPDiscountLine( CPMSInvoiceLineInfo& infoSale, CString& strLine );
	bool GetCASHRSPPaymentLine( CPMSInvoiceLineInfo& infoSale, CString& strLine );

private:
	bool GetTermSalesLine( CPMSInvoiceLineInfo& infoSale, CStringArray& arraySales, bool bPMS, int nCovers );
	bool GetTermDiscountLine( CPMSInvoiceLineInfo& infoSale, CString& strLine, bool bPMS );
	bool GetTermPaymentLine( CPMSInvoiceLineInfo& infoSale, CString& strLine, bool bPMS );

private:
	double m_dInvoiceTaxContent;
	double m_dInvoiceDiscountBuffer;
	double m_dInvoiceTaxableBuffer[MAX_TAX_BANDS + 1];

private:
	CArray<CPMSInvoiceLineInfo,CPMSInvoiceLineInfo> m_arrayInvoiceItems;
	CArray<CPMSInvoiceLineInfo,CPMSInvoiceLineInfo> m_arrayPaymentTotals;
	CArray<CPMSInvoiceLineInfo,CPMSInvoiceLineInfo> m_arrayAllowanceTotals;
};

/**********************************************************************/
#endif
/**********************************************************************/
