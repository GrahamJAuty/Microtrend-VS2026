#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "EposReportBase.h"
#include "EposReportConsolPaymentByItem.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CEposReportPaymentByItem : public CEposReport
{
public:
	CEposReportPaymentByItem(CEposSelectArray& SelectArray, int nReportType);
	~CEposReportPaymentByItem();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void ProcessPMSFile(CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize);
	void ProcessEPOSFile(CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize);

private:
	void CreateSalesSection(CString& strHeader, CEposReportPaymentByItemBlock& Block);
	void CreatePaymentSection(CString& strHeader, CReportConsolidationArray<CEposReportConsolPBI_MainBlock>* pPayments, int nBlockStart, int nBlockEnd, bool bCharge);

private:
	void ProcessTransaction();
	void ProcessTransactionException(int nType);
	void ResetTransaction();

private:
	void ConsolidatePluNo(__int64 nPluNo, int nDeptNo, int nGroupNo, double dVal);
	void ConsolidatePayment(CPaymentByItemBufferPayment& infoPayment, bool bZeroItem, __int64 nItem, double dVal);
	void ConsolidateException(CPaymentByItemBufferPayment& infoPayment, int nExceptionType, double dVal);

private:
	void InitialiseConsolidationBlocks();

private:
	CReportConsolidationArray<CPaymentByItemBufferPayment> m_ConsolBufferPayment;
	CReportConsolidationArray<CPaymentByItemBufferSale> m_ConsolBufferSale;
	CReportConsolidationArray<CSortedInt64Item> m_arrayTransactionItemsNormal;
	CReportConsolidationArray<CSortedInt64Item> m_arrayTransactionItemsCharge;
	
private:
	int m_nTransactionRAType;
	bool m_bTransactionCashbackFlag;

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPaymentByItemBlock, CEposReportPaymentByItemBlock> m_arrayReportBlocks;
	CArray<int, int> m_arrayTerminalBlocks;
	CReportPluInfoFinder m_PluInfoFinder;
	
private:
	bool m_bGotPayMethGoodLine;
	bool m_bGotPayMethFilteredItem;
	bool m_bGotNormalBlockGoodLine;
	bool m_bGotNormalBlockFilteredItem;
	bool m_bGotChargeBlockGoodLine;
	bool m_bGotChargeBlockFilteredItem;
};


/**********************************************************************/
