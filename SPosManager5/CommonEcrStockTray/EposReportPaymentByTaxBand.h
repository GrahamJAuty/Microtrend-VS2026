#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "EposReportBase.h"
#include "EposReportConsolPaymentByTaxBand.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CEposReportPaymentByTaxBand : public CEposReport
{
public:
	CEposReportPaymentByTaxBand( CEposSelectArray& SelectArray );
	~CEposReportPaymentByTaxBand();
	
	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void ProcessPMSFile	( CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize );
	void ProcessEPOSFile( CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize );

private:
	void CreateSalesSection( CString& strHeader, CEposReportPaymentByTaxBandBlock& Block );
	void CreatePaymentSection( CString& strHeader, CReportConsolidationArray<CEposReportConsolPBT_MainBlock>* pPayments, int nBlockStart, int nBlockEnd );
	void CreateTotalsSection( CString& strHeader, CReportConsolidationArray<CEposReportConsolPBT_TotalBlock>* pTotals );
	
private:
	void ProcessTransaction();
	void ProcessTransactionException( int nType );
	void ResetTransaction();

private:
	void ConsolidatePluNo( __int64 nPluNo, int nDeptNo, int nGroupNo, int nTaxDateCode, int nTaxBand, double dVal );
	void ConsolidatePaymentTax( int nPaymentNo, int nTaxBand, bool bPushDown, double dPayment, double dTax );
	
private:
	void InitialiseConsolidationBlocks();
	
private:
	CReportConsolidationArray<CPaymentByTaxBandBufferPayment> m_ConsolBufferPayment;
	CReportConsolidationArray<CPaymentByTaxBandBufferSale> m_ConsolBufferSale;
	
private:
	int m_nTransactionRAType;
	bool m_bTransactionCashbackFlag;
	
private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPaymentByTaxBandBlock,CEposReportPaymentByTaxBandBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
