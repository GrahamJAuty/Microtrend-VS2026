/**********************************************************************/
#include "..\CommonEcrTray\ReportPluStockLevel.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\SalesHistoryReportPlu.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "EposReportBase.h"
#include "LocationCSVArray.h"
#include "PosTrayTaskFieldMask.h"
#include "PosTrayTaskIterator.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/

CPosTrayTaskSortInfo::CPosTrayTaskSortInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayTaskSortInfo::Reset()
{
	m_nFieldGroupForSort = 0;
	m_nFieldTypeForSort = 0;
	m_nFieldIdxForSort = 0;
	m_nFieldTypeForList = 0;
}

/**********************************************************************/

void CPosTrayTaskSortInfo::Add (CPosTrayTaskSortInfo& source )
{
}

/**********************************************************************/

int CPosTrayTaskSortInfo::Compare( CPosTrayTaskSortInfo& source, int nHint )
{
	if ( m_nFieldGroupForSort != source.m_nFieldGroupForSort )
		return ( ( m_nFieldGroupForSort > source.m_nFieldGroupForSort ) ? 1 : -1 );

	if ( m_nFieldTypeForSort != source.m_nFieldTypeForSort )
		return ( ( m_nFieldTypeForSort > source.m_nFieldTypeForSort ) ? 1 : -1 );
	
	if ( m_nFieldIdxForSort != source.m_nFieldIdxForSort )
		return ( ( m_nFieldIdxForSort > source.m_nFieldIdxForSort ) ? 1 : -1 );	
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTraySelectedReport::CPosTraySelectedReport()
{
	m_strReportID = "";
	m_strReportName = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayTask::CPosTrayTask( int nMode )
{
	m_TaskName.Create( "TaskName", POSTRAY_MAXLENGTH_TASK_NAME, "" );
	m_TaskBatchNo.Create( "TaskBatchNo", 0, POSTRAY_MAX_BATCH, 0 );
	
	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		CString str = "";
		str.Format("TaskEmailNickname%d", n + 1);
		m_TaskEmailNickname[n].Create(str, 200, "");
	}
	
	m_TaskFormatEmail.Create( "TaskEmailFormat", -1, 1, -1 );
	m_TaskEmailByReport.Create("TaskEmailByReport", 2000, "");
	m_TaskFormatExport.Create( "TaskExportFormat", -1, 1, 0 );
	m_TaskExportFolder.Create( "TaskExportFolder", 200, "" );
	m_TaskExportFilename.Create ( "TaskExportFilename", 50, "" );
	m_TaskExportNameFormat.Create ( "TaskExportFilenameFormat", 0, 5, 0 );
	m_TaskLocNameFlag.Create("TaskLocNameFlag", FALSE);
	m_TaskFTPFlag.Create( "TaskFTPFlag", FALSE );
	m_TaskFTPFolder.Create( "TaskFTPFolder", 200, "" );
	m_TaskExportDateType.Create( "TaskExportDateType", 0, 4, 0 );
	m_TaskAutoPrint.Create ( "TaskAutoPrint", FALSE );
	m_LegacyTaskHour.Create( "TaskTimeHour", 0, 23, 0 );
	m_LegacyTaskMinute.Create( "TaskTimeMinute", 0, 59, 0 );
	m_TaskHourStart.Create ( "TaskTimeHourStart", -1, 23, -1 );
	m_TaskMinuteStart.Create ( "TaskTimeMinuteStart", -1, 59, -1 );
	m_TaskHourEnd.Create ( "TaskTimeHourEnd", -1, 23, -1 );
	m_TaskMinuteEnd.Create ( "TaskTimeMinuteEnd", -1, 59, -1 );
	m_TaskFrequency.Create ( "TaskTimeFrequency", 1, 60, 60 );
	m_TaskSun.Create( "TaskDaySun", FALSE );
	m_TaskMon.Create( "TaskDayMon", FALSE );
	m_TaskTue.Create( "TaskDayTue", FALSE );
	m_TaskWed.Create( "TaskDayWed", FALSE );
	m_TaskThu.Create( "TaskDayThu", FALSE );
	m_TaskFri.Create( "TaskDayFri", FALSE );
	m_TaskSat.Create( "TaskDaySat", FALSE );
	m_TaskMonthDays.Create("TaskMonthDays", 1, GetAllMonthDaysMask(), GetAllMonthDaysMask());
	m_TaskBacklog.Create( "TaskBacklog", TRUE );

	CString strDefaultDateRange = EcrmanOptions.GetReportsDefaultDateRange();
	if (strDefaultDateRange.Left(6) == "Custom")
	{
		strDefaultDateRange = "Today";
	}
		
	m_BatchName.Create ( "BatchName", POSTRAY_MAXLENGTH_BATCH_NAME, strDefaultDateRange );
	m_BatchDateType.Create( "BatchDateType", 100, strDefaultDateRange );
	m_BatchSessionLegacy.Create( "BatchSession", 100, "" );
	m_BatchSessionEntity.Create( "BatchSessionEntity", 100, "" );
	m_BatchConType.Create( "BatchConType", 0, 9999, 0 );
	m_BatchConDbNo.Create( "BatchConDbNo", 0, 9999, 0 );
	m_BatchConNwkNo.Create( "BatchConNwkNo", 0, 9999, 0 );
	m_BatchConEntityNo.Create( "BatchConEntityNo", 0, 9999, 0 );
	m_BatchConTermIdx.Create( "BatchConTermIdx", 0, 9999, 0 );
	m_BatchTermDateType.Create("BatchTermDateType", 0, 2, 0);
	m_BatchTableFilterType.Create( "BatchTableFilterType", 0, 3, 0 );
	m_BatchTableFilterLevel.Create( "BatchTableFilterLevel", 1, 3, 1 );
	m_BatchTableFilterEntityNo.Create( "BatchTableFilterEntityNo", 1, 999, 1 );
	m_BatchDeptFilter.Create( "BatchDeptFilter", 0, 99999, DEPARTMENTSET_REPORT );
	m_BatchPluFilter.Create( "BatchPluFilter", 0, 999, 0 );
	m_BatchSaleTime.Create( "BatchSaleTime", FALSE );
	m_BatchPreferConsolType.Create( "BatchPreferConsolType", 0, 2, 0 );
	m_BatchJobSetConsolType.Create( "BatchJobSetConsolType", FALSE );
	m_BatchReportEcr.Create( "BatchReportEcr", FALSE );
	m_BatchReportLoc.Create( "BatchReportLoc", FALSE );
	m_BatchReportAll.Create( "BatchReportAll", FALSE );
	m_BatchReportCon.Create( "BatchReportCon", FALSE );
	m_BatchItemServer.Create( "BatchFlagItemServer", FALSE );
	m_BatchTrainServer.Create( "BatchFlagTrainServer", FALSE );
	m_BatchPMSType.Create( "BatchPMSType", 0, 2, 0 );
	m_BatchPMSPending.Create( "BatchFlagPMSPending", FALSE );
	m_BatchPMSActive.Create( "BatchFlagPMSActive", FALSE );
	m_BatchPMSComplete.Create( "BatchFlagPMSComplete", FALSE );
	m_BatchPMSCancelled.Create( "BatchFlagPMSCancelled", FALSE );
	m_BatchReportList.Create( "BatchReportList", 1000, "" );
	m_BatchReportTime.Create( "BatchReportTime", FALSE );

	m_BatchDateRoundTo.Create( "BatchDateRoundTo", 0, 2, 0 );
	m_BatchTimeSlice.Create( "BatchTimeSlice", 200, "" );
	m_BatchPluRange.Create( "BatchPluRange", FALSE );
	m_BatchPluFrom.Create( "BatchPluNoFrom", 0, 99999999999999, 0 );
	m_BatchPluTo.Create( "BatchPluNoTo", 0, 99999999999999, 0 );
	m_BatchDeptFilterType.Create( "BatchDeptFilterType", 0, 1, 0 );
	m_BatchModifierFilter.Create( "BatchModifierFilter", 0, 999, 999 );
	m_BatchModifierCustom.Create( "BatchModifierCustom", 0, 0x3FF, 0x3FF ); 
	m_BatchTaxBandFilter.Create( "BatchTaxBandFilter", 0, 999, 999 );
	m_BatchTaxBandCustom.Create( "BatchTaxBandCustom", 0, TAXBAND_FILTER_ALL, TAXBAND_FILTER_ALL ); 
	m_BatchPriceLevelFilter.Create( "BatchPriceLevelFilter", 0, 999, 999 );
	m_BatchPriceLevelCustom.Create( "BatchPriceLevelCustom", 0, 0x3FF, 0x3FF );
	m_BatchEposSales.Create( "BatchEposSales", TRUE );
	m_BatchManualSales.Create( "BatchManualSales", FALSE );
	m_BatchImportSales.Create( "BatchImportSales", FALSE );
	m_BatchPMSTerminal.Create( "BatchPMSTerminal", FALSE );
	m_BatchMonthSort.Create( "BatchMonthSort", FALSE );
	m_BatchDeptSort.Create( "BatchDeptSort", FALSE );
	m_BatchSupplierSort.Create( "BatchSupplierSort", FALSE );
	m_BatchSupplierFilter.Create( "BatchSupplierFilter", -1, 999, -1 );
	m_BatchStockZeroSkip.Create( "BatchStockZeroSkip", FALSE );
	m_BatchPercentage.Create( "BatchPercentage", FALSE );
	m_BatchEstProfit.Create( "BatchEstProfit", FALSE );
	m_BatchCustomerOnly.Create( "BatchCustomerOnly", FALSE );
	m_BatchVIPOnly.Create( "BatchVIPOnly", FALSE );
	m_BatchChartType.Create( "BatchChartType", 0, 2, 0 );
	m_BatchChartBottom.Create( "BatchChartBottom100", FALSE );
	m_BatchChartValue.Create( "BatchChartByValue", FALSE );
	m_BatchChartSize.Create( "BatchChartSize", 1, 9999, 100 );
	m_BatchChartCutOff.Create( "BatchChartCutOff", -99999999.99999, 99999999.99999, 0.0 );
	m_BatchTaxDP.Create( "BatchVatDecimalPlaces", -1, 8, -1 );
	m_BatchOfferFilter.Create( "BatchOfferFilter", 0, 511, 511 );
	m_BatchExportFieldSales.Create( "BatchExportFieldSales", 0, 2999, 0, POSTRAY_MAX_FIELDS_PLU );
	m_BatchExportFieldPayment.Create( "BatchExportFieldPayment", 0, 2999, 0, POSTRAY_MAX_FIELDS_PAYMENT );
	m_BatchExportFieldVoid.Create( "BatchExportFieldVoid", 0, 2999, 0, POSTRAY_MAX_FIELDS_VOID );
	m_BatchExportFieldDiscount.Create( "BatchExportFieldDiscount", 0, 2999, 0, POSTRAY_MAX_FIELDS_DISCOUNT );

	m_BatchTAEdit.Create("BatchTAEdit", 0, 2, 0);
	m_BatchTAGroup.Create("BatchTAGroup", 0, 1, 0);
	m_BatchTATotalHours.Create("BatchTATotalHours", 0, 1, 0);
	m_BatchTAOpenShift.Create("BatchTAOpenShift", FALSE);
	m_BatchTAClosingTime.Create("BatchTAClosingTime", FALSE);
	m_BatchTAServerPickType.Create("BatchTAServerPickType", 0, 1, 0);
	m_BatchTAServerPickFrom.Create("BatchTAServerPickFrom", 1, 9999, 1);
	m_BatchTAServerPickTo.Create("BatchTAServerPickTo", 1, 9999, 9999);

	m_LegacyBatchTaxFilter.Create( "BatchTaxCodeFilter", 0, 999, 999 );
	m_LegacyBatchTaxCustom.Create( "BatchTaxCodeCustom", 0, TAXBAND_FILTER_ALL, TAXBAND_FILTER_ALL);
	
	m_MergeDelete.Create( "MergeDelete", FALSE );
		
	Reset();
	m_bFatalReadError = FALSE;

	switch( nMode )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_EPOS_ALL:
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		m_nMode = nMode;
		break;

	default:
		m_nMode = POSTRAY_TASKFILE_EPOS_ALL;
		break;
	}
}

/**********************************************************************/

CString CPosTrayTask::GetTaskEmailNickname(int n)
{
	CString strResult = "";
	if ((n >= 0) && (n < POSTRAY_MAX_EMAIL_NICKNAMES))
	{
		strResult = m_TaskEmailNickname[n].GetValue();
	}
	return strResult;
}

/**********************************************************************/

void CPosTrayTask::SetTaskEmailNickname(int n, const char* sz)
{
	if ((n >= 0) && (n < POSTRAY_MAX_EMAIL_NICKNAMES))
	{
		m_TaskEmailNickname[n].SetValue(sz);
	}
}

/**********************************************************************/

