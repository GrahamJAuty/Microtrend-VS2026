#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "CashRSPVersionChecker.h"
#include "PosTrayTaskInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CLIPAExportBufferSale
{
public:
	CLIPAExportBufferSale();

public:
	void Reset();
	int Compare ( CLIPAExportBufferSale& source, int nHint = 0 );
	void Add	( CLIPAExportBufferSale& source );
	
public:
	int m_nLocIdx;
	int m_nPriceLevel;
	int m_nPaymentType;
	__int64 m_nPluNo;
	int m_nGLCode;
	CString m_strCostCentre;
	CString m_strProject;
	bool m_bIsType5Line;
	CString m_strTaxBand;
	double m_dTaxRate;
	int m_nLineType;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CLIPAExportBufferPayment
{
public:
	CLIPAExportBufferPayment();

public:
	void Reset();
	int Compare ( CLIPAExportBufferPayment& source, int nHint = 0 );
	void Add	( CLIPAExportBufferPayment& source );
	
public:
	int m_nPaymentType;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CLIPAExportBufferPluItem
{
public:
	CLIPAExportBufferPluItem();

public:
	void Reset();
	int Compare ( CLIPAExportBufferPluItem& source, int nHint = 0 );
	void Add	( CLIPAExportBufferPluItem& source );
	
public:
	__int64 m_nPluNo;
	int m_nGLCode;
	CString m_strCostCentre;
	CString m_strProject;
};

/**********************************************************************/

class CPosTrayLIPAExportCreator
{
public:
	CPosTrayLIPAExportCreator( CPosTrayLIPAScheduleInfo& infoExport );

public:
	bool ProcessExport( COleDateTime& dateReport );

private:
	bool ProcessEPOSFile( CTermFileListInfo& infoFile );
	void ConsolidatePluNo( __int64 nPluNo, int nPriceLevel, int nTaxBand, double dVal );
	void ResetTransaction();
	void ProcessTransaction( int nLocIdx );
	void LogError( const char* szError );

private:
	CReportConsolidationArray<CLIPAExportBufferPluItem> m_PluInfoTable;
	CReportConsolidationArray<CLIPAExportBufferPayment> m_TransactionBufferPayment;
	CReportConsolidationArray<CLIPAExportBufferSale> m_TransactionBufferSale;
	CReportConsolidationArray<CLIPAExportBufferSale> m_ConsolArray;
	CStringArray m_bufferDiscounts;
	CStringArray m_arrayErrors;

private:
	int m_nTransactionRAType;

private:
	CCashRSPVersionChecker m_CashRSPVersionChecker;
	CPosTrayLIPAScheduleInfo& m_infoExport;
	CString m_strTranHeader;
	bool m_bGotTranError;
	int m_nTaxDateCode;
	int m_nTransactionAccountID;
};

/**********************************************************************/
#endif
/**********************************************************************/


