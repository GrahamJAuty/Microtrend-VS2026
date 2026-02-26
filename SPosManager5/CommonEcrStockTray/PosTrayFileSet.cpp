/**********************************************************************/
 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayFileSet.h"
/**********************************************************************/

void CPosTrayFileSet::GetPosTrayFileSet( int nType )
{
	m_pArrayTaskInfo = NULL;
	m_pArrayBatchInfo = NULL;
		
	switch( nType )
	{
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_HIST_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskHist;
		m_infoTaskTemp = SysFiles::PosTrayTaskHistTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchHist;
		m_infoBatchTemp = SysFiles::PosTrayBatchHistTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_HIST_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_HIST_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_HIST_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskExportSales;
		m_infoTaskTemp = SysFiles::PosTrayTaskExportSalesTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchExportSales;
		m_infoBatchTemp = SysFiles::PosTrayBatchExportSalesTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_EXPORT_SALES_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_EXPORT_SALES_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_EXPORT_SALES_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskExportPayment;
		m_infoTaskTemp = SysFiles::PosTrayTaskExportPaymentTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchExportPayment;
		m_infoBatchTemp = SysFiles::PosTrayBatchExportPaymentTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskExportVoid;
		m_infoTaskTemp = SysFiles::PosTrayTaskExportVoidTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchExportVoid;
		m_infoBatchTemp = SysFiles::PosTrayBatchExportVoidTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_EXPORT_VOID_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_EXPORT_VOID_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_EXPORT_VOID_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskExportDiscount;
		m_infoTaskTemp = SysFiles::PosTrayTaskExportDiscountTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchExportDiscount;
		m_infoBatchTemp = SysFiles::PosTrayBatchExportDiscountTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL;
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:

		if ( SysInfo.IsStockSystem() == TRUE )
		{
			m_infoTaskFile = SysFiles::PosTrayTaskStkLevelStk;
			m_infoTaskTemp = SysFiles::PosTrayTaskStkLevelStkTemp;
			m_infoBatchFile = SysFiles::PosTrayBatchStkLevelStk;
			m_infoBatchTemp = SysFiles::PosTrayBatchStkLevelStkTemp;
		}
		else
		{
			m_infoTaskFile = SysFiles::PosTrayTaskStkLevelEcr;
			m_infoTaskTemp = SysFiles::PosTrayTaskStkLevelEcrTemp;
			m_infoBatchFile = SysFiles::PosTrayBatchStkLevelEcr;
			m_infoBatchTemp = SysFiles::PosTrayBatchStkLevelEcrTemp;
		}

		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_STKLEVEL_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_STKLEVEL_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_STKLEVEL_ALL;
		break;

	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskProcessTran;
		m_infoTaskTemp = SysFiles::PosTrayTaskProcessTranTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchProcessTran;
		m_infoBatchTemp = SysFiles::PosTrayBatchProcessTranTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_PROCESSTRAN_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_PROCESSTRAN_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_PROCESSTRAN_ALL;
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
		m_infoTaskFile = SysFiles::PosTrayTaskTimeAttend;
		m_infoTaskTemp = SysFiles::PosTrayTaskTimeAttendTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchTimeAttend;
		m_infoBatchTemp = SysFiles::PosTrayBatchTimeAttendTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_TIMEATTEND_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_TIMEATTEND_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_TIMEATTEND_ALL;
		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_EPOS_ALL:
	default:
		m_infoTaskFile = SysFiles::PosTrayTaskEpos;
		m_infoTaskTemp = SysFiles::PosTrayTaskEposTemp;
		m_infoBatchFile = SysFiles::PosTrayBatchEpos;
		m_infoBatchTemp = SysFiles::PosTrayBatchEposTemp;
		m_infoDateFile = SysFiles::PosTrayLiveTaskDate;
		m_infoDateTemp = SysFiles::PosTrayLiveTaskDateTemp;
		m_nTaskFileType = POSTRAY_TASKFILE_EPOS_TASK;
		m_nBatchFileType = POSTRAY_TASKFILE_EPOS_BATCH;
		m_nAllFileType = POSTRAY_TASKFILE_EPOS_ALL;
		break;
	}
}

/**********************************************************************/

bool CPosTrayFileSet::IsExportTask()
{
	switch( m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/