bool CPosTrayTask::IsProcessTranTask()
{
	switch (m_nMode)
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPosTrayTask::IsExportTask()
{
	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		return TRUE;
		
	default:
		return FALSE;
	}
}

/**********************************************************************/

void CPosTrayTask::CopyFrom( CPosTrayTask& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CPosTrayTask::QuickLoadExportFolder( const char* szTaskFilename )
{
	CEnhancedIniFile iniFile;
	if (iniFile.Read(szTaskFilename) == TRUE)
	{
		iniFile.ReadString(m_TaskExportFolder);
	}
	else
	{
		m_TaskExportFolder.SetValue("");
	}
}

/**********************************************************************/

void CPosTrayTask::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	CPosTrayTaskFieldMask Mask;
	Mask.SetMode(m_nMode);

	if (TRUE == Mask.m_bWantTaskName)
	{
		iniFile.ReadString(m_TaskName);
	}

	if (TRUE == Mask.m_bWantBatchNo)
	{
		iniFile.ReadInt(m_TaskBatchNo);
	}

	if (TRUE == Mask.m_bWantEmailNickname)
	{
		for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
		{
			iniFile.ReadString(m_TaskEmailNickname[n]);
		}

		CString strTest = m_TaskEmailNickname[0].GetValue();

		if (strTest == "")
		{
			CIniFileString iniLegacy;
			iniLegacy.Create("TaskEmailAddress", 200, "");
			iniFile.ReadString(iniLegacy);
			m_TaskEmailNickname->SetValue(iniLegacy.GetValue());
		}
	}

	if ( TRUE == Mask.m_bWantFormatEmail )			iniFile.ReadInt( m_TaskFormatEmail );
	if ( TRUE == Mask.m_bWantEmailByReport)			iniFile.ReadString(m_TaskEmailByReport);
	if ( TRUE == Mask.m_bWantFormatExport )			iniFile.ReadInt( m_TaskFormatExport );
	if ( TRUE == Mask.m_bWantExportFolder )			iniFile.ReadString( m_TaskExportFolder );
	if ( TRUE == Mask.m_bWantExportFilename )		iniFile.ReadString( m_TaskExportFilename );
	if ( TRUE == Mask.m_bWantExportFilenameFormat )	iniFile.ReadInt( m_TaskExportNameFormat );
	if ( TRUE == Mask.m_bWantLocNameFlag)			iniFile.ReadBool(m_TaskLocNameFlag);
	if ( TRUE == Mask.m_bWantFTPFlag )				iniFile.ReadBool( m_TaskFTPFlag );
	if ( TRUE == Mask.m_bWantFTPFolder )			iniFile.ReadString( m_TaskFTPFolder );
	if ( TRUE == Mask.m_bWantExportDateType )		iniFile.ReadInt( m_TaskExportDateType );
	if ( TRUE == Mask.m_bWantAutoPrintFlag )		iniFile.ReadBool( m_TaskAutoPrint );
	if ( TRUE == Mask.m_bWantLegacyTimeHour	)		iniFile.ReadInt( m_LegacyTaskHour );
	if ( TRUE == Mask.m_bWantLegacyTimeMinute )		iniFile.ReadInt( m_LegacyTaskMinute );
	if ( TRUE == Mask.m_bWantTimeHourStart )		iniFile.ReadInt( m_TaskHourStart );
	if ( TRUE == Mask.m_bWantTimeHourEnd )			iniFile.ReadInt( m_TaskHourEnd );
	if ( TRUE == Mask.m_bWantTimeMinuteStart )		iniFile.ReadInt( m_TaskMinuteStart );
	if ( TRUE == Mask.m_bWantTimeMinuteEnd )		iniFile.ReadInt( m_TaskMinuteEnd );
	if ( TRUE == Mask.m_bWantFrequency )			iniFile.ReadInt( m_TaskFrequency );
	if ( TRUE == Mask.m_bWantDaySunFlag )			iniFile.ReadBool( m_TaskSun );
	if ( TRUE == Mask.m_bWantDayMonFlag )			iniFile.ReadBool( m_TaskMon );
	if ( TRUE == Mask.m_bWantDayTueFlag )			iniFile.ReadBool( m_TaskTue );
	if ( TRUE == Mask.m_bWantDayWedFlag )			iniFile.ReadBool( m_TaskWed );
	if ( TRUE == Mask.m_bWantDayThuFlag )			iniFile.ReadBool( m_TaskThu );
	if ( TRUE == Mask.m_bWantDayFriFlag )			iniFile.ReadBool( m_TaskFri );
	if ( TRUE == Mask.m_bWantDaySatFlag )			iniFile.ReadBool( m_TaskSat );
	if ( TRUE == Mask.m_bWantMonthDays)				iniFile.ReadInt(m_TaskMonthDays);
	if ( TRUE == Mask.m_bWantBacklogFlag )			iniFile.ReadBool( m_TaskBacklog );
	
	/*****/
	
	if ( TRUE == Mask.m_bWantBatchName )		iniFile.ReadString( m_BatchName );
	if ( TRUE == Mask.m_bWantDateType )			iniFile.ReadString( m_BatchDateType );
	if ( TRUE == Mask.m_bWantSessionLegacy )	iniFile.ReadString( m_BatchSessionLegacy );
	if ( TRUE == Mask.m_bWantSessionEntity )	iniFile.ReadString( m_BatchSessionEntity );
	if ( TRUE == Mask.m_bWantConType )			iniFile.ReadInt( m_BatchConType );
	if ( TRUE == Mask.m_bWantConDbNo )			iniFile.ReadInt( m_BatchConDbNo );
	if ( TRUE == Mask.m_bWantConNwkNo )			iniFile.ReadInt( m_BatchConNwkNo );
	if ( TRUE == Mask.m_bWantConEntityNo )		iniFile.ReadInt( m_BatchConEntityNo );
	if ( TRUE == Mask.m_bWantConTermIdx )		iniFile.ReadInt( m_BatchConTermIdx );
	if ( TRUE == Mask.m_bWantTermDateType)		iniFile.ReadInt(m_BatchTermDateType);
	if ( TRUE == Mask.m_bWantTableFilterType )	iniFile.ReadInt( m_BatchTableFilterType );
	if ( TRUE == Mask.m_bWantTableFilterLevel )	iniFile.ReadInt( m_BatchTableFilterLevel );
	if ( TRUE == Mask.m_bWantTableFilterEntityNo )	iniFile.ReadInt( m_BatchTableFilterEntityNo );
	if ( TRUE == Mask.m_bWantDeptFilter )		iniFile.ReadInt( m_BatchDeptFilter );
	if ( TRUE == Mask.m_bWantPluFilter )		iniFile.ReadInt( m_BatchPluFilter );
	if ( TRUE == Mask.m_bWantSaleTime )			iniFile.ReadBool( m_BatchSaleTime );
	if ( TRUE == Mask.m_bWantPreferConsolType )	iniFile.ReadInt( m_BatchPreferConsolType );
	if ( TRUE == Mask.m_bWantJobSetConsolType )	iniFile.ReadBool( m_BatchJobSetConsolType );
	if ( TRUE == Mask.m_bWantReportEcr )		iniFile.ReadBool( m_BatchReportEcr );
	if ( TRUE == Mask.m_bWantReportLoc )		iniFile.ReadBool( m_BatchReportLoc );
	if ( TRUE == Mask.m_bWantReportAll )		iniFile.ReadBool( m_BatchReportAll );
	if ( TRUE == Mask.m_bWantReportCon )		iniFile.ReadBool( m_BatchReportCon );
	if ( TRUE == Mask.m_bWantItemServer )		iniFile.ReadBool( m_BatchItemServer );
	if ( TRUE == Mask.m_bWantTrainServer )		iniFile.ReadBool( m_BatchTrainServer );
	if ( TRUE == Mask.m_bWantPMSType )			iniFile.ReadInt( m_BatchPMSType );
	if ( TRUE == Mask.m_bWantPMSPending )		iniFile.ReadBool( m_BatchPMSPending );
	if ( TRUE == Mask.m_bWantPMSActive )		iniFile.ReadBool( m_BatchPMSActive );
	if ( TRUE == Mask.m_bWantPMSComplete )		iniFile.ReadBool( m_BatchPMSComplete );
	if ( TRUE == Mask.m_bWantPMSCancelled )		iniFile.ReadBool( m_BatchPMSCancelled );
	if ( TRUE == Mask.m_bWantReportList )		iniFile.ReadString( m_BatchReportList );
	if ( TRUE == Mask.m_bWantReportTime )		iniFile.ReadBool( m_BatchReportTime );
	if ( TRUE == Mask.m_bWantDateRoundTo )		iniFile.ReadInt( m_BatchDateRoundTo );
	if ( TRUE == Mask.m_bWantTimeSlice )		iniFile.ReadString( m_BatchTimeSlice );
	if ( TRUE == Mask.m_bWantPluRange )			iniFile.ReadBool( m_BatchPluRange );
	if ( TRUE == Mask.m_bWantPluFrom )			iniFile.ReadInt64( m_BatchPluFrom );
	if ( TRUE == Mask.m_bWantPluTo )			iniFile.ReadInt64( m_BatchPluTo );
	if ( TRUE == Mask.m_bWantDeptFilterType )	iniFile.ReadInt( m_BatchDeptFilterType );

	if ( TRUE == Mask.m_bWantTaxBandFilter )
	{
		iniFile.ReadInt( m_BatchTaxBandFilter );
		int nValue = m_BatchTaxBandFilter.GetValue();

		if ( nValue == m_BatchTaxBandFilter.GetDefault() )
		{
			iniFile.ReadInt( m_LegacyBatchTaxFilter );
			nValue = m_LegacyBatchTaxFilter.GetValue();
		}

		m_BatchTaxBandFilter.Reset();
		SetBatchTaxBandFilter( nValue );
	}
	
	if ( TRUE == Mask.m_bWantTaxBandCustom )	
	{
		iniFile.ReadInt( m_BatchTaxBandCustom );
		
		if ( m_BatchTaxBandCustom.GetValue() == m_BatchTaxBandCustom.GetDefault() )
		{
			iniFile.ReadInt( m_LegacyBatchTaxCustom );
			m_BatchTaxBandCustom.SetValue( m_LegacyBatchTaxCustom.GetValue() );
		}
	}

	if ( TRUE == Mask.m_bWantPriceLevelFilter )
	{
		iniFile.ReadInt( m_BatchPriceLevelFilter );
		int nValue = m_BatchPriceLevelFilter.GetValue();
		m_BatchPriceLevelFilter.Reset();
		SetBatchPriceLevelFilter( nValue );
	}
	if ( TRUE == Mask.m_bWantPriceLevelCustom )	iniFile.ReadInt( m_BatchPriceLevelCustom );

	if ( TRUE == Mask.m_bWantModifierFilter )
	{
		iniFile.ReadInt( m_BatchModifierFilter );
		int nValue = m_BatchModifierFilter.GetValue();
		m_BatchModifierFilter.Reset();
		SetBatchModifierFilter( nValue );
	}
	if ( TRUE == Mask.m_bWantModifierCustom )	iniFile.ReadInt( m_BatchModifierCustom );

	if ( TRUE == Mask.m_bWantEposSales )		iniFile.ReadBool( m_BatchEposSales );
	if ( TRUE == Mask.m_bWantManualSales )		iniFile.ReadBool( m_BatchManualSales );
	if ( TRUE == Mask.m_bWantImportSales )		iniFile.ReadBool( m_BatchImportSales );
	if ( TRUE == Mask.m_bWantPMSTerminal )		iniFile.ReadBool( m_BatchPMSTerminal );
	if ( TRUE == Mask.m_bWantMonthSort )		iniFile.ReadBool( m_BatchMonthSort );
	if ( TRUE == Mask.m_bWantDeptSort )			iniFile.ReadBool( m_BatchDeptSort );
	if ( TRUE == Mask.m_bWantSupplierSort )		iniFile.ReadBool( m_BatchSupplierSort );
	if ( TRUE == Mask.m_bWantSupplierFilter )	iniFile.ReadInt( m_BatchSupplierFilter );
	if ( TRUE == Mask.m_bWantStockZeroSkip )	iniFile.ReadBool( m_BatchStockZeroSkip );
	if ( TRUE == Mask.m_bWantPercentage )		iniFile.ReadBool( m_BatchPercentage );
	if ( TRUE == Mask.m_bWantEstProfit )		iniFile.ReadBool( m_BatchEstProfit );
	if ( TRUE == Mask.m_bWantCustomerOnly )		iniFile.ReadBool( m_BatchCustomerOnly );
	if ( TRUE == Mask.m_bWantVIPOnly )			iniFile.ReadBool( m_BatchVIPOnly );
	if ( TRUE == Mask.m_bWantOfferFilter )		iniFile.ReadInt( m_BatchOfferFilter );
	if ( TRUE == Mask.m_bWantChartType )		iniFile.ReadInt( m_BatchChartType );
	if ( TRUE == Mask.m_bWantChartBottom )		iniFile.ReadBool( m_BatchChartBottom );
	if ( TRUE == Mask.m_bWantChartValue )		iniFile.ReadBool( m_BatchChartValue );
	if ( TRUE == Mask.m_bWantChartSize )		iniFile.ReadInt( m_BatchChartSize );
	if ( TRUE == Mask.m_bWantChartCutOff )		iniFile.ReadDouble( m_BatchChartCutOff );
	if ( TRUE == Mask.m_bWantTaxDP )			iniFile.ReadInt( m_BatchTaxDP );

	if ( TRUE == Mask.m_bWantSalesExportFields )
	{
		for ( int n = 0; n < m_BatchExportFieldSales.GetCount(); n++ )
			iniFile.ReadInt( m_BatchExportFieldSales, n );
	}

	if ( TRUE == Mask.m_bWantPaymentExportFields )
	{
		for ( int n = 0; n < m_BatchExportFieldPayment.GetCount(); n++ )
			iniFile.ReadInt( m_BatchExportFieldPayment, n );
	}

	if ( TRUE == Mask.m_bWantVoidExportFields )
	{
		for ( int n = 0; n < m_BatchExportFieldVoid.GetCount(); n++ )
			iniFile.ReadInt( m_BatchExportFieldVoid, n );
	}

	if ( TRUE == Mask.m_bWantDiscountExportFields )
	{
		for ( int n = 0; n < m_BatchExportFieldDiscount.GetCount(); n++ )
			iniFile.ReadInt( m_BatchExportFieldDiscount, n );
	}
	
	/*****/

	if (TRUE == Mask.m_bWantTimeAttend)
	{
		iniFile.ReadInt(m_BatchTAGroup);
		iniFile.ReadInt(m_BatchTAEdit);
		iniFile.ReadInt(m_BatchTATotalHours);
		iniFile.ReadBool(m_BatchTAOpenShift);
		iniFile.ReadBool(m_BatchTAClosingTime);
		iniFile.ReadInt(m_BatchTAServerPickType);
		iniFile.ReadInt(m_BatchTAServerPickFrom);
		iniFile.ReadInt(m_BatchTAServerPickTo);
	}

	/*****/

	if ( TRUE == Mask.m_bWantMergeDelete )		iniFile.ReadBool( m_MergeDelete );

	/*****/
	
	if ( ( m_TaskHourStart.GetValue() == -1 ) || ( m_TaskMinuteStart.GetValue() == -1 ) )
	{
		m_TaskHourStart.SetValue( m_LegacyTaskHour.GetValue() );
		m_TaskMinuteStart.SetValue( m_LegacyTaskMinute.GetValue() );
	}

	if ( ( m_TaskHourEnd.GetValue() == -1 ) || ( m_TaskMinuteEnd.GetValue() == -1 ) )
	{
		m_TaskHourEnd.SetValue( m_LegacyTaskHour.GetValue() );
		m_TaskMinuteEnd.SetValue( m_LegacyTaskMinute.GetValue() );
	}
}

