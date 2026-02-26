#pragma once
/**********************************************************************/
#include "PosTrayTaskInfo.h"
/**********************************************************************/

struct CPosTrayTaskArrays
{
public:
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskProcessTran;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchProcessTran;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskReportEpos;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchReportEpos;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskReportHist;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchReportHist;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskReportStkLevel;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchReportStkLevel;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskReportTimeAttend;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchReportTimeAttend;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskExportSales;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchExportSales;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskExportPayment;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchExportPayment;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskExportVoid;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchExportVoid;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_taskExportDiscount;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_batchExportDiscount;
};

/**********************************************************************/