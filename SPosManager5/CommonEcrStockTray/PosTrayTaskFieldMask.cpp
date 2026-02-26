 /**********************************************************************/
#include "PosTrayTask.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayTaskFieldMask.h"
/**********************************************************************/
#define POSTRAY_TASKMODE_ALL 0
#define POSTRAY_TASKMODE_TASK 1
#define POSTRAY_TASKMODE_BATCH 2
/**********************************************************************/

CPosTrayTaskFieldMask::CPosTrayTaskFieldMask()
{
	m_bWantTaskName = FALSE;
	m_bWantBatchNo = FALSE;
	m_bWantEmailNickname = FALSE;
	m_bWantFormatEmail = FALSE;
	m_bWantEmailByReport = FALSE;
	m_bWantFormatExport = FALSE;
	m_bWantExportFolder = FALSE;
	m_bWantExportFilename = FALSE;
	m_bWantExportFilenameFormat = FALSE;
	m_bWantLocNameFlag = FALSE;
	m_bWantFTPFlag = FALSE;
	m_bWantFTPFolder = FALSE;
	m_bWantExportDateType = FALSE;
	m_bWantAutoPrintFlag = FALSE;
	m_bWantLegacyTimeHour = FALSE;
	m_bWantLegacyTimeMinute = FALSE;
	m_bWantTimeHourStart = FALSE;
	m_bWantTimeMinuteStart = FALSE;
	m_bWantTimeHourEnd = FALSE;
	m_bWantTimeMinuteEnd = FALSE;
	m_bWantFrequency = FALSE;
	m_bWantDaySunFlag = FALSE;
	m_bWantDayMonFlag = FALSE;
	m_bWantDayTueFlag = FALSE;
	m_bWantDayWedFlag = FALSE;
	m_bWantDayThuFlag = FALSE;
	m_bWantDayFriFlag = FALSE;
	m_bWantDaySatFlag = FALSE;
	m_bWantMonthDays = FALSE;
	m_bWantBacklogFlag = FALSE;
	/*****/
	m_bWantBatchName = FALSE;
	m_bWantDateType = FALSE;
	m_bWantSessionLegacy = FALSE;
	m_bWantSessionEntity = FALSE;
	m_bWantConType = FALSE;
	m_bWantConDbNo = FALSE;
	m_bWantConNwkNo = FALSE;
	m_bWantConEntityNo = FALSE;
	m_bWantConTermIdx = FALSE;
	m_bWantTermDateType = FALSE;
	m_bWantTableFilterType = FALSE;
	m_bWantTableFilterLevel = FALSE;
	m_bWantTableFilterEntityNo = FALSE;
	m_bWantDeptFilter = FALSE;
	m_bWantPluFilter = FALSE;
	m_bWantSaleTime = FALSE;
	m_bWantPreferConsolType = FALSE;
	m_bWantJobSetConsolType = FALSE;
	m_bWantReportEcr = FALSE;
	m_bWantReportLoc = FALSE;
	m_bWantReportAll = FALSE;
	m_bWantReportCon = FALSE;
	m_bWantItemServer = FALSE;
	m_bWantTrainServer = FALSE;
	m_bWantPMSType = FALSE;
	m_bWantPMSPending = FALSE;
	m_bWantPMSActive = FALSE;
	m_bWantPMSComplete = FALSE;
	m_bWantPMSCancelled = FALSE;
	m_bWantReportList = FALSE;
	m_bWantReportTime = FALSE;
	m_bWantDateRoundTo = FALSE;
	m_bWantTimeSlice = FALSE;
	m_bWantPluRange = FALSE;
	m_bWantPluFrom = FALSE;
	m_bWantPluTo = FALSE;
	m_bWantDeptFilterType = FALSE;
	m_bWantModifierFilter = FALSE;
	m_bWantModifierCustom = FALSE;
	m_bWantTaxBandFilter = FALSE;
	m_bWantTaxBandCustom = FALSE;
	m_bWantPriceLevelFilter = FALSE;
	m_bWantPriceLevelCustom = FALSE;
	m_bWantEposSales = FALSE;
	m_bWantManualSales = FALSE;
	m_bWantImportSales = FALSE;
	m_bWantPMSTerminal = FALSE;
	m_bWantMonthSort = FALSE;
	m_bWantDeptSort = FALSE;
	m_bWantSupplierSort = FALSE;
	m_bWantSupplierFilter = FALSE;
	m_bWantStockZeroSkip = FALSE;
	m_bWantPercentage = FALSE;
	m_bWantEstProfit = FALSE;
	m_bWantCustomerOnly = FALSE;
	m_bWantVIPOnly = FALSE;
	m_bWantChartType = FALSE;
	m_bWantChartBottom = FALSE;
	m_bWantChartValue = FALSE;
	m_bWantChartSize = FALSE;
	m_bWantChartCutOff = FALSE;
	m_bWantTaxDP = FALSE;
	m_bWantOfferFilter = FALSE;
	m_bWantTimeAttend = FALSE;
	m_bWantSalesExportFields = FALSE;
	m_bWantPaymentExportFields = FALSE;
	m_bWantVoidExportFields = FALSE;
	m_bWantDiscountExportFields = FALSE;
	/*****/
	m_bWantMergeDelete = FALSE;
}

/**********************************************************************/

void CPosTrayTaskFieldMask::SetMode( int nMode )
{
	int nTaskMode = POSTRAY_TASKMODE_ALL;
	
	switch( nMode )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	case POSTRAY_TASKFILE_EPOS_ALL:
	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		nTaskMode = POSTRAY_TASKMODE_ALL;
		break;

	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		nTaskMode = POSTRAY_TASKMODE_TASK;
		break;

	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
		nTaskMode = POSTRAY_TASKMODE_BATCH;
		break;
	}

	int nTaskType = POSTRAY_TASKFILE_EPOS_ALL;

	switch( nMode )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
		nTaskType = POSTRAY_TASKFILE_PROCESSTRAN_ALL;
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
		nTaskType = POSTRAY_TASKFILE_TIMEATTEND_ALL;
		break;

	case POSTRAY_TASKFILE_EPOS_ALL:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_EPOS_TASK:
		nTaskType = POSTRAY_TASKFILE_EPOS_ALL;
		break;

	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_HIST_TASK:
		nTaskType = POSTRAY_TASKFILE_HIST_ALL;
		break;

	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		nTaskType = POSTRAY_TASKFILE_STKLEVEL_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		nTaskType = POSTRAY_TASKFILE_EXPORT_SALES_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		nTaskType = POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		nTaskType = POSTRAY_TASKFILE_EXPORT_VOID_ALL;
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		nTaskType = POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL;
		break;
	}

	if ( nTaskMode != POSTRAY_TASKMODE_BATCH )
	{
		switch( nTaskType )
		{
		case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
			m_bWantTaskName = TRUE;
			m_bWantTimeHourStart = TRUE;
			m_bWantTimeMinuteStart = TRUE;
			m_bWantTimeHourEnd = TRUE;
			m_bWantTimeMinuteEnd = TRUE;
			m_bWantFrequency = TRUE;
			m_bWantDaySunFlag = TRUE;
			m_bWantDayMonFlag = TRUE;
			m_bWantDayTueFlag = TRUE;
			m_bWantDayWedFlag = TRUE;
			m_bWantDayThuFlag = TRUE;
			m_bWantDayFriFlag = TRUE;
			m_bWantDaySatFlag = TRUE;
			m_bWantMonthDays = TRUE;
			break;

		case POSTRAY_TASKFILE_EPOS_ALL:
		case POSTRAY_TASKFILE_HIST_ALL:
		case POSTRAY_TASKFILE_STKLEVEL_ALL:
		case POSTRAY_TASKFILE_TIMEATTEND_ALL:
			m_bWantTaskName = TRUE;
			m_bWantBatchNo = TRUE;
			m_bWantEmailNickname = TRUE;
			m_bWantFormatEmail = TRUE;
			m_bWantEmailByReport = TRUE;
			m_bWantFormatExport = TRUE;
			m_bWantExportFolder = TRUE;
			m_bWantLegacyTimeHour = TRUE;
			m_bWantLegacyTimeMinute = TRUE;
			m_bWantTimeHourStart = TRUE;
			m_bWantTimeMinuteStart = TRUE;
			m_bWantTimeHourEnd = TRUE;
			m_bWantTimeMinuteEnd = TRUE;
			m_bWantFrequency = TRUE;
			m_bWantDaySunFlag = TRUE;
			m_bWantDayMonFlag = TRUE;
			m_bWantDayTueFlag = TRUE;
			m_bWantDayWedFlag = TRUE;
			m_bWantDayThuFlag = TRUE;
			m_bWantDayFriFlag = TRUE;
			m_bWantDaySatFlag = TRUE;
			m_bWantMonthDays = TRUE;
			m_bWantExportFilename = TRUE;
			m_bWantExportFilenameFormat = TRUE;
			m_bWantAutoPrintFlag = TRUE;
			m_bWantBacklogFlag = TRUE;
			break;

		case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
			m_bWantTaskName = TRUE;
			m_bWantBatchNo = TRUE;
			m_bWantEmailNickname = TRUE;
			m_bWantFormatEmail = TRUE;
			m_bWantFormatExport = TRUE;
			m_bWantExportFolder = TRUE;
			m_bWantFTPFlag = TRUE;
			m_bWantFTPFolder = TRUE;
			m_bWantExportDateType = TRUE;
			m_bWantLegacyTimeHour = TRUE;
			m_bWantLegacyTimeMinute = TRUE;
			m_bWantTimeHourStart = TRUE;
			m_bWantTimeMinuteStart = TRUE;
			m_bWantTimeHourEnd = TRUE;
			m_bWantTimeMinuteEnd = TRUE;
			m_bWantFrequency = TRUE;
			m_bWantDaySunFlag = TRUE;
			m_bWantDayMonFlag = TRUE;
			m_bWantDayTueFlag = TRUE;
			m_bWantDayWedFlag = TRUE;
			m_bWantDayThuFlag = TRUE;
			m_bWantDayFriFlag = TRUE;
			m_bWantDaySatFlag = TRUE;
			m_bWantMonthDays = TRUE;
			m_bWantExportFilename = TRUE;
			m_bWantExportFilenameFormat = TRUE;
			break;
		}

		switch (nTaskType)
		{
		case POSTRAY_TASKFILE_EPOS_ALL:
			m_bWantLocNameFlag = TRUE;
			break;
		}
	}

	if ( nTaskMode != POSTRAY_TASKMODE_TASK )
	{
		switch( nTaskType )
		{
		case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
			break;

		case POSTRAY_TASKFILE_EPOS_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantConTermIdx = TRUE;
			m_bWantTermDateType = TRUE;
			m_bWantDeptFilter = TRUE;
			m_bWantPluFilter = TRUE;
			m_bWantSaleTime = TRUE;
			m_bWantPreferConsolType = TRUE;
			m_bWantJobSetConsolType = TRUE;
			m_bWantReportEcr = TRUE;
			m_bWantReportLoc = TRUE;
			m_bWantReportAll = TRUE;
			m_bWantReportCon = TRUE;
			m_bWantItemServer = TRUE;
			m_bWantTrainServer = TRUE;
			m_bWantPMSType = TRUE;
			m_bWantPMSPending = TRUE;
			m_bWantPMSActive = TRUE;
			m_bWantPMSComplete = TRUE;
			m_bWantPMSCancelled = TRUE;
			m_bWantReportList = TRUE;
			m_bWantReportTime = TRUE;
			break;

		case POSTRAY_TASKFILE_HIST_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantConTermIdx = TRUE;
			m_bWantTermDateType = TRUE;
			m_bWantTableFilterType = TRUE;
			m_bWantTableFilterLevel = TRUE;
			m_bWantTableFilterEntityNo = TRUE;
			m_bWantDeptFilter = TRUE;
			m_bWantPluFilter = TRUE;
			m_bWantSaleTime = TRUE;
			m_bWantDateRoundTo = TRUE;
			m_bWantReportList = TRUE;
			m_bWantTimeSlice = TRUE;
			m_bWantPluRange = TRUE;
			m_bWantPluFrom = TRUE;
			m_bWantPluTo = TRUE;
			m_bWantDeptFilterType = TRUE;
			m_bWantModifierFilter = TRUE;
			m_bWantModifierCustom = TRUE;
			m_bWantTaxBandFilter = TRUE;
			m_bWantTaxBandCustom = TRUE;
			m_bWantPriceLevelFilter = TRUE;
			m_bWantPriceLevelCustom = TRUE;
			m_bWantEposSales = TRUE;
			m_bWantImportSales = TRUE;
			m_bWantManualSales = TRUE;
			m_bWantPMSTerminal = TRUE;
			m_bWantMonthSort = TRUE;
			m_bWantDeptSort = TRUE;
			m_bWantPercentage = TRUE;
			m_bWantEstProfit = TRUE;
			m_bWantCustomerOnly = TRUE;
			m_bWantVIPOnly = TRUE;
			m_bWantChartType = TRUE;
			m_bWantChartBottom = TRUE;
			m_bWantChartValue = TRUE;
			m_bWantChartSize = TRUE;
			m_bWantChartCutOff = TRUE;
			break;

		case POSTRAY_TASKFILE_TIMEATTEND_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantDateRoundTo = TRUE;
			m_bWantReportList = TRUE;
			m_bWantTimeAttend = TRUE;
			break;

		case POSTRAY_TASKFILE_STKLEVEL_ALL:
			if ( SysInfo.IsStockSystem() == TRUE )
			{
				m_bWantBatchName = TRUE;
			}
			else
			{
				m_bWantDateType = TRUE;
				m_bWantConType = TRUE;
				m_bWantConDbNo = TRUE;
				m_bWantConEntityNo = TRUE;
				m_bWantReportList = TRUE;
				m_bWantDeptFilter = TRUE;
				m_bWantPluRange = TRUE;
				m_bWantPluFrom = TRUE;
				m_bWantPluTo = TRUE;
				m_bWantBatchName = TRUE;
				m_bWantDeptSort = TRUE;
				m_bWantSupplierSort = TRUE;
				m_bWantSupplierFilter = TRUE;
				m_bWantStockZeroSkip = TRUE;
			}
			break;

		case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantConTermIdx = TRUE;
			m_bWantTableFilterType = TRUE;
			m_bWantTableFilterLevel = TRUE;
			m_bWantTableFilterEntityNo = TRUE;
			m_bWantDeptFilter = TRUE;
			m_bWantPluFilter = TRUE;
			m_bWantSaleTime = TRUE;
			m_bWantDateRoundTo = TRUE;
			m_bWantTimeSlice = TRUE;
			m_bWantPluRange = TRUE;
			m_bWantPluFrom = TRUE;
			m_bWantPluTo = TRUE;
			m_bWantDeptFilterType = TRUE;
			m_bWantModifierFilter = TRUE;
			m_bWantModifierCustom = TRUE;
			m_bWantTaxBandFilter = TRUE;
			m_bWantTaxBandCustom = TRUE;
			m_bWantPriceLevelFilter = TRUE;
			m_bWantPriceLevelCustom = TRUE;
			m_bWantEposSales = TRUE;
			m_bWantImportSales = TRUE;
			m_bWantManualSales = TRUE;
			m_bWantPMSTerminal = TRUE;
			m_bWantCustomerOnly = TRUE;
			m_bWantVIPOnly = TRUE;
			m_bWantSalesExportFields = TRUE;
			m_bWantTaxDP = TRUE;
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantConTermIdx = TRUE;
			m_bWantDateRoundTo = TRUE;
			m_bWantTimeSlice = TRUE;
			m_bWantPMSTerminal = TRUE;
			m_bWantPaymentExportFields = TRUE;
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantConTermIdx = TRUE;
			m_bWantSaleTime = TRUE;
			m_bWantDateRoundTo = TRUE;
			m_bWantTimeSlice = TRUE;
			m_bWantPMSTerminal = TRUE;
			m_bWantVoidExportFields = TRUE;
			break;

		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
			m_bWantBatchName = TRUE;
			m_bWantDateType = TRUE;
			m_bWantSessionLegacy = TRUE;
			m_bWantSessionEntity = TRUE;
			m_bWantConType = TRUE;
			m_bWantConDbNo = TRUE;
			m_bWantConNwkNo = TRUE;
			m_bWantConEntityNo = TRUE;
			m_bWantConTermIdx = TRUE;
			m_bWantTableFilterType = TRUE;
			m_bWantTableFilterLevel = TRUE;
			m_bWantTableFilterEntityNo = TRUE;
			m_bWantDeptFilter = TRUE;
			m_bWantPluFilter = TRUE;
			m_bWantSaleTime = TRUE;
			m_bWantDateRoundTo = TRUE;
			m_bWantTimeSlice = TRUE;
			m_bWantPluRange = TRUE;
			m_bWantPluFrom = TRUE;
			m_bWantPluTo = TRUE;
			m_bWantDeptFilterType = TRUE;
			m_bWantModifierFilter = TRUE;
			m_bWantModifierCustom = TRUE;
			m_bWantTaxBandFilter = TRUE;
			m_bWantTaxBandCustom = TRUE;
			m_bWantPriceLevelFilter = TRUE;
			m_bWantPriceLevelCustom = TRUE;
			m_bWantEposSales = TRUE;
			m_bWantImportSales = TRUE;
			m_bWantManualSales = TRUE;
			m_bWantPMSTerminal = TRUE;
			m_bWantCustomerOnly = TRUE;
			m_bWantVIPOnly = TRUE;
			m_bWantOfferFilter = TRUE;
			m_bWantDiscountExportFields = TRUE;
			break;
		}
	}
		
	if ( nTaskMode == POSTRAY_TASKMODE_ALL )
		m_bWantMergeDelete = TRUE;
}

/**********************************************************************/