/**********************************************************************/

void CPosTrayTask::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	CPosTrayTaskFieldMask Mask;
	Mask.SetMode(m_nMode);

	if (TRUE == Mask.m_bWantTaskName)				iniFile.WriteString(m_TaskName);
	if (TRUE == Mask.m_bWantBatchNo)				iniFile.WriteInt(m_TaskBatchNo);

	if (TRUE == Mask.m_bWantEmailNickname)
	{
		for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
		{
			iniFile.WriteString(m_TaskEmailNickname[n]);
		}
	}

	if (TRUE == Mask.m_bWantFormatEmail)			iniFile.WriteInt(m_TaskFormatEmail);
	if (TRUE == Mask.m_bWantEmailByReport)			iniFile.WriteString(m_TaskEmailByReport);
	if (TRUE == Mask.m_bWantFormatExport)			iniFile.WriteInt(m_TaskFormatExport);
	if (TRUE == Mask.m_bWantExportFolder)			iniFile.WriteString(m_TaskExportFolder);
	if (TRUE == Mask.m_bWantExportFilename)		iniFile.WriteString(m_TaskExportFilename);
	if (TRUE == Mask.m_bWantExportFilenameFormat)	iniFile.WriteInt(m_TaskExportNameFormat);
	if (TRUE == Mask.m_bWantLocNameFlag)			iniFile.WriteBool(m_TaskLocNameFlag);
	if (TRUE == Mask.m_bWantFTPFlag)				iniFile.WriteBool(m_TaskFTPFlag);
	if (TRUE == Mask.m_bWantFTPFolder)			iniFile.WriteString(m_TaskFTPFolder);
	if (TRUE == Mask.m_bWantExportDateType)		iniFile.WriteInt(m_TaskExportDateType);
	if (TRUE == Mask.m_bWantAutoPrintFlag)		iniFile.WriteBool(m_TaskAutoPrint);
	if (TRUE == Mask.m_bWantLegacyTimeHour)		iniFile.WriteInt(m_LegacyTaskHour);
	if (TRUE == Mask.m_bWantLegacyTimeMinute)		iniFile.WriteInt(m_LegacyTaskMinute);
	if (TRUE == Mask.m_bWantTimeHourStart)		iniFile.WriteInt(m_TaskHourStart);
	if (TRUE == Mask.m_bWantTimeHourEnd)			iniFile.WriteInt(m_TaskHourEnd);
	if (TRUE == Mask.m_bWantTimeMinuteStart)		iniFile.WriteInt(m_TaskMinuteStart);
	if (TRUE == Mask.m_bWantTimeMinuteEnd)		iniFile.WriteInt(m_TaskMinuteEnd);
	if (TRUE == Mask.m_bWantFrequency)			iniFile.WriteInt(m_TaskFrequency);
	if (TRUE == Mask.m_bWantDaySunFlag)			iniFile.WriteBool(m_TaskSun);
	if (TRUE == Mask.m_bWantDayMonFlag)			iniFile.WriteBool(m_TaskMon);
	if (TRUE == Mask.m_bWantDayTueFlag)			iniFile.WriteBool(m_TaskTue);
	if (TRUE == Mask.m_bWantDayWedFlag)			iniFile.WriteBool(m_TaskWed);
	if (TRUE == Mask.m_bWantDayThuFlag)			iniFile.WriteBool(m_TaskThu);
	if (TRUE == Mask.m_bWantDayFriFlag)			iniFile.WriteBool(m_TaskFri);
	if (TRUE == Mask.m_bWantDaySatFlag)			iniFile.WriteBool(m_TaskSat);
	if (TRUE == Mask.m_bWantMonthDays)			iniFile.WriteInt(m_TaskMonthDays);
	if (TRUE == Mask.m_bWantBacklogFlag)			iniFile.WriteBool(m_TaskBacklog);

	/*****/

	if (TRUE == Mask.m_bWantBatchName)			iniFile.WriteString(m_BatchName);
	if (TRUE == Mask.m_bWantDateType)				iniFile.WriteString(m_BatchDateType);

	if (TRUE == Mask.m_bWantSessionLegacy)
	{
		CString strLegacy = m_BatchSessionLegacy.GetValue();

		if (strLegacy != "")
		{
			iniFile.WriteString(m_BatchSessionLegacy);
		}
	}

	if (TRUE == Mask.m_bWantSessionEntity)		iniFile.WriteString(m_BatchSessionEntity);
	if (TRUE == Mask.m_bWantConType)				iniFile.WriteInt(m_BatchConType);
	if (TRUE == Mask.m_bWantConDbNo)				iniFile.WriteInt(m_BatchConDbNo);
	if (TRUE == Mask.m_bWantConNwkNo)				iniFile.WriteInt(m_BatchConNwkNo);
	if (TRUE == Mask.m_bWantConEntityNo)			iniFile.WriteInt(m_BatchConEntityNo);
	if (TRUE == Mask.m_bWantConTermIdx)			iniFile.WriteInt(m_BatchConTermIdx);
	if (TRUE == Mask.m_bWantTermDateType)			iniFile.WriteInt(m_BatchTermDateType);
	if (TRUE == Mask.m_bWantTableFilterType)		iniFile.WriteInt(m_BatchTableFilterType);
	if (TRUE == Mask.m_bWantTableFilterLevel)		iniFile.WriteInt(m_BatchTableFilterLevel);
	if (TRUE == Mask.m_bWantTableFilterEntityNo)	iniFile.WriteInt(m_BatchTableFilterEntityNo);
	if (TRUE == Mask.m_bWantDeptFilter)			iniFile.WriteInt(m_BatchDeptFilter);
	if (TRUE == Mask.m_bWantPluFilter)			iniFile.WriteInt(m_BatchPluFilter);
	if (TRUE == Mask.m_bWantSaleTime)				iniFile.WriteBool(m_BatchSaleTime);
	if (TRUE == Mask.m_bWantPreferConsolType)		iniFile.WriteInt(m_BatchPreferConsolType);
	if (TRUE == Mask.m_bWantJobSetConsolType)		iniFile.WriteBool(m_BatchJobSetConsolType);
	if (TRUE == Mask.m_bWantReportEcr)			iniFile.WriteBool(m_BatchReportEcr);
	if (TRUE == Mask.m_bWantReportLoc)			iniFile.WriteBool(m_BatchReportLoc);
	if (TRUE == Mask.m_bWantReportAll)			iniFile.WriteBool(m_BatchReportAll);
	if (TRUE == Mask.m_bWantReportCon)			iniFile.WriteBool(m_BatchReportCon);
	if (TRUE == Mask.m_bWantItemServer)			iniFile.WriteBool(m_BatchItemServer);
	if (TRUE == Mask.m_bWantTrainServer)			iniFile.WriteBool(m_BatchTrainServer);
	if (TRUE == Mask.m_bWantPMSType)				iniFile.WriteInt(m_BatchPMSType);
	if (TRUE == Mask.m_bWantPMSPending)			iniFile.WriteBool(m_BatchPMSPending);
	if (TRUE == Mask.m_bWantPMSActive)			iniFile.WriteBool(m_BatchPMSActive);
	if (TRUE == Mask.m_bWantPMSComplete)			iniFile.WriteBool(m_BatchPMSComplete);
	if (TRUE == Mask.m_bWantPMSCancelled)			iniFile.WriteBool(m_BatchPMSCancelled);
	if (TRUE == Mask.m_bWantReportList)			iniFile.WriteString(m_BatchReportList);
	if (TRUE == Mask.m_bWantReportTime)			iniFile.WriteBool(m_BatchReportTime);
	if (TRUE == Mask.m_bWantDateRoundTo)			iniFile.WriteInt(m_BatchDateRoundTo);
	if (TRUE == Mask.m_bWantTimeSlice)			iniFile.WriteString(m_BatchTimeSlice);
	if (TRUE == Mask.m_bWantPluRange)				iniFile.WriteBool(m_BatchPluRange);
	if (TRUE == Mask.m_bWantPluFrom)				iniFile.WriteInt64(m_BatchPluFrom);
	if (TRUE == Mask.m_bWantPluTo)				iniFile.WriteInt64(m_BatchPluTo);
	if (TRUE == Mask.m_bWantDeptFilterType)		iniFile.WriteInt(m_BatchDeptFilterType);
	if (TRUE == Mask.m_bWantModifierFilter)		iniFile.WriteInt(m_BatchModifierFilter);
	if (TRUE == Mask.m_bWantModifierCustom)		iniFile.WriteInt(m_BatchModifierCustom);
	if (TRUE == Mask.m_bWantTaxBandFilter)		iniFile.WriteInt(m_BatchTaxBandFilter);
	if (TRUE == Mask.m_bWantTaxBandCustom)		iniFile.WriteInt(m_BatchTaxBandCustom);
	if (TRUE == Mask.m_bWantPriceLevelFilter)		iniFile.WriteInt(m_BatchPriceLevelFilter);
	if (TRUE == Mask.m_bWantPriceLevelCustom)		iniFile.WriteInt(m_BatchPriceLevelCustom);
	if (TRUE == Mask.m_bWantEposSales)			iniFile.WriteBool(m_BatchEposSales);
	if (TRUE == Mask.m_bWantManualSales)			iniFile.WriteBool(m_BatchManualSales);
	if (TRUE == Mask.m_bWantImportSales)			iniFile.WriteBool(m_BatchImportSales);
	if (TRUE == Mask.m_bWantPMSTerminal)			iniFile.WriteBool(m_BatchPMSTerminal);
	if (TRUE == Mask.m_bWantMonthSort)			iniFile.WriteBool(m_BatchMonthSort);
	if (TRUE == Mask.m_bWantDeptSort)				iniFile.WriteBool(m_BatchDeptSort);
	if (TRUE == Mask.m_bWantSupplierSort)			iniFile.WriteBool(m_BatchSupplierSort);
	if (TRUE == Mask.m_bWantSupplierFilter)		iniFile.WriteInt(m_BatchSupplierFilter);
	if (TRUE == Mask.m_bWantStockZeroSkip)		iniFile.WriteBool(m_BatchStockZeroSkip);
	if (TRUE == Mask.m_bWantPercentage)			iniFile.WriteBool(m_BatchPercentage);
	if (TRUE == Mask.m_bWantEstProfit)			iniFile.WriteBool(m_BatchEstProfit);
	if (TRUE == Mask.m_bWantCustomerOnly)			iniFile.WriteBool(m_BatchCustomerOnly);
	if (TRUE == Mask.m_bWantVIPOnly)				iniFile.WriteBool(m_BatchVIPOnly);
	if (TRUE == Mask.m_bWantOfferFilter)			iniFile.WriteInt(m_BatchOfferFilter);
	if (TRUE == Mask.m_bWantChartType)			iniFile.WriteInt(m_BatchChartType);
	if (TRUE == Mask.m_bWantChartBottom)			iniFile.WriteBool(m_BatchChartBottom);
	if (TRUE == Mask.m_bWantChartValue)			iniFile.WriteBool(m_BatchChartValue);
	if (TRUE == Mask.m_bWantChartSize)			iniFile.WriteInt(m_BatchChartSize);
	if (TRUE == Mask.m_bWantChartCutOff)			iniFile.WriteDouble(m_BatchChartCutOff, 2);
	if (TRUE == Mask.m_bWantTaxDP)				iniFile.WriteInt(m_BatchTaxDP);

	if (TRUE == Mask.m_bWantSalesExportFields)
	{
		for (int n = 0; n < m_BatchExportFieldSales.GetCount(); n++)
		{
			iniFile.WriteInt(m_BatchExportFieldSales, n);
		}
	}

	if (TRUE == Mask.m_bWantPaymentExportFields)
	{
		for (int n = 0; n < m_BatchExportFieldPayment.GetCount(); n++)
		{
			iniFile.WriteInt(m_BatchExportFieldPayment, n);
		}
	}

	if (TRUE == Mask.m_bWantVoidExportFields)
	{
		for (int n = 0; n < m_BatchExportFieldVoid.GetCount(); n++)
		{
			iniFile.WriteInt(m_BatchExportFieldVoid, n);
		}
	}

	if (TRUE == Mask.m_bWantDiscountExportFields)
	{
		for (int n = 0; n < m_BatchExportFieldDiscount.GetCount(); n++)
		{
			iniFile.WriteInt(m_BatchExportFieldDiscount, n);
		}
	}

	/*****/

	if (TRUE == Mask.m_bWantTimeAttend)
	{
		iniFile.WriteInt(m_BatchTAGroup);
		iniFile.WriteInt(m_BatchTAEdit);
		iniFile.WriteInt(m_BatchTATotalHours);
		iniFile.WriteBool(m_BatchTAOpenShift);
		iniFile.WriteBool(m_BatchTAClosingTime);
		iniFile.WriteInt(m_BatchTAServerPickType);
		iniFile.WriteInt(m_BatchTAServerPickFrom);
		iniFile.WriteInt(m_BatchTAServerPickTo);
	}

	/*****/

	if (TRUE == Mask.m_bWantMergeDelete)
	{
		iniFile.WriteBool(m_MergeDelete);
	}
}

/**********************************************************************/

void CPosTrayTask::MergeTask( CPosTrayTask& source )
{
	bool bCanMerge = FALSE;

	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_TIMEATTEND_TASK);
		break;

	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_PROCESSTRAN_TASK);
		break;

	case POSTRAY_TASKFILE_EPOS_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_EPOS_TASK );
		break;

	case POSTRAY_TASKFILE_HIST_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_HIST_TASK );
		break;

	case POSTRAY_TASKFILE_STKLEVEL_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_STKLEVEL_TASK );
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_EXPORT_SALES_TASK );
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK );
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_EXPORT_VOID_TASK );
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		bCanMerge = ( source.GetMode() == POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK );
		break;
	}

	if (TRUE == bCanMerge)
	{
		CPosTrayTaskFieldMask Mask;
		Mask.SetMode(m_nMode);
		if (TRUE == Mask.m_bWantTaskName)					SetTaskName(source.GetTaskName());
		if (TRUE == Mask.m_bWantBatchNo)					SetTaskBatchNo(source.GetTaskBatchNo());

		if (TRUE == Mask.m_bWantEmailNickname)
		{
			for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
			{
				SetTaskEmailNickname(n, source.GetTaskEmailNickname(n));
			}
		}

		if (TRUE == Mask.m_bWantFormatEmail)				SetTaskFormatEmail(source.GetTaskFormatEmail());
		if (TRUE == Mask.m_bWantEmailByReport)				SetTaskEmailByReport(source.GetTaskEmailByReport());
		if (TRUE == Mask.m_bWantFormatExport)				SetTaskFormatExport(source.GetTaskFormatExport());
		if (TRUE == Mask.m_bWantExportFolder)				SetTaskExportFolder(source.GetTaskExportFolder());
		if (TRUE == Mask.m_bWantExportFilename)				SetTaskExportFilename(source.GetTaskExportFilename());
		if (TRUE == Mask.m_bWantExportFilenameFormat)		SetTaskExportFilenameFormat(source.GetTaskExportFilenameFormat());
		if (TRUE == Mask.m_bWantLocNameFlag)				SetTaskLocNameFlag(source.GetTaskLocNameFlag());
		if (TRUE == Mask.m_bWantFTPFlag)					SetTaskFTPFlag(source.GetTaskFTPFlag());
		if (TRUE == Mask.m_bWantFTPFolder)					SetTaskFTPFolder(source.GetTaskFTPFolder());
		if (TRUE == Mask.m_bWantExportDateType)				SetTaskExportDateType(source.GetTaskExportDateType());
		if (TRUE == Mask.m_bWantAutoPrintFlag)				SetTaskAutoPrintFlag(source.GetTaskAutoPrintFlag());
		if (TRUE == Mask.m_bWantTimeHourStart)				SetTaskTimeHourStart(source.GetTaskTimeHourStart());
		if (TRUE == Mask.m_bWantTimeMinuteStart)			SetTaskTimeMinuteStart(source.GetTaskTimeMinuteStart());
		if (TRUE == Mask.m_bWantTimeHourEnd)				SetTaskTimeHourEnd(source.GetTaskTimeHourEnd());
		if (TRUE == Mask.m_bWantTimeMinuteEnd)				SetTaskTimeMinuteEnd(source.GetTaskTimeMinuteEnd());
		if (TRUE == Mask.m_bWantFrequency)					SetTaskTimeFrequency(source.GetTaskTimeFrequency());
		if (TRUE == Mask.m_bWantDaySunFlag)					SetTaskDaySunFlag(source.GetTaskDaySunFlag());
		if (TRUE == Mask.m_bWantDayMonFlag)					SetTaskDayMonFlag(source.GetTaskDayMonFlag());
		if (TRUE == Mask.m_bWantDayTueFlag)					SetTaskDayTueFlag(source.GetTaskDayTueFlag());
		if (TRUE == Mask.m_bWantDayWedFlag)					SetTaskDayWedFlag(source.GetTaskDayWedFlag());
		if (TRUE == Mask.m_bWantDayThuFlag)					SetTaskDayThuFlag(source.GetTaskDayThuFlag());
		if (TRUE == Mask.m_bWantDayFriFlag)					SetTaskDayFriFlag(source.GetTaskDayFriFlag());
		if (TRUE == Mask.m_bWantDaySatFlag)					SetTaskDaySatFlag(source.GetTaskDaySatFlag());
		if (TRUE == Mask.m_bWantMonthDays)					SetTaskMonthDays(source.GetTaskMonthDays());
		if (TRUE == Mask.m_bWantBacklogFlag)				SetTaskBacklogFlag(source.GetTaskBacklogFlag());
	}
}

/**********************************************************************/

void CPosTrayTask::MergeBatch(CPosTrayTask& source)
{
	bool bCanMerge = FALSE;

	switch (m_nMode)
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_PROCESSTRAN_BATCH);
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_TIMEATTEND_BATCH);
		break;

	case POSTRAY_TASKFILE_EPOS_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_EPOS_BATCH);
		break;

	case POSTRAY_TASKFILE_HIST_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_HIST_BATCH);
		break;

	case POSTRAY_TASKFILE_STKLEVEL_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_STKLEVEL_BATCH);
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_EXPORT_SALES_BATCH);
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH);
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_EXPORT_VOID_BATCH);
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		bCanMerge = (source.GetMode() == POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH);
		break;
	}

	if (TRUE == bCanMerge)
	{
		CPosTrayTaskFieldMask Mask;
		Mask.SetMode(m_nMode);
		if (TRUE == Mask.m_bWantBatchName)		SetBatchName(source.GetBatchName());
		if (TRUE == Mask.m_bWantDateType)			SetBatchDateType(source.GetBatchDateType());
		if (TRUE == Mask.m_bWantSessionLegacy)	SetBatchSessionLegacy(source.GetBatchSessionLegacy());
		if (TRUE == Mask.m_bWantSessionEntity)	SetBatchSessionEntity(source.GetBatchSessionEntity());
		if (TRUE == Mask.m_bWantConType)			SetBatchConType(source.GetBatchConType());
		if (TRUE == Mask.m_bWantConDbNo)			SetBatchConDbNo(source.GetBatchConDbNo());
		if (TRUE == Mask.m_bWantConNwkNo)			SetBatchConNwkNo(source.GetBatchConNwkNo());
		if (TRUE == Mask.m_bWantConEntityNo)		SetBatchConEntityNo(source.GetBatchConEntityNo());
		if (TRUE == Mask.m_bWantConTermIdx)		SetBatchConTermIdx(source.GetBatchConTermIdx());
		if (TRUE == Mask.m_bWantTermDateType)		SetBatchTermDateType(source.GetBatchTermDateType());
		if (TRUE == Mask.m_bWantTableFilterType)	SetBatchTableFilterType(source.GetBatchTableFilterType());
		if (TRUE == Mask.m_bWantTableFilterLevel)	SetBatchTableFilterLevel(source.GetBatchTableFilterLevel());
		if (TRUE == Mask.m_bWantTableFilterEntityNo) SetBatchTableFilterEntityNo(source.GetBatchTableFilterEntityNo());
		if (TRUE == Mask.m_bWantDeptFilter)		SetBatchDeptFilter(source.GetBatchDeptFilter());
		if (TRUE == Mask.m_bWantPluFilter)		SetBatchPluFilter(source.GetBatchPluFilter());
		if (TRUE == Mask.m_bWantSaleTime)			SetBatchSaleTimeFlag(source.GetBatchSaleTimeFlag());
		if (TRUE == Mask.m_bWantPreferConsolType)	SetBatchPreferConsolType(source.GetBatchPreferConsolType());
		if (TRUE == Mask.m_bWantJobSetConsolType)	SetBatchJobSetConsolType(source.GetBatchJobSetConsolType());
		if (TRUE == Mask.m_bWantReportEcr)		SetBatchReportEcrFlag(source.GetBatchReportEcrFlag());
		if (TRUE == Mask.m_bWantReportLoc)		SetBatchReportLocFlag(source.GetBatchReportLocFlag());
		if (TRUE == Mask.m_bWantReportAll)		SetBatchReportAllFlag(source.GetBatchReportAllFlag());
		if (TRUE == Mask.m_bWantReportCon)		SetBatchReportConFlag(source.GetBatchReportConFlag());
		if (TRUE == Mask.m_bWantItemServer)		SetBatchItemServerFlag(source.GetBatchItemServerFlag());
		if (TRUE == Mask.m_bWantTrainServer)		SetBatchTrainServerFlag(source.GetBatchTrainServerFlag());
		if (TRUE == Mask.m_bWantPMSType)			SetBatchPMSType(source.GetBatchPMSType());
		if (TRUE == Mask.m_bWantPMSPending)		SetBatchPMSPendingFlag(source.GetBatchPMSPendingFlag());
		if (TRUE == Mask.m_bWantPMSActive)		SetBatchPMSActiveFlag(source.GetBatchPMSActiveFlag());
		if (TRUE == Mask.m_bWantPMSComplete)		SetBatchPMSActiveFlag(source.GetBatchPMSCompleteFlag());
		if (TRUE == Mask.m_bWantPMSCancelled)		SetBatchPMSCancelledFlag(source.GetBatchPMSCancelledFlag());
		if (TRUE == Mask.m_bWantReportList)		SetBatchReportList(source.GetBatchReportList());
		if (TRUE == Mask.m_bWantReportTime)		SetBatchReportTimeFlag(source.GetBatchReportTimeFlag());
		if (TRUE == Mask.m_bWantDateRoundTo)		SetBatchDateRoundTo(source.GetBatchDateRoundTo());
		if (TRUE == Mask.m_bWantTimeSlice)		SetBatchTimeSlice(source.GetBatchTimeSlice());
		if (TRUE == Mask.m_bWantPluRange)			SetBatchPluRangeFlag(source.GetBatchPluRangeFlag());
		if (TRUE == Mask.m_bWantPluFrom)			SetBatchPluNoFrom(source.GetBatchPluNoFrom());
		if (TRUE == Mask.m_bWantPluTo)			SetBatchPluNoTo(source.GetBatchPluNoTo());
		if (TRUE == Mask.m_bWantDeptFilterType)	SetBatchDeptFilterType(source.GetBatchDeptFilterType());
		if (TRUE == Mask.m_bWantModifierFilter)	SetBatchModifierFilter(source.GetBatchModifierFilter());
		if (TRUE == Mask.m_bWantModifierCustom)	SetBatchModifierCustom(source.GetBatchModifierCustom());
		if (TRUE == Mask.m_bWantTaxBandFilter)	SetBatchTaxBandFilter(source.GetBatchTaxBandFilter());
		if (TRUE == Mask.m_bWantTaxBandCustom)	SetBatchTaxBandCustom(source.GetBatchTaxBandCustom());
		if (TRUE == Mask.m_bWantPriceLevelFilter)	SetBatchPriceLevelFilter(source.GetBatchPriceLevelFilter());
		if (TRUE == Mask.m_bWantPriceLevelCustom)	SetBatchPriceLevelCustom(source.GetBatchPriceLevelCustom());
		if (TRUE == Mask.m_bWantEposSales)		SetBatchEposSalesFlag(source.GetBatchEposSalesFlag());
		if (TRUE == Mask.m_bWantManualSales)		SetBatchManualSalesFlag(source.GetBatchManualSalesFlag());
		if (TRUE == Mask.m_bWantImportSales)		SetBatchImportSalesFlag(source.GetBatchImportSalesFlag());
		if (TRUE == Mask.m_bWantPMSTerminal)		SetBatchPMSTerminalFlag(source.GetBatchPMSTerminalFlag());
		if (TRUE == Mask.m_bWantMonthSort)		SetBatchMonthSortFlag(source.GetBatchMonthSortFlag());
		if (TRUE == Mask.m_bWantDeptSort)			SetBatchDeptSortFlag(source.GetBatchDeptSortFlag());
		if (TRUE == Mask.m_bWantSupplierSort)		SetBatchSupplierSortFlag(source.GetBatchSupplierSortFlag());
		if (TRUE == Mask.m_bWantSupplierFilter)	SetBatchSupplierFilter(source.GetBatchSupplierFilter());
		if (TRUE == Mask.m_bWantStockZeroSkip)	SetBatchStockZeroSkipFlag(source.GetBatchStockZeroSkipFlag());
		if (TRUE == Mask.m_bWantPercentage)		SetBatchPercentageFlag(source.GetBatchPercentageFlag());
		if (TRUE == Mask.m_bWantEstProfit)		SetBatchEstProfitFlag(source.GetBatchEstProfitFlag());
		if (TRUE == Mask.m_bWantCustomerOnly)		SetBatchCustomerOnlyFlag(source.GetBatchCustomerOnlyFlag());
		if (TRUE == Mask.m_bWantVIPOnly)			SetBatchVIPOnlyFlag(source.GetBatchVIPOnlyFlag());
		if (TRUE == Mask.m_bWantOfferFilter)		SetBatchOfferFilter(source.GetBatchOfferFilter());
		if (TRUE == Mask.m_bWantChartType)		SetBatchChartType(source.GetBatchChartType());
		if (TRUE == Mask.m_bWantChartBottom)		SetBatchChartBottom100Flag(source.GetBatchChartBottom100Flag());
		if (TRUE == Mask.m_bWantChartValue)		SetBatchChartByValueFlag(source.GetBatchChartByValueFlag());
		if (TRUE == Mask.m_bWantChartSize)		SetBatchChartSize(source.GetBatchChartSize());
		if (TRUE == Mask.m_bWantChartCutOff)		SetBatchChartCutOff(source.GetBatchChartCutOff());
		if (TRUE == Mask.m_bWantTaxDP)			SetBatchTaxDecimalPlaces(source.GetBatchTaxDecimalPlaces());

		if (TRUE == Mask.m_bWantSalesExportFields)
		{
			for (int n = 1; n <= m_BatchExportFieldSales.GetCount(); n++)
			{
				SetBatchExportFieldSales(n, source.GetBatchExportFieldSales(n));
			}
		}

		if (TRUE == Mask.m_bWantPaymentExportFields)
		{
			for (int n = 1; n <= m_BatchExportFieldPayment.GetCount(); n++)
			{
				SetBatchExportFieldPayment(n, source.GetBatchExportFieldPayment(n));
			}
		}

		if (TRUE == Mask.m_bWantVoidExportFields)
		{
			for (int n = 1; n <= m_BatchExportFieldVoid.GetCount(); n++)
			{
				SetBatchExportFieldVoid(n, source.GetBatchExportFieldVoid(n));
			}
		}

		if (TRUE == Mask.m_bWantDiscountExportFields)
		{
			for (int n = 1; n <= m_BatchExportFieldDiscount.GetCount(); n++)
			{
				SetBatchExportFieldDiscount(n, source.GetBatchExportFieldDiscount(n));
			}
		}

		if (TRUE == Mask.m_bWantTimeAttend)
		{
			SetBatchTAGroup(source.GetBatchTAGroup());
			SetBatchTAEdit(source.GetBatchTAEdit());
			SetBatchTATotalHours(source.GetBatchTATotalHours());
			SetBatchTAOpenShiftFlag(source.GetBatchTAOpenShiftFlag());
			SetBatchTAClosingTimeFlag(source.GetBatchTAClosingTimeFlag());
			SetBatchTAServerPickType(source.GetBatchTAServerPickType());
			SetBatchTAServerPickFrom(source.GetBatchTAServerPickFrom());
			SetBatchTAServerPickTo(source.GetBatchTAServerPickTo());
		}
	}
}

/**********************************************************************/

void CPosTrayTask::Reset()
{
	//TASK
	m_nTaskNo = 1;
	m_TaskName.Reset();
	m_TaskBatchNo.Reset();

	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		m_TaskEmailNickname[n].Reset();
	}

	m_TaskFormatEmail.Reset();
	m_TaskEmailByReport.Reset();
	m_TaskFormatExport.Reset();
	m_TaskExportFolder.Reset();
	m_TaskExportFilename.Reset();
	m_TaskExportNameFormat.Reset();
	m_TaskLocNameFlag.Reset();
	m_TaskFTPFlag.Reset();
	m_TaskFTPFolder.Reset();
	m_TaskExportDateType.Reset();
	m_TaskAutoPrint.Reset();
	m_TaskHourStart.Reset();
	m_TaskMinuteStart.Reset();
	m_TaskHourEnd.Reset();
	m_TaskMinuteEnd.Reset();
	m_TaskFrequency.Reset();
	m_TaskSun.Reset();
	m_TaskMon.Reset();
	m_TaskTue.Reset();
	m_TaskWed.Reset();
	m_TaskThu.Reset();
	m_TaskFri.Reset();
	m_TaskSat.Reset();
	m_TaskMonthDays.Reset();
	m_TaskBacklog.Reset();

	m_strTaskLastRunTime = "";

	//BATCH
	m_nBatchNo = 1;
	m_BatchName.Reset();
	m_BatchDateType.Reset();
	m_BatchSessionLegacy.Reset();
	m_BatchSessionEntity.Reset();
	m_BatchConType.Reset();
	m_BatchConDbNo.Reset();
	m_BatchConNwkNo.Reset();
	m_BatchConEntityNo.Reset();
	m_BatchConTermIdx.Reset();
	m_BatchTermDateType.Reset();
	m_BatchTableFilterType.Reset();
	m_BatchTableFilterLevel.Reset();
	m_BatchTableFilterEntityNo.Reset();
	m_BatchDeptFilter.Reset();
	m_BatchPluFilter.Reset();
	m_BatchSaleTime.Reset();
	m_BatchPreferConsolType.Reset();
	m_BatchJobSetConsolType.Reset();
	m_BatchReportEcr.Reset();
	m_BatchReportLoc.Reset();
	m_BatchReportAll.Reset();
	m_BatchReportCon.Reset();
	m_BatchItemServer.Reset();
	m_BatchTrainServer.Reset();
	m_BatchPMSType.Reset();
	m_BatchPMSPending.Reset();
	m_BatchPMSActive.Reset();
	m_BatchPMSComplete.Reset();
	m_BatchPMSCancelled.Reset();
	m_BatchReportList.Reset();
	m_BatchReportTime.Reset();

	//BATCH HISTORY
	m_BatchDateRoundTo.Reset();
	m_BatchTimeSlice.Reset();
	m_BatchPluRange.Reset();
	m_BatchPluFrom.Reset();
	m_BatchPluTo.Reset();
	m_BatchDeptFilterType.Reset();
	m_BatchModifierFilter.Reset();
	m_BatchModifierCustom.Reset();
	m_BatchTaxBandFilter.Reset();
	m_BatchTaxBandCustom.Reset();
	m_BatchPriceLevelFilter.Reset();
	m_BatchPriceLevelCustom.Reset();
	m_BatchEposSales.Reset();
	m_BatchManualSales.Reset();
	m_BatchImportSales.Reset();
	m_BatchPMSTerminal.Reset();
	m_BatchMonthSort.Reset();
	m_BatchDeptSort.Reset();
	m_BatchSupplierSort.Reset();
	m_BatchSupplierFilter.Reset();
	m_BatchStockZeroSkip.Reset();
	m_BatchPercentage.Reset();
	m_BatchEstProfit.Reset();
	m_BatchCustomerOnly.Reset();
	m_BatchVIPOnly.Reset();
	m_BatchChartType.Reset();
	m_BatchChartBottom.Reset();
	m_BatchChartValue.Reset();
	m_BatchChartSize.Reset();
	m_BatchChartCutOff.Reset();
	m_BatchTaxDP.Reset();

	//BATCH TIME ATTEND
	m_BatchTAEdit.Reset();
	m_BatchTAGroup.Reset();
	m_BatchTATotalHours.Reset();
	m_BatchTAOpenShift.Reset();
	m_BatchTAClosingTime.Reset();
	m_BatchTAServerPickType.Reset();
	m_BatchTAServerPickFrom.Reset();
	m_BatchTAServerPickTo.Reset();

	//BATCH CSV
	for (int n = 0; n < m_BatchExportFieldSales.GetCount(); n++)
	{
		m_BatchExportFieldSales.Reset(n);
	}

	//BATCH CSV
	for (int n = 0; n < m_BatchExportFieldPayment.GetCount(); n++)
	{
		m_BatchExportFieldPayment.Reset(n);
	}

	//BATCH CSV
	for (int n = 0; n < m_BatchExportFieldVoid.GetCount(); n++)
	{
		m_BatchExportFieldVoid.Reset(n);
	}

	//BATCH CSV
	for (int n = 0; n < m_BatchExportFieldDiscount.GetCount(); n++)
	{
		m_BatchExportFieldDiscount.Reset(n);
	}

	//ALL
	m_MergeDelete.Reset();
}

/**********************************************************************/

void CPosTrayTask::SetTaskNo(int n)
{
	if ((n >= 1) && (n <= POSTRAY_MAX_TASK))
	{
		m_nTaskNo = n;
	}
}

/**********************************************************************/

