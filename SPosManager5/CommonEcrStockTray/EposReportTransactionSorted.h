#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolTransactionSorted.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/
#define TRANBUFFER_LINETYPE_NORMAL 0
#define TRANBUFFER_LINETYPE_NOSEARCH 1
#define TRANBUFFER_LINETYPE_HIDDEN 2
#define TRANBUFFER_LINETYPE_BLANKS 3
#define TRANBUFFER_LINETYPE_FILTERED 4
/**********************************************************************/

struct CEposReportTransactionBufferItem
{
public:
	CEposReportTransactionBufferItem();

public:
	int m_nLineType;
	int m_nServerNo;
	int m_nBlankLineCount;
	CString m_strText;
};

/**********************************************************************/

class CEposReportTransactionBuffer
{
public:
	CEposReportTransactionBuffer();
	void Reset();
	
public:
	int GetSize(){ return m_array.GetSize(); }
	void GetAt( int n, CEposReportTransactionBufferItem& item );
	void SetAt( int n, CEposReportTransactionBufferItem& item );

public:
	double GetUnderpaid(){ return m_dUnderpaid; }
	double GetOverpaid(){ return m_dOverpaid; }
	
public:
	void AddBlanks( int n );
	void AddReportLine( const char* sz, int nServerNo );
	void AddNoSearchLine( const char* sz );
	void AddHiddenLine( const char* sz );
	void SetUnderpaid( double d){ m_dUnderpaid = d; }
	void SetOverpaid( double d){ m_dOverpaid = d; }
	
private:
	CArray<CEposReportTransactionBufferItem,CEposReportTransactionBufferItem> m_array;
	double m_dUnderpaid;
	double m_dOverpaid;
};

/**********************************************************************/

struct CEposReportTransactionSortedTranInfo
{
public:
	CEposReportTransactionSortedTranInfo();
	void Reset();

public:
	bool m_bGotFirstSaleTime;
	COleDateTime m_oleFirstSaleTime;
};

/**********************************************************************/

class CEposReportTransactionSorted : public CEposReport
{
public:
	CEposReportTransactionSorted( CEposSelectArray& SelectArray, int nReportType );
	CEposReportTransactionSorted( CEposSelectArray& SelectArray, int nReportType, const char* szCustomSettings );
	void SetNoReadDatabaseFlag(){ m_bNoReadDatabase = TRUE; }
	
	virtual void Consolidate();
	virtual bool CreateReport();

private:
	bool CreateReportInternal();

private:
	void GetTransactionHeaders( CCSVEposTermLine& csvIn, int nCashRSPLineType );
	void BufferPMSHeader( CCSVEposTermLine& csvI );
	void BufferSale( CCSVEposTermLine& csvIn, bool bWastageTransaction );
	void BufferPayment( CCSVEposTermLine& csvIn );
	void BufferNoSale( CCSVEposTermLine& csvIn );
	void BufferAgeVerify( CCSVEposTermLine& csvIn );
	void BufferPaidIO( CCSVEposTermLine& csvIn );
	void BufferItemDiscount( CCSVEposTermLine& csvIn );
	void BufferSubtotalDiscount( CCSVEposTermLine& csvIn, bool bLoyalty );
	void BufferSubtotalDiscountTax( CCSVEposTermLine& csvIn, bool bLoyalty );
	void BufferVoidDiscount( CCSVEposTermLine& csvIn );
	void BufferDelivery( CCSVEposTermLine& csvIn );
	void BufferFooter( CCSVEposTermLine& csvIn );
	void BufferUnknown( CCSVEposTermLine& csvIn );
	
	void ConsolidateTransaction( double dTransactionTotal );
	void WriteTransactions( int nDbIdx, int nLocIdx );
	const char* GetTransactionReportLine( const char* szTime, const char* szInfo1, const char* szInfo2, int& nVNoToWrite );
	void WriteBufferedTransaction( CEposReportTransactionBuffer& buffer );
	
private:
	void GetServerName( int nDbIdx, int nLocIdx, int nServerNo, CString& strServerName );

private:
	bool m_bPMSTransaction;
	bool m_bWastageTransaction;
	CString m_strTransactionHeader;
	CArray<CEposReportTransactionSortedLine> m_arrayBufferPMSHeader; 
	CArray<CEposReportTransactionSortedLine> m_arrayBufferGeneral;
	CArray<CEposReportTransactionSortedLine> m_arrayBufferPayments;
	CArray<CEposReportTransactionSortedLine> m_arrayBufferDiscounts;
	CArray<CEposReportTransactionSortedLine> m_arrayBufferFooter; 
	bool m_bNoReadDatabase;
	double m_dTransactionTotalNonCashbackSales;
	double m_dTransactionTotalNonCashbackPayments;
	double m_dTotalOverPaid;
	double m_dTotalUnderPaid;
	
private:
	CReportConsolidationArray<CTermFileListInfo> m_arrayFileInfo;
	CEposReportConsolTransactionSorted m_TransactionHeaderIndex;
	CEposReportConsolTransactionSorted m_TransactionHeaderIndexNext;
	CEposReportTransactionSortedLine m_TransactionHeaderLine;
	CReportConsolidationArray<CEposReportConsolTransactionSorted> m_arrayIndex;
	CArray<CEposReportTransactionSortedLine> m_arrayLines;
	CString m_strTransactionReportLine;

private:
	CEposReportCustomSettingsTransaction m_ReportSettings;
	CEposReportTransactionSortedTranInfo m_TransactionInfo;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