int CPosTrayTask::GetBatchExportFieldSales( int n )
{
	if ((n >= 1) && (n <= m_BatchExportFieldSales.GetCount()))
	{
		return m_BatchExportFieldSales.GetValue(n - 1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchExportFieldSales(int n, int nType)
{
	if ((n >= 1) && (n <= m_BatchExportFieldSales.GetCount()))
	{
		m_BatchExportFieldSales.SetValue(n - 1, nType);
	}
}

/**********************************************************************/

int CPosTrayTask::GetBatchExportFieldPayment( int n )
{
	if ((n >= 1) && (n <= m_BatchExportFieldPayment.GetCount()))
	{
		return m_BatchExportFieldPayment.GetValue(n - 1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchExportFieldPayment( int n, int nType )
{
	if ((n >= 1) && (n <= m_BatchExportFieldPayment.GetCount()))
	{
		m_BatchExportFieldPayment.SetValue(n - 1, nType);
	}
}

/**********************************************************************/

int CPosTrayTask::GetBatchExportFieldVoid( int n )
{
	if ((n >= 1) && (n <= m_BatchExportFieldVoid.GetCount()))
	{
		return m_BatchExportFieldVoid.GetValue(n - 1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchExportFieldVoid( int n, int nType )
{
	if ((n >= 1) && (n <= m_BatchExportFieldVoid.GetCount()))
	{
		m_BatchExportFieldVoid.SetValue(n - 1, nType);
	}
}

/**********************************************************************/

int CPosTrayTask::GetBatchExportFieldDiscount(int n)
{
	if ((n >= 1) && (n <= m_BatchExportFieldDiscount.GetCount()))
	{
		return m_BatchExportFieldDiscount.GetValue(n - 1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchExportFieldDiscount( int n, int nType )
{
	if ((n >= 1) && (n <= m_BatchExportFieldDiscount.GetCount()))
	{
		m_BatchExportFieldDiscount.SetValue(n - 1, nType);
	}
}

/**********************************************************************/

void CPosTrayTask::SetLastRunTime( COleDateTime& time, bool bTemp )
{
	if ( time.m_status == COleDateTime::valid )
	{
		CString strTime;
		strTime.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d",
			time.GetYear(),
			time.GetMonth(),
			time.GetDay(),
			time.GetHour(),
			time.GetMinute() );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( bTemp );
		if (fileDate.Open(strFilename, "wb") == TRUE)
		{
			fileDate.WriteLine(strTime);
		}
	}
}

/**********************************************************************/

void CPosTrayTask::SetLastRunTime( const char* sz, bool bTemp )
{
	CString str = sz;

	bool bValid = FALSE;

	if (str == "")
	{
		bValid = TRUE;
	}
	else if ((str.GetLength() == 12) && (::TestNumeric(str) == TRUE))
	{
		bValid = TRUE;
	}

	if ( TRUE == bValid )
	{
		CSSFile fileDate;
		CString strFilename = GetDateFilename( bTemp );
		if (fileDate.Open(strFilename, "wb") == TRUE)
		{
			fileDate.WriteLine(str);
		}
	}
}

/**********************************************************************/

const char* CPosTrayTask::GetLastRunTime( bool bTemp )
{
	m_strTaskLastRunTime = "";

	CSSFile fileDate;
	CString strFilename = GetDateFilename( bTemp );
	if ( fileDate.Open( strFilename, "rb" ) == TRUE )
	{
		CString strDate;
		fileDate.ReadString( strDate );

		bool bValid = FALSE;

		if (strDate == "")
		{
			bValid = TRUE;
		}
		else
		{
			if ( strDate.GetLength() == 8 )
			{
				CString strExtra;
				strExtra.Format( "%2.2d%2.2d",
					m_TaskHourStart.GetValue(),
					m_TaskMinuteStart.GetValue() );

				strDate += strExtra;
			}

			if (strDate.GetLength() == 12)
			{
				bValid = ::TestNumeric(strDate);
			}
		}
		
		if (TRUE == bValid)
		{
			m_strTaskLastRunTime = strDate;
		}
	}

	return m_strTaskLastRunTime;
}

/**********************************************************************/

int CPosTrayTask::GetMergedTaskType()
{
	int nType = 0;
	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_EPOS_ALL:
		nType = 1;
		break;

	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_HIST_ALL:
		nType = 2;
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		nType = 3;
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		nType = 4;
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		nType = 5;
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		nType = 6;
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
		nType = ( SysInfo.IsStockSystem() ) ? 8 : 7;
		break;

	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		nType = 9;
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
		nType = 10;
		break;
	}

	return nType;
}

/**********************************************************************/

void CPosTrayTask::SetBatchNo( int n )
{
	if ((n >= 1) && (n <= POSTRAY_MAX_BATCH))
	{
		m_nBatchNo = n;
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchModifierFilter( int n )
{
	if ((n >= 0) && (n <= 10))
	{
		m_BatchModifierFilter.SetValue(n);
	}
	else if (999 == n)
	{
		m_BatchModifierFilter.SetValue(n);
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchTaxBandFilter( int n )
{
	if ((n >= 0) && (n <= 7))
	{
		m_BatchTaxBandFilter.SetValue(n);
	}
	else if (999 == n)
	{
		m_BatchTaxBandFilter.SetValue(n);
	}
}

/**********************************************************************/

void CPosTrayTask::SetBatchPriceLevelFilter( int n )
{
	if ((n >= 0) && (n <= 10))
	{
		m_BatchPriceLevelFilter.SetValue(n);
	}
	else if (999 == n)
	{
		m_BatchPriceLevelFilter.SetValue(n);
	}
}

/**********************************************************************/

bool CPosTrayTask::Read( bool bTemp )
{
	CString strTaskName;
	strTaskName.Format( "Task %d", m_nTaskNo );
	m_TaskName.Create ( "TaskName", POSTRAY_MAXLENGTH_TASK_NAME, strTaskName );

	CString strBatchName;
	strBatchName.Format( "Batch %d", m_nBatchNo );
	m_BatchName.Create ( "BatchName", POSTRAY_MAXLENGTH_BATCH_NAME, strBatchName );

	CString strFilename = GetFilename( bTemp );
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CPosTrayTask::Write( bool bTemp )
{
	CEnhancedIniFile iniFile;
	PrepareIniFile( iniFile );
	return iniFile.Write( GetFilename( bTemp ) );
}

/**********************************************************************/

const char* CPosTrayTask::GetLiveTaskFolder()
{
	m_strLiveTaskFolder = "PosTray";
	::GetSyssetProgramPath( m_strLiveTaskFolder );
	::CreateSubdirectory( m_strLiveTaskFolder );

	{
		CString strExtra;
		strExtra.Format( "\\Tasks%3.3d", GetMergedTaskType() );
		m_strLiveTaskFolder += strExtra;
		::CreateSubdirectory( m_strLiveTaskFolder );
	}

	{
		CString strExtra;
		strExtra.Format( "\\%4.4d", m_nTaskNo );
		m_strLiveTaskFolder += strExtra;
		::CreateSubdirectory( m_strLiveTaskFolder );
	}

	return m_strLiveTaskFolder;
}

/**********************************************************************/

const char* CPosTrayTask::GetFilename( bool bTemp )
{
	m_strFilename = "";

	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_EPOS_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		{
			CFilenameUpdater FnUp( SysFiles::PosTrayLiveTask );
			m_strFilename += GetLiveTaskFolder();
			m_strFilename += "\\";
			m_strFilename += FnUp.GetFilenameToUse();
		}
		break;
	}

	if ( m_strFilename == "" )
	{
		SysFileInfo fileType;

		switch (m_nMode)
		{
		case POSTRAY_TASKFILE_HIST_TASK:
			fileType = (bTemp) ? SysFiles::PosTrayTaskHistTemp : SysFiles::PosTrayTaskHist;
			break;

		case POSTRAY_TASKFILE_HIST_BATCH:
			fileType = (bTemp) ? SysFiles::PosTrayBatchHistTemp : SysFiles::PosTrayBatchHist;
			break;

		case POSTRAY_TASKFILE_STKLEVEL_TASK:
			if (SysInfo.IsStockSystem() == TRUE)
			{
				fileType = (bTemp) ? SysFiles::PosTrayTaskStkLevelStkTemp : SysFiles::PosTrayTaskStkLevelStk;
			}
			else
			{
				fileType = (bTemp) ? SysFiles::PosTrayTaskStkLevelEcrTemp : SysFiles::PosTrayTaskStkLevelEcr;
			}
			break;

		case POSTRAY_TASKFILE_STKLEVEL_BATCH:	
			if (SysInfo.IsStockSystem() == TRUE)
			{
				fileType = (bTemp) ? SysFiles::PosTrayBatchStkLevelStkTemp : SysFiles::PosTrayBatchStkLevelStk;
			}
			else
			{
				fileType = (bTemp) ? SysFiles::PosTrayBatchStkLevelEcrTemp : SysFiles::PosTrayBatchStkLevelEcr;
			}
			break;

		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:	
			fileType = (bTemp) ? SysFiles::PosTrayTaskExportSalesTemp : SysFiles::PosTrayTaskExportSales;		
			break;

		case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:	
			fileType = (bTemp) ? SysFiles::PosTrayBatchExportSalesTemp : SysFiles::PosTrayBatchExportSales;		
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:	
			fileType = (bTemp) ? SysFiles::PosTrayTaskExportPaymentTemp : SysFiles::PosTrayTaskExportPayment;		
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:	
			fileType = (bTemp) ? SysFiles::PosTrayBatchExportPaymentTemp : SysFiles::PosTrayBatchExportPayment;		
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:	
			fileType = (bTemp) ? SysFiles::PosTrayTaskExportVoidTemp : SysFiles::PosTrayTaskExportVoid;		
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:	
			fileType = (bTemp) ? SysFiles::PosTrayBatchExportVoidTemp : SysFiles::PosTrayBatchExportVoid;		
			break;

		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:	
			fileType = (bTemp) ? SysFiles::PosTrayTaskExportDiscountTemp : SysFiles::PosTrayTaskExportDiscount;		
			break;

		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:	
			fileType = (bTemp) ? SysFiles::PosTrayBatchExportDiscountTemp : SysFiles::PosTrayBatchExportDiscount;		
			break;

		case POSTRAY_TASKFILE_EPOS_TASK:	
			fileType = (bTemp) ? SysFiles::PosTrayTaskEposTemp : SysFiles::PosTrayTaskEpos;		
			break;

		case POSTRAY_TASKFILE_EPOS_BATCH:	
			fileType = (bTemp) ? SysFiles::PosTrayBatchEposTemp : SysFiles::PosTrayBatchEpos;		
			break;

		case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
			fileType = (bTemp) ? SysFiles::PosTrayTaskProcessTranTemp : SysFiles::PosTrayTaskProcessTran;
			break;

		case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
			fileType = (bTemp) ? SysFiles::PosTrayBatchProcessTranTemp : SysFiles::PosTrayBatchProcessTran;
			break;

		case POSTRAY_TASKFILE_TIMEATTEND_TASK:
			fileType = (bTemp) ? SysFiles::PosTrayTaskTimeAttendTemp : SysFiles::PosTrayTaskTimeAttend;
			break;

		case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
			fileType = (bTemp) ? SysFiles::PosTrayBatchTimeAttendTemp : SysFiles::PosTrayBatchTimeAttend;
			break;
		}

		int nFileNo = 0;
		switch( m_nMode )
		{
		case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
		case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
		case POSTRAY_TASKFILE_HIST_BATCH:	
		case POSTRAY_TASKFILE_EPOS_BATCH:
		case POSTRAY_TASKFILE_STKLEVEL_BATCH:
		case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
		case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
		case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
			nFileNo = m_nBatchNo;
			break;
		
		case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
		case POSTRAY_TASKFILE_TIMEATTEND_TASK:
		case POSTRAY_TASKFILE_HIST_TASK:	
		case POSTRAY_TASKFILE_EPOS_TASK:
		case POSTRAY_TASKFILE_STKLEVEL_TASK:
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:	
		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		default:
			nFileNo = m_nTaskNo;
			break;
		}

		CFilenameUpdater FnUp( fileType, 0, nFileNo );
		m_strFilename = FnUp.GetFilenameToUse();
	}
	return m_strFilename;
}

/**********************************************************************/

const char* CPosTrayTask::GetDateFilename( bool bTemp )
{
	m_strDateFilename = GetLiveTaskFolder();	
	m_strDateFilename += "\\";

	if ( FALSE == bTemp )
	{
		CFilenameUpdater FnUp( SysFiles::PosTrayLiveTaskDate );
		m_strDateFilename += FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::PosTrayLiveTaskDateTemp );
		m_strDateFilename += FnUp.GetFilenameToUse();
	}
		
	return m_strDateFilename;
}

/**********************************************************************/

bool CPosTrayTask::GetNextRunTime( COleDateTime& oleTimeNextRun, CString& strTimeLastRun )
{
	COleDateTime oleTimeNow = COleDateTime::GetCurrentTime();
	return GetNextRunTime( oleTimeNextRun, strTimeLastRun, oleTimeNow );
}

/**********************************************************************/

bool CPosTrayTask::GetNextRunTime(COleDateTime& oleTimeNextRun, CString& strTimeLastRun, COleDateTime& oleTimeNow)
{
	// No month days set, so no next run time. Should be impossible.
	if ((GetTaskMonthDays() & GetAllMonthDaysMask()) == 0)
	{
		return FALSE;
	}

	COleDateTime oleTimeNowNoSeconds = COleDateTime(
		oleTimeNow.GetYear(),
		oleTimeNow.GetMonth(),
		oleTimeNow.GetDay(),
		oleTimeNow.GetHour(),
		oleTimeNow.GetMinute(),
		0);

	COleDateTime oleTimeLastRun;
	{
		bool bGotTimeLastRun = FALSE;
		if ((strTimeLastRun.GetLength() == 12) && (::TestNumeric(strTimeLastRun) == TRUE))
		{
			oleTimeLastRun = COleDateTime(
				atoi(strTimeLastRun.Left(4)),
				atoi(strTimeLastRun.Mid(4, 2)),
				atoi(strTimeLastRun.Mid(6, 2)),
				atoi(strTimeLastRun.Mid(8, 2)),
				atoi(strTimeLastRun.Right(2)),
				0);

			bGotTimeLastRun = (oleTimeLastRun.m_status == COleDateTime::valid);
		}

		if (FALSE == bGotTimeLastRun)
		{
			oleTimeLastRun = oleTimeNowNoSeconds;
		}
	}

	if ((GetTaskBacklogFlag() == FALSE) || (IsExportTask() == TRUE) || (IsProcessTranTask() == TRUE))
	{
		COleDateTime timeTemp = oleTimeNowNoSeconds;
		timeTemp -= COleDateTimeSpan(0, 0, 1, 0);

		if (timeTemp > oleTimeLastRun)
		{
			oleTimeLastRun = timeTemp;
		}
	}

	bool bGotNextRun = FALSE;

	while (TRUE)
	{
		CPosTrayTaskIterator Iterator(*this);
		if (Iterator.GetNextTaskRunTime(oleTimeLastRun, oleTimeNowNoSeconds, oleTimeNextRun) == FALSE)
		{
			return FALSE;
		}

		int nMaxMonthDay = 31;

		switch (oleTimeNextRun.GetMonth())
		{
		case 4:
		case 6:
		case 9:
		case 11:
			nMaxMonthDay = 30;
			break;

		case 2:
		{
			int nYear = oleTimeNextRun.GetYear();

			if (((nYear % 4) == 0) && ((nYear % 100) != 0) || ((nYear % 400) == 0))
			{
				nMaxMonthDay = 29;
			}
			else
			{
				nMaxMonthDay = 28;
			}
		}
		}

		int nDayOfMonth = oleTimeNextRun.GetDay();	
		bool bDayOfMonthOK = GetTaskMonthDayFlag(nDayOfMonth);

		if ( (FALSE == bDayOfMonthOK) && ( nDayOfMonth == nMaxMonthDay ) )
		{
			for (int n = nDayOfMonth + 1; n <= 31; n++)
			{
				if (GetTaskMonthDayFlag(n) == TRUE)
				{
					bDayOfMonthOK = TRUE;
					break;
				}
			}
		}

		if (FALSE == bDayOfMonthOK)
		{
			oleTimeLastRun = oleTimeNextRun;
			oleTimeNowNoSeconds = oleTimeLastRun + COleDateTimeSpan(0, 0, 1, 0);

			oleTimeNowNoSeconds = COleDateTime(
				oleTimeNowNoSeconds.GetYear(),
				oleTimeNowNoSeconds.GetMonth(),
				oleTimeNowNoSeconds.GetDay(),
				oleTimeNowNoSeconds.GetHour(),
				oleTimeNowNoSeconds.GetMinute(),
				0);
		}
		else
		{
			return TRUE;
		}
	}
}

/**********************************************************************/

CString CPosTrayTask::GetMonthDaysDisplayString()
{
	CString strResult = "";
    
    if (GetTaskMonthDays() == GetAllMonthDaysMask())
	{
		strResult = "All";
	}
	else
	{
		CCSV csvResult;
		CString strAltResult = "";

		for (int n = 1; n <= 31; n++)
		{
			if (GetTaskMonthDayFlag(n) == TRUE)
			{
				csvResult.Add(n);
				strAltResult += "O"; // selected
			}
			else
			{
				strAltResult += "x"; // not selected
			}

			if ((n % 7) == 0)
			{
				strAltResult += "  ";
			}
		}

		strResult = csvResult.GetLine();
		if (strResult.GetLength() > 40)
		{
			strResult = strAltResult;
		}
	}

	return strResult;
}

/**********************************************************************/

int CPosTrayTask::ValidateTask()
{
	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_EXPORT_SALES_BATCH:
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_BATCH:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		return EMAILTASK_VALIDATE_WRONGTYPE;
		break;

	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	case POSTRAY_TASKFILE_EPOS_ALL:
	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
	default:
		break;
	}

	switch (m_nMode)
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		break;

	default:
		if (m_TaskBatchNo.GetValue() == 0)
		{
			return EMAILTASK_VALIDATE_NOBATCH;
		}
		break;
	}

	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_EPOS_ALL:
	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
		{
			CString strReports = m_BatchReportList.GetValue();
			if (strReports == "")
			{
				return EMAILTASK_VALIDATE_NOJOBS;
			}
		}
		break;
	}

	switch (m_nMode)
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_BATCH:
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		break;

	default:
	{
		int nEmail = GetTaskFormatEmail();
		bool bEmail = (0 == nEmail) || (1 == nEmail);

		int nExport = GetTaskFormatExport();
		bool bExport = (0 == nExport) || (1 == nExport);

		if ((FALSE == bEmail) && (FALSE == bExport) && (FALSE == GetTaskAutoPrintFlag()))
		{
			return EMAILTASK_VALIDATE_NOACTION;
		}

		/*
		if (TRUE == bEmail)
		{
			CString strEmail = m_TaskEmail.GetValue();
			if (strEmail == "")
			{
				return EMAILTASK_VALIDATE_NOEMAIL;
			}
		}
		*/
	}
	break;
	}

	bool bGotWeekday = FALSE;
	bGotWeekday |= m_TaskSun.GetValue();
	bGotWeekday |= m_TaskMon.GetValue();
	bGotWeekday |= m_TaskTue.GetValue();
	bGotWeekday |= m_TaskWed.GetValue();
	bGotWeekday |= m_TaskThu.GetValue();
	bGotWeekday |= m_TaskFri.GetValue();
	bGotWeekday |= m_TaskSat.GetValue();

	if (FALSE == bGotWeekday)
	{
		return EMAILTASK_VALIDATE_NODAYS;
	}

	switch( m_nMode )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	case POSTRAY_TASKFILE_HIST_ALL:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		break;

	case POSTRAY_TASKFILE_EPOS_ALL:
	default:
		if ((m_BatchReportEcr.GetValue() || m_BatchReportLoc.GetValue() || m_BatchReportAll.GetValue() || m_BatchReportCon.GetValue()) == FALSE)
		{
			return EMAILTASK_VALIDATE_NOCONTYPE;
		}
		break;
	}

	return EMAILTASK_VALIDATE_OK;
}

/**********************************************************************/

const char* CPosTrayTask::GetChartText()
{
	CString strTopBottom = ( m_BatchChartBottom.GetValue() == TRUE ) ? "Bottom" : "Top";
	CString strSize = "";
	CString strQtyValue = ( m_BatchChartValue.GetValue() == TRUE ) ? "Value" : "Qty";
	CString strCutOff = "";

	switch( m_BatchChartType.GetValue() )
	{
	case 1:
		{
			CCSV csv( ',', '"', FALSE, FALSE, TRUE );
			csv.Add( m_BatchChartCutOff.GetValue(), 5 );			
			strCutOff.Format( ", cut off at %s", csv.GetString(0) );
		}
		break;

	case 2:
		strSize.Format( " %d", m_BatchChartSize.GetValue() );
		break;

	case 0:
	default:
		break;
	}

	m_strChartText = "";
	m_strChartText += strTopBottom;
	m_strChartText += strSize;
	m_strChartText += " by ";
	m_strChartText += strQtyValue;
	m_strChartText += strCutOff;

	return m_strChartText;
}

/**********************************************************************/

void CPosTrayTask::GetExportFieldList( CIniFileIntArray& arrayIniFile, CArray<int,int>& arrayFields )
{
	arrayFields.RemoveAll();
	CReportConsolidationArray<CSortedIntItem> arrayFieldsInUse;

	for ( int nFieldIdx = 0; nFieldIdx < arrayIniFile.GetCount(); nFieldIdx++ )
	{
		int nFieldType = arrayIniFile.GetValue(nFieldIdx);

		if ( nFieldType != 0 )
		{
			CSortedIntItem item;
			item.m_nItem = nFieldType;
			
			int nPos;
			if ( arrayFieldsInUse.Find( item, nPos ) == FALSE )
			{
				arrayFieldsInUse.InsertAt( nPos, item );
				arrayFields.Add( nFieldType );
			}
		}
	}
}

/**********************************************************************/

void CPosTrayTask::GetExportFieldListSales( CArray<int,int>& arrayFields )
{
	GetExportFieldList( m_BatchExportFieldSales, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::GetExportFieldListPayment( CArray<int,int>& arrayFields )
{
	GetExportFieldList( m_BatchExportFieldPayment, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::GetExportFieldListVoid( CArray<int,int>& arrayFields )
{
	GetExportFieldList( m_BatchExportFieldVoid, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::GetExportFieldListDiscount( CArray<int,int>& arrayFields )
{
	GetExportFieldList( m_BatchExportFieldDiscount, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::SetExportFieldList( CIniFileIntArray& arrayIniFile, CArray<int,int>& arrayFields )
{
	CReportConsolidationArray<CSortedIntItem> arrayFieldsInUse;

	int nFieldPos = 0;
	for ( int nFieldIdx = 0; ( nFieldIdx < arrayIniFile.GetCount() ) && ( nFieldIdx < arrayFields.GetSize() ); nFieldIdx++ )
	{
		int nFieldType = arrayFields.GetAt( nFieldIdx );

		if ( nFieldType != 0 )
		{
			CSortedIntItem item;
			item.m_nItem = nFieldType;
			
			int nPos;
			if ( arrayFieldsInUse.Find( item, nPos ) == FALSE )
			{
				arrayFieldsInUse.InsertAt( nPos, item );
				arrayIniFile.SetValue( nFieldPos++, nFieldType );
			}
		}
	}

	for (; nFieldPos < arrayIniFile.GetCount(); nFieldPos++)
	{
		arrayIniFile.Reset(nFieldPos);
	}
}

/**********************************************************************/

void CPosTrayTask::SetExportFieldListSales( CArray<int,int>& arrayFields )
{
	SetExportFieldList( m_BatchExportFieldSales, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::SetExportFieldListPayment( CArray<int,int>& arrayFields )
{
	SetExportFieldList( m_BatchExportFieldPayment, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::SetExportFieldListDiscount( CArray<int,int>& arrayFields )
{
	SetExportFieldList( m_BatchExportFieldDiscount, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::SetExportFieldListVoid( CArray<int,int>& arrayFields )
{
	SetExportFieldList( m_BatchExportFieldVoid, arrayFields );
}

/**********************************************************************/

void CPosTrayTask::TidyExportFieldList( CIniFileIntArray& arrayIniFile, CArray<int,int>& arrayFields )
{
	CReportConsolidationArray<CPosTrayTaskSortInfo> arraySort;

	for ( int nFieldIdx = 0; ( nFieldIdx < arrayFields.GetSize() ) && ( nFieldIdx < arrayIniFile.GetCount() ); nFieldIdx++ )
	{
		int nFieldType = arrayFields.GetAt( nFieldIdx );

		switch( nFieldType  )
		{
		case POSTRAY_EXPORTFIELD_QTY_BASE:
		case POSTRAY_EXPORTFIELD_QTY_MOD:
		case POSTRAY_EXPORTFIELD_VALUE_FULL:
		case POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM:
		case POSTRAY_EXPORTFIELD_DISCOUNTONLY:
		case POSTRAY_EXPORTFIELD_PREMIUMONLY:
		case POSTRAY_EXPORTFIELD_VALUE_DISC:
		case POSTRAY_EXPORTFIELD_TAX:
		case POSTRAY_EXPORTFIELD_VALUE_NET:
		case POSTRAY_EXPORTFIELD_COST:
		case POSTRAY_EXPORTFIELD_WASTE_QTY_BASE:
		case POSTRAY_EXPORTFIELD_WASTE_QTY_MOD:
		case POSTRAY_EXPORTFIELD_WASTE_COST:
		case POSTRAY_EXPORTFIELD_VALUE_TENDER:
		case POSTRAY_EXPORTFIELD_VALUE_AMOUNT:
		case POSTRAY_EXPORTFIELD_VALUE_GRATUITY:
		case POSTRAY_EXPORTFIELD_VALUE_CASHBACK:
		case POSTRAY_EXPORTFIELD_VALUE_SURPLUS:
		case POSTRAY_EXPORTFIELD_LINE_COUNT:
		case POSTRAY_EXPORTFIELD_VALUE_VOID:
			{
				CPosTrayTaskSortInfo info;
				info.m_nFieldGroupForSort = 1;
				info.m_nFieldTypeForSort = nFieldType;
				info.m_nFieldIdxForSort = nFieldIdx;
				info.m_nFieldTypeForList = nFieldType;
				arraySort.Consolidate( info );
			}
			break;

		case 0:
			break;

		default:
			{
				CPosTrayTaskSortInfo info;
				info.m_nFieldGroupForSort = 0;
				info.m_nFieldTypeForSort = 0;
				info.m_nFieldIdxForSort = nFieldIdx;
				info.m_nFieldTypeForList = nFieldType;
				arraySort.Consolidate( info );
			}
			break;
		}
	}

	arrayFields.RemoveAll();
	for ( int nFieldIdx = 0; nFieldIdx < arraySort.GetSize(); nFieldIdx++ )
	{
		CPosTrayTaskSortInfo info;
		arraySort.GetAt( nFieldIdx, info );
		arrayFields.Add( info.m_nFieldTypeForList );
	}
}

/**********************************************************************/

void CPosTrayTask::TidyExportFieldListSales(CArray<int, int>& arrayFields)
{
	TidyExportFieldList(m_BatchExportFieldSales, arrayFields);
}

/**********************************************************************/

void CPosTrayTask::TidyExportFieldListPayment(CArray<int, int>& arrayFields)
{
	TidyExportFieldList(m_BatchExportFieldPayment, arrayFields);
}

/**********************************************************************/

void CPosTrayTask::TidyExportFieldListVoid(CArray<int, int>& arrayFields)
{
	TidyExportFieldList(m_BatchExportFieldVoid, arrayFields);
}

/**********************************************************************/

void CPosTrayTask::TidyExportFieldListDiscount(CArray<int, int>& arrayFields)
{
	TidyExportFieldList(m_BatchExportFieldDiscount, arrayFields);
}

/**********************************************************************/

const char* CPosTrayTask::GetExportFilenameSuffix(COleDateTime& oleDateTime, CPosTrayTaskConsolInfo& infoQuick)
{
	CString strTaskNo = "";
	CString strDate = "";
	CString strTime = "";

	switch (GetTaskExportFilenameFormat())
	{
	case 3:
	case 4:
	case 5:
		strTaskNo.Format("_%2.2d", GetTaskNo());
		break;
	}

	switch (GetTaskExportFilenameFormat())
	{
	case 1:
	case 2:
	case 4:
	case 5:
		strDate.Format("_%4.4d%2.2d%2.2d",
			oleDateTime.GetYear(),
			oleDateTime.GetMonth(),
			oleDateTime.GetDay());
		break;
	}

	switch (GetTaskExportFilenameFormat())
	{
	case 2:
	case 5:
		strTime.Format("_%2.2d%2.2d",
			oleDateTime.GetHour(),
			oleDateTime.GetMinute());
		break;
	}

	CString strQuick = "";

	if (GetTaskLocNameFlag() == FALSE)
	{
		switch (infoQuick.m_nQuickType)
		{
		case 1:
			strQuick.Format("_1%3.3d%3.3d%3.3d%3.3d", infoQuick.m_nDbNo, infoQuick.m_nNwkNo, infoQuick.m_nLocNo, infoQuick.m_nTermNo);
			break;

		case 2:
			strQuick.Format("_2%3.3d%3.3d%3.3d", infoQuick.m_nDbNo, infoQuick.m_nNwkNo, infoQuick.m_nLocNo);
			break;

		case 3:
			strQuick.Format("_3%3.3d", infoQuick.m_nDbNo);
			break;
		}
	}
	else
	{
		CString strTemp = "";
		switch (infoQuick.m_nQuickType)
		{
		case 1:
		{
			int nLocIdx = 0;
			if (dbLocation.FindLocationByNumber(infoQuick.m_nNwkNo, infoQuick.m_nLocNo, nLocIdx) == TRUE)
			{
				strTemp += "_";
				strTemp += dbLocation.GetName(nLocIdx);
				strTemp += "_";

				int nTermIdx = 0;
				if (dbLocation.FindTerminalByNumber(nLocIdx, infoQuick.m_nTermNo, nTermIdx) == TRUE)
				{
					strTemp += dbLocation.GetTerminalName(nLocIdx, nTermIdx);
				}
				else
				{
					CString strExtra = "";
					strExtra.Format("Terminal%d", infoQuick.m_nTermNo);
					strTemp += strExtra;
				}
			}
			else
			{
				strTemp.Format("_Location%d.%d_Terminal%d", infoQuick.m_nNwkNo, infoQuick.m_nLocNo, infoQuick.m_nTermNo);
			}
		}
		break;

		case 2:
		{
			int nLocIdx = 0;
			if (dbLocation.FindLocationByNumber(infoQuick.m_nNwkNo, infoQuick.m_nLocNo, nLocIdx) == TRUE)
			{
				strTemp += "_";
				strTemp += dbLocation.GetName(nLocIdx);
			}
			else
			{
				strTemp.Format("_Location%d.%d", infoQuick.m_nNwkNo, infoQuick.m_nLocNo );
			}
		}
		break;

		case 3:
		{
			int nDbIdx = 0;
			if (dbDatabase.FindDatabaseByNumber(infoQuick.m_nDbNo, nDbIdx) == TRUE)
			{
				strTemp += "_";
				strTemp += dbDatabase.GetName(nDbIdx);
			}
			else
			{
				strTemp.Format("_Database%d", infoQuick.m_nDbNo);
			}
		}
		break;

		}

		strQuick = "";
		bool bLastCharWasSpace = FALSE;
		for (int n = 0; n < strTemp.GetLength(); n++)
		{
			char c = strTemp.GetAt(n);

			if (c == ' ')
			{
				bLastCharWasSpace = TRUE;
			}
			else
			{
				if (TRUE == bLastCharWasSpace)
				{
					if (strQuick != "")
					{
						strQuick += "-";
					}
				}

				strQuick += c;
				bLastCharWasSpace = FALSE;
			}
		}
	}

	m_strExportFilenameSuffix = strQuick + strTaskNo + strDate + strTime;
	
	return m_strExportFilenameSuffix;
}

/**********************************************************************/

const char* CPosTrayTask::GetExportFilenameSuffix( COleDateTime& oleDateTime )
{
	CString strTaskNo = "";
	CString strDate = "";
	CString strTime = "";

	switch( GetTaskExportFilenameFormat() )
	{
	case 3:
	case 4:
	case 5:
		strTaskNo.Format( "_%2.2d", GetTaskNo() );
		break;
	}

	switch( GetTaskExportFilenameFormat() )
	{
	case 1:
	case 2:
	case 4:
	case 5:
		strDate.Format( "_%4.4d%2.2d%2.2d",
			oleDateTime.GetYear(),
			oleDateTime.GetMonth(),
			oleDateTime.GetDay() );
		break;
	}

	switch( GetTaskExportFilenameFormat() )
	{
	case 2:
	case 5:
		strTime.Format( "_%2.2d%2.2d",
			oleDateTime.GetHour(),
			oleDateTime.GetMinute() );
		break;
	}

	m_strExportFilenameSuffix = strTaskNo + strDate + strTime;
	
	return m_strExportFilenameSuffix;
}

/**********************************************************************/

const char* CPosTrayTask::GetExportFilename( const char* szType, CString& strFixedDateSuffix, COleDateTime& dateTask )
{
	m_strExportFilename = "\\";
	
	{
		CString strFilename = GetTaskExportFilename();
		::TrimSpaces( strFilename, FALSE );
		
		if ( strFilename == "" )
			strFilename = szType;
		else
		{
			strFilename.MakeLower();
			
			int nLen = strFilename.GetLength(); 
			
			if ( nLen >= 4 )
				if ( strFilename.Right(4) == ".csv" )
					strFilename = strFilename.Left( nLen - 4 );
		}

		if ( strFixedDateSuffix != "" )
		{
			m_strExportFilename += strFilename;
			m_strExportFilename += strFixedDateSuffix;
		}
		else
		{
			m_strExportFilename += strFilename;
			m_strExportFilename += GetExportFilenameSuffix( dateTask );
		}

		m_strExportFilename += ".csv";
	}

	return m_strExportFilename;
}

/**********************************************************************/

int CPosTrayTask::GetNextManualExportNo()
{
	int nManualExportNo = 1;
	CFilenameUpdater FnUp( SysFiles::PosTrayManualExportCount );
		
	{
		CSSFile fileCheck;
		if ( fileCheck.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString str;
			fileCheck.ReadString( str );
			nManualExportNo = atoi( str ) + 1;
		}
	}

	if ( ( nManualExportNo < 1 ) || ( nManualExportNo > 9999 ) )
		nManualExportNo = 1;

	{
		CSSFile fileCheck;
		if ( fileCheck.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		{
			CString str;
			str.Format( "%d", nManualExportNo );
			fileCheck.WriteLine( str );
		}
	}

	return nManualExportNo;
}

/**********************************************************************/

void CPosTrayTask::GetSelectedReportList(CArray<CPosTraySelectedReport>& arrayReports)
{
	CCSV csvReports(GetBatchReportList());

	arrayReports.RemoveAll();

	switch (m_nMode)
	{
	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_EPOS_BATCH:
	case POSTRAY_TASKFILE_EPOS_ALL:
	{
		CEposReportSelect EposReportSelect;

		for (int n = 0; n < csvReports.GetSize(); n++)
		{
			CPosTraySelectedReport info;
			info.m_strReportID = csvReports.GetString(n);

			int nReportType = csvReports.GetInt(n);

			if (nReportType < EPOS_CUSTOM_FIRST_REPORT)
			{
				info.m_strReportName = CEposReport::GetReportNameExternal(nReportType);
			}
			else
			{
				info.m_strReportName = EposReportSelect.GetCustomReportName(nReportType);
			}

			arrayReports.Add(info);
		}
	}
	break;

	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_HIST_BATCH:
	case POSTRAY_TASKFILE_HIST_ALL:
	{
		CReportConsolidationArray<CSortedStringItem> arrayEntityIDs;
		for (int n = 0; n < csvReports.GetSize(); n++)
		{
			CSortedStringItem item;
			item.m_strItem = csvReports.GetString(n);
			arrayEntityIDs.Consolidate(item);
		}

		CDataManagerInfo info;
		DataManagerNonDb.OpenSalesHistoryPlu(DB_READONLY, info);

		for (int n = 0; n < DataManagerNonDb.SalesHistoryPlu.GetSize(); n++)
		{
			CSalesHistoryCSVRecordPlu ReportRecord;

			DataManagerNonDb.SalesHistoryPlu.GetAt(n, ReportRecord);

			CSortedStringItem item;
			item.m_strItem = ReportRecord.GetEntityID();

			int nPos;
			if (arrayEntityIDs.Find(item, nPos) == TRUE)
			{
				CPosTraySelectedReport infoSelected;
				infoSelected.m_strReportID = item.m_strItem;
				infoSelected.m_strReportName = ReportRecord.GetListName();
				arrayReports.Add(infoSelected);
			}
		}
		DataManagerNonDb.CloseSalesHistoryPlu(info);
	}
	break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_BATCH:
	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
	{
		for (int n = 0; n < csvReports.GetSize(); n++)
		{
			CPosTraySelectedReport infoSelected;
			infoSelected.m_strReportName = "";

			switch (csvReports.GetInt(n))
			{
			case 0:
				infoSelected.m_strReportID = "0";
				infoSelected.m_strReportName = "Detailed Report";
				break;

			case 1:
				infoSelected.m_strReportID = "1";
				infoSelected.m_strReportName = "Export Summary (shifts only)";
				break;

			case 2:
				infoSelected.m_strReportID = "2";
				infoSelected.m_strReportName = "Export Summary (with breaks)";
				break;
			}

			if (infoSelected.m_strReportName != "")
			{
				arrayReports.Add(infoSelected);
			}
		}
	}
	break;

#ifndef STOCKMAN_SYSTEM
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_BATCH:
	case POSTRAY_TASKFILE_STKLEVEL_ALL:
	{
		for (int n = 0; n < csvReports.GetSize(); n++)
		{
			CPosTraySelectedReport infoSelected;
			infoSelected.m_strReportName = "";

			int nReportType = csvReports.GetInt(n);
			switch ( nReportType)
			{
			case REPORT_PLUSTOCK_APPARENT:
				infoSelected.m_strReportName = "Apparent Stock";
				break;

			case REPORT_PLUSTOCK_VALUATION:
				infoSelected.m_strReportName = "Apparent Valuation";
				break;

			case REPORT_PLUSTOCK_REORDER:
				infoSelected.m_strReportName = "Reorder Report";
				break;

			case REPORT_PLUSTOCK_DEL_AUDIT:
				infoSelected.m_strReportName = "Delivery Audit";
				break;

			case REPORT_PLUSTOCK_DEL_TOTAL:
				infoSelected.m_strReportName = "Delivery Totals";
				break;

			case REPORT_PLUSTOCK_DEL_VALUE:
				infoSelected.m_strReportName = "Delivery Valuation";
				break;

			}

			if (infoSelected.m_strReportName != "")
			{
				infoSelected.m_strReportID.Format("%4.4d", nReportType);
				arrayReports.Add(infoSelected);
			}
		}
	}
	break;
#endif
	}
}

/**********************************************************************/

void CPosTrayTask::CreateEmailReportMap( CReportConsolidationArray<CSortedIntByString>& arrayEmailByReport)
{
	CreateEmailReportMap(GetTaskEmailByReport(), arrayEmailByReport);
}

/**********************************************************************/

void CPosTrayTask::CreateEmailReportMap( CString strEmailByReport, CReportConsolidationArray<CSortedIntByString>& arrayEmailByReport)
{
	arrayEmailByReport.RemoveAll();

	CCSV csvEmailByReport(strEmailByReport);
	for (int n = 0; n < csvEmailByReport.GetSize() - 1; n += 2)
	{
		CSortedIntByString info{};
		info.m_strItem = csvEmailByReport.GetString(n);
		info.m_nItem = csvEmailByReport.GetInt(n + 1);
		arrayEmailByReport.Consolidate(info);
	}
}

/**********************************************************************/

