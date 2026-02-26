#pragma once
/**********************************************************************/
#define POSTRAY_MAXLENGTH_TASK_NAME 20
#define POSTRAY_MAXLENGTH_BATCH_NAME 20
#define POSTRAY_MAXLENGTH_FIELD_NAME 50
/**********************************************************************/
#define POSTRAY_MAX_TASK 99
#define POSTRAY_MAX_BATCH 99
#define POSTRAY_MAX_FIELDS_PLU 30
#define POSTRAY_MAX_FIELDS_PAYMENT 30
#define POSTRAY_MAX_FIELDS_VOID 30
#define POSTRAY_MAX_FIELDS_DISCOUNT 30
#define POSTRAY_MAX_EMAIL_NICKNAMES 5
/**********************************************************************/
#define POSTRAY_TASKFILE_EPOS_TASK 0
#define POSTRAY_TASKFILE_EPOS_BATCH 1
#define POSTRAY_TASKFILE_EPOS_ALL 2
/**********************************************************************/
#define POSTRAY_TASKFILE_HIST_TASK 3
#define POSTRAY_TASKFILE_HIST_BATCH 4
#define POSTRAY_TASKFILE_HIST_ALL 5
/**********************************************************************/
#define POSTRAY_TASKFILE_EXPORT_SALES_TASK 6
#define POSTRAY_TASKFILE_EXPORT_SALES_BATCH 7
#define POSTRAY_TASKFILE_EXPORT_SALES_ALL 8
/**********************************************************************/
#define POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK 9
#define POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH 10
#define POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL 11
/**********************************************************************/
#define POSTRAY_TASKFILE_EXPORT_VOID_TASK 12
#define POSTRAY_TASKFILE_EXPORT_VOID_BATCH 13
#define POSTRAY_TASKFILE_EXPORT_VOID_ALL 14
/**********************************************************************/
#define POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK 15
#define POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH 16
#define POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL 17
/**********************************************************************/
#define POSTRAY_TASKFILE_STKLEVEL_TASK 18
#define POSTRAY_TASKFILE_STKLEVEL_BATCH 19
#define POSTRAY_TASKFILE_STKLEVEL_ALL 20
/**********************************************************************/
#define POSTRAY_TASKFILE_PROCESSTRAN_TASK 21
#define POSTRAY_TASKFILE_PROCESSTRAN_BATCH 22
#define POSTRAY_TASKFILE_PROCESSTRAN_ALL 23
/**********************************************************************/
#define POSTRAY_TASKFILE_TIMEATTEND_TASK 24
#define POSTRAY_TASKFILE_TIMEATTEND_BATCH 25
#define POSTRAY_TASKFILE_TIMEATTEND_ALL 26
/**********************************************************************/
#define POSTRAY_EPOS_CONSOL_DEFAULT 0
#define POSTRAY_EPOS_CONSOL_SAVED 1
#define POSTRAY_EPOS_CONSOL_SYSTEM 2
/**********************************************************************/
#define EMAILTASK_VALIDATE_OK 0
#define EMAILTASK_VALIDATE_NOBATCH 1
#define EMAILTASK_VALIDATE_NOJOBS 2
#define EMAILTASK_VALIDATE_NODAYS 3
#define EMAILTASK_VALIDATE_NOCONTYPE 4
#define EMAILTASK_VALIDATE_NOEMAIL 5
#define EMAILTASK_VALIDATE_NOACTION 6
#define EMAILTASK_VALIDATE_WRONGTYPE 7
/**********************************************************************/
#include "EnhancedIniFile.h"
#include "PosTrayExportFieldNames.h"
/**********************************************************************/

struct CPosTrayTaskConsolInfo
{
	int m_nQuickType;
	int m_nDbNo;
	int m_nNwkNo;
	int m_nLocNo;
	int m_nTermNo;
};

/**********************************************************************/

struct CPosTraySelectedReport
{
public:
	CPosTraySelectedReport();

public:
	CString m_strReportID;
	CString m_strReportName;
};

/**********************************************************************/

struct CPosTrayTaskSortInfo
{
public:
	CPosTrayTaskSortInfo();
	void Reset();
	
public:
	int Compare ( CPosTrayTaskSortInfo& source, int nHint = 0 );
	void Add	( CPosTrayTaskSortInfo& source );
	
public:
	int m_nFieldGroupForSort;
	int m_nFieldTypeForSort;
	int m_nFieldIdxForSort;

public:
	int m_nFieldTypeForList;
};

/**********************************************************************/

class CPosTrayTask
{
public:
	CPosTrayTask( int nMode );
	void Reset();
	bool IsExportTask();
	bool IsProcessTranTask();

	bool Read( bool bTemp );
	bool Write( bool bTemp );

	bool GetFatalReadErrorFlag()	{ return m_bFatalReadError; }
	void ClearFatalReadErrorFlag()	{ m_bFatalReadError = FALSE; }

	void CopyFrom( CPosTrayTask& source );

private:
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );	
	void PrepareIniFile( CEnhancedIniFile& iniFile );

public:
	int GetMode(){ return m_nMode; }
	void MergeTask( CPosTrayTask& source );
	void MergeBatch( CPosTrayTask& source );

	//TASK NUMBER
	int GetTaskNo()					{ return m_nTaskNo; }
	void SetTaskNo( int n );

	//BATCH NUMBER
	int GetBatchNo()				{ return m_nBatchNo; }
	void SetBatchNo( int n );

	//LAST RUN DATE
	const char* GetLastRunTime( bool bTemp );
	void SetLastRunTime( const char* sz, bool bTemp );
	void SetLastRunTime( COleDateTime& date, bool bTemp );

	int ValidateTask();
	void GetExportFieldListSales( CArray<int,int>& arrayFields );
	void SetExportFieldListSales( CArray<int,int>& arrayFields );
	void TidyExportFieldListSales( CArray<int,int>& arrayFields );
	void GetExportFieldListPayment( CArray<int,int>& arrayFields );
	void SetExportFieldListPayment( CArray<int,int>& arrayFields );
	void TidyExportFieldListPayment( CArray<int,int>& arrayFields );
	void GetExportFieldListVoid( CArray<int,int>& arrayFields );
	void SetExportFieldListVoid( CArray<int,int>& arrayFields );
	void TidyExportFieldListVoid( CArray<int,int>& arrayFields );
	void GetExportFieldListDiscount( CArray<int,int>& arrayFields );
	void SetExportFieldListDiscount( CArray<int,int>& arrayFields );
	void TidyExportFieldListDiscount( CArray<int,int>& arrayFields );
	
	const char* GetChartText();

	int GetMergedTaskType();

	//GET FUNCTIONS
	const char* GetTaskName()			{ return m_TaskName.GetValue(); }
	int GetTaskBatchNo()				{ return m_TaskBatchNo.GetValue(); }
	CString GetTaskEmailNickname(int n);
	int GetTaskFormatEmail()			{ return m_TaskFormatEmail.GetValue(); }
	const char* GetTaskEmailByReport()  { return m_TaskEmailByReport.GetValue(); }
	int GetTaskFormatExport()			{ return m_TaskFormatExport.GetValue(); }
	const char* GetTaskExportFolder()	{ return m_TaskExportFolder.GetValue(); }
	const char* GetTaskExportFilename()	{ return m_TaskExportFilename.GetValue(); }
	int GetTaskExportFilenameFormat()	{ return m_TaskExportNameFormat.GetValue(); }
	bool GetTaskLocNameFlag()			{ return m_TaskLocNameFlag.GetValue(); }
	bool GetTaskFTPFlag()				{ return m_TaskFTPFlag.GetValue(); }
	const char* GetTaskFTPFolder()		{ return m_TaskFTPFolder.GetValue(); }
	int GetTaskExportDateType()			{ return m_TaskExportDateType.GetValue(); }
	bool GetTaskAutoPrintFlag()			{ return m_TaskAutoPrint.GetValue(); }
	int GetLegacyTaskTimeHour()			{ return m_LegacyTaskHour.GetValue(); }
	int GetLegacyTaskTimeMinute()		{ return m_LegacyTaskMinute.GetValue(); }
	int GetTaskTimeHourStart()			{ return m_TaskHourStart.GetValue(); }
	int GetTaskTimeMinuteStart()		{ return m_TaskMinuteStart.GetValue(); }
	int GetTaskTimeHourEnd()			{ return m_TaskHourEnd.GetValue(); }
	int GetTaskTimeMinuteEnd()			{ return m_TaskMinuteEnd.GetValue(); }
	int GetTaskTimeFrequency()			{ return m_TaskFrequency.GetValue(); }
	bool GetTaskDaySunFlag()			{ return m_TaskSun.GetValue(); }
	bool GetTaskDayMonFlag()			{ return m_TaskMon.GetValue(); }
	bool GetTaskDayTueFlag()			{ return m_TaskTue.GetValue(); }
	bool GetTaskDayWedFlag()			{ return m_TaskWed.GetValue(); }
	bool GetTaskDayThuFlag()			{ return m_TaskThu.GetValue(); }
	bool GetTaskDayFriFlag()			{ return m_TaskFri.GetValue(); }
	bool GetTaskDaySatFlag()			{ return m_TaskSat.GetValue(); }
	bool GetTaskBacklogFlag()			{ return m_TaskBacklog.GetValue(); }

	int GetTaskMonthDays()
	{
		int nResult = m_TaskMonthDays.GetValue();
		nResult &= GetAllMonthDaysMask();

		if (0 == nResult)
		{
			nResult = 1;
		}

		return nResult;
	}

	bool GetTaskMonthDayFlag(int nDay)
	{
		bool bResult = FALSE;

		if ((nDay >= 1) && (nDay <= 31))
		{
			bResult = GetTaskMonthDays() & (1 << (nDay - 1));
		}

		return bResult;
	}
	
	//GET FUNCTIONS
	const char* GetBatchName()			{ return m_BatchName.GetValue(); }
	const char* GetBatchDateType()		{ return m_BatchDateType.GetValue(); }
	const char* GetBatchSessionLegacy()	{ return m_BatchSessionLegacy.GetValue(); }
	const char* GetBatchSessionEntity()	{ return m_BatchSessionEntity.GetValue(); }
	int GetBatchConType()				{ return m_BatchConType.GetValue(); }
	int GetBatchConDbNo()				{ return m_BatchConDbNo.GetValue(); }
	int GetBatchConNwkNo()				{ return m_BatchConNwkNo.GetValue(); }
	int GetBatchConEntityNo()			{ return m_BatchConEntityNo.GetValue(); }
	int GetBatchConTermIdx()			{ return m_BatchConTermIdx.GetValue(); }
	int GetBatchTermDateType()			{ return m_BatchTermDateType.GetValue(); }
	int GetBatchTableFilterType()		{ return m_BatchTableFilterType.GetValue(); }
	int GetBatchTableFilterLevel()		{ return m_BatchTableFilterLevel.GetValue(); }
	int GetBatchTableFilterEntityNo()	{ return m_BatchTableFilterEntityNo.GetValue(); }
	int GetBatchDeptFilter()			{ return m_BatchDeptFilter.GetValue(); }
	int GetBatchPluFilter()				{ return m_BatchPluFilter.GetValue(); }
	bool GetBatchSaleTimeFlag()			{ return m_BatchSaleTime.GetValue(); }
	int GetBatchPreferConsolType()		{ return m_BatchPreferConsolType.GetValue(); }
	bool GetBatchJobSetConsolType()		{ return m_BatchJobSetConsolType.GetValue(); }
	bool GetBatchReportEcrFlag()		{ return m_BatchReportEcr.GetValue(); }
	bool GetBatchReportLocFlag()		{ return m_BatchReportLoc.GetValue(); }
	bool GetBatchReportAllFlag()		{ return m_BatchReportAll.GetValue(); }
	bool GetBatchReportConFlag()		{ return m_BatchReportCon.GetValue(); }
	bool GetBatchItemServerFlag()		{ return m_BatchItemServer.GetValue(); }
	bool GetBatchTrainServerFlag()		{ return m_BatchTrainServer.GetValue(); }
	int GetBatchPMSType()				{ return m_BatchPMSType.GetValue(); }
	bool GetBatchPMSPendingFlag()		{ return m_BatchPMSPending.GetValue(); }
	bool GetBatchPMSActiveFlag()		{ return m_BatchPMSActive.GetValue(); }
	bool GetBatchPMSCompleteFlag()		{ return m_BatchPMSComplete.GetValue(); }
	bool GetBatchPMSCancelledFlag()		{ return m_BatchPMSCancelled.GetValue(); }
	const char* GetBatchReportList()	{ return m_BatchReportList.GetValue(); }
	bool GetBatchReportTimeFlag()		{ return m_BatchReportTime.GetValue(); }

	//GET FUNCTIONS
	int GetBatchDateRoundTo()			{ return m_BatchDateRoundTo.GetValue(); }
	const char* GetBatchTimeSlice()		{ return m_BatchTimeSlice.GetValue(); }
	bool GetBatchPluRangeFlag()			{ return m_BatchPluRange.GetValue(); }
	__int64 GetBatchPluNoFrom()			{ return m_BatchPluFrom.GetValue(); }
	__int64 GetBatchPluNoTo()			{ return m_BatchPluTo.GetValue(); }
	int GetBatchDeptFilterType()		{ return m_BatchDeptFilterType.GetValue(); }
	int GetBatchModifierFilter()		{ return m_BatchModifierFilter.GetValue(); }
	int GetBatchModifierCustom()		{ return m_BatchModifierCustom.GetValue(); }
	int GetBatchTaxBandFilter()			{ return m_BatchTaxBandFilter.GetValue(); }
	int GetBatchTaxBandCustom()			{ return m_BatchTaxBandCustom.GetValue(); }
	int GetBatchPriceLevelFilter()		{ return m_BatchPriceLevelFilter.GetValue(); }
	int GetBatchPriceLevelCustom()		{ return m_BatchPriceLevelCustom.GetValue(); }
	bool GetBatchEposSalesFlag()		{ return m_BatchEposSales.GetValue(); }
	bool GetBatchManualSalesFlag()		{ return m_BatchManualSales.GetValue(); }
	bool GetBatchImportSalesFlag()		{ return m_BatchImportSales.GetValue(); }
	bool GetBatchPMSTerminalFlag()		{ return m_BatchPMSTerminal.GetValue(); }
	bool GetBatchMonthSortFlag()		{ return m_BatchMonthSort.GetValue(); }
	bool GetBatchDeptSortFlag()			{ return m_BatchDeptSort.GetValue(); }
	bool GetBatchSupplierSortFlag()		{ return m_BatchSupplierSort.GetValue(); }
	int GetBatchSupplierFilter()		{ return m_BatchSupplierFilter.GetValue(); }
	bool GetBatchStockZeroSkipFlag()	{ return m_BatchStockZeroSkip.GetValue(); }
	bool GetBatchPercentageFlag()		{ return m_BatchPercentage.GetValue(); }
	bool GetBatchEstProfitFlag()		{ return m_BatchEstProfit.GetValue(); }
	bool GetBatchCustomerOnlyFlag()		{ return m_BatchCustomerOnly.GetValue(); }
	bool GetBatchVIPOnlyFlag()			{ return m_BatchVIPOnly.GetValue(); }
	int GetBatchChartType()				{ return m_BatchChartType.GetValue(); }
	bool GetBatchChartBottom100Flag()	{ return m_BatchChartBottom.GetValue(); }
	bool GetBatchChartByValueFlag()		{ return m_BatchChartValue.GetValue(); }
	int GetBatchChartSize()				{ return m_BatchChartSize.GetValue(); }
	double GetBatchChartCutOff()		{ return m_BatchChartCutOff.GetValue(); }
	int GetBatchTaxDecimalPlaces()		{ return m_BatchTaxDP.GetValue(); }
	int GetBatchOfferFilter()			{ return m_BatchOfferFilter.GetValue(); }

	int GetLegacyBatchTaxFilter()		{ return m_LegacyBatchTaxFilter.GetValue(); }
	int GetLegacyBatchTaxCustom()		{ return m_LegacyBatchTaxCustom.GetValue(); }

	//GET FUNCTIONS
	int GetBatchTAGroup()				{ return m_BatchTAGroup.GetValue(); }
	int GetBatchTAEdit()				{ return m_BatchTAEdit.GetValue(); }
	int GetBatchTATotalHours()			{ return m_BatchTATotalHours.GetValue(); }
	bool GetBatchTAOpenShiftFlag()		{ return m_BatchTAOpenShift.GetValue(); }
	bool GetBatchTAClosingTimeFlag()	{ return m_BatchTAClosingTime.GetValue(); }
	int GetBatchTAServerPickType()		{ return m_BatchTAServerPickType.GetValue(); }
	int GetBatchTAServerPickFrom()		{ return m_BatchTAServerPickFrom.GetValue(); }
	int GetBatchTAServerPickTo()		{ return m_BatchTAServerPickTo.GetValue(); }
	
	//GET FUNCTIONS
	int GetBatchExportFieldSales( int n );
	int GetBatchExportFieldPayment( int n );
	int GetBatchExportFieldVoid( int n );
	int GetBatchExportFieldDiscount( int n );
	
	//GET FUNCTIONS
	bool GetMergeDeleteFlag()			{ return m_MergeDelete.GetValue(); }

	//SET FUNCTIONS
	void SetTaskName( const char* sz )			{ m_TaskName.SetValue(sz); }
	void SetTaskBatchNo( int n )				{ m_TaskBatchNo.SetValue(n); }
	void SetTaskEmailNickname(int n, const char* sz);
	void SetTaskFormatEmail( int n )			{ m_TaskFormatEmail.SetValue(n); }
	void SetTaskEmailByReport( const char* sz) { m_TaskEmailByReport.SetValue(sz); }
	void SetTaskFormatExport( int n )			{ m_TaskFormatExport.SetValue(n); }
	void SetTaskExportFolder( const char* sz )	{ m_TaskExportFolder.SetValue(sz); }
	void SetTaskExportFilename( const char* sz ){ m_TaskExportFilename.SetValue(sz); }
	void SetTaskExportFilenameFormat( int n )	{ m_TaskExportNameFormat.SetValue(n); }
	void SetTaskLocNameFlag( bool b )			{ m_TaskLocNameFlag.SetValue(b); }
	void SetTaskFTPFlag( bool b )				{ m_TaskFTPFlag.SetValue(b); }
	void SetTaskFTPFolder( const char* sz )		{ m_TaskFTPFolder.SetValue(sz); }
	void SetTaskExportDateType( int n )			{ m_TaskExportDateType.SetValue(n); }
	void SetTaskAutoPrintFlag( bool b )			{ m_TaskAutoPrint.SetValue(b); }
	void SetLegacyTaskTimeHour( int n )			{ m_LegacyTaskHour.SetValue(n); }
	void SetLegacyTaskTimeMinute( int n )		{ m_LegacyTaskMinute.SetValue(n); }
	void SetTaskTimeHourStart( int n )			{ m_TaskHourStart.SetValue(n); }
	void SetTaskTimeMinuteStart( int n )		{ m_TaskMinuteStart.SetValue(n); }
	void SetTaskTimeHourEnd( int n )			{ m_TaskHourEnd.SetValue(n); }
	void SetTaskTimeMinuteEnd( int n )			{ m_TaskMinuteEnd.SetValue(n); }
	void SetTaskTimeFrequency( int n )			{ m_TaskFrequency.SetValue(n); }
	void SetTaskDaySunFlag( bool b )			{ m_TaskSun.SetValue(b); }
	void SetTaskDayMonFlag( bool b )			{ m_TaskMon.SetValue(b); }
	void SetTaskDayTueFlag( bool b )			{ m_TaskTue.SetValue(b); }
	void SetTaskDayWedFlag( bool b )			{ m_TaskWed.SetValue(b); }
	void SetTaskDayThuFlag( bool b )			{ m_TaskThu.SetValue(b); }
	void SetTaskDayFriFlag( bool b )			{ m_TaskFri.SetValue(b); }
	void SetTaskDaySatFlag( bool b )			{ m_TaskSat.SetValue(b); }
	void SetTaskBacklogFlag( bool b )			{ m_TaskBacklog.SetValue(b); }

	void SetTaskMonthDays(int n)
	{
		if ((n >= 1) && ( n <= GetAllMonthDaysMask()))
		{
			m_TaskMonthDays.SetValue(n);
		}
	}
	
	void SetTaskMonthDayFlag(int nDay, bool bFlag)
	{
		if ((nDay >= 1) && (nDay <= 31))
		{
			int nMask = 1 << (nDay - 1);
			int nDays = m_TaskMonthDays.GetValue();
			
			if (bFlag)
			{
				nDays |= nMask;
			}
			else
			{
				nDays &= ~nMask;
			}

			m_TaskMonthDays.SetValue(nDays);
		}
	}

	//SET FUNCTIONS
	void SetBatchName( const char* sz )			{ m_BatchName.SetValue(sz); }
	void SetBatchDateType( const char* sz )		{ m_BatchDateType.SetValue(sz); }
	void SetBatchSessionLegacy( const char* sz ){ m_BatchSessionLegacy.SetValue(sz); }
	void SetBatchSessionEntity( const char* sz ){ m_BatchSessionEntity.SetValue(sz); }
	void SetBatchConType( int n )				{ m_BatchConType.SetValue(n); }
	void SetBatchConDbNo( int n )				{ m_BatchConDbNo.SetValue(n); }
	void SetBatchConNwkNo( int n )				{ m_BatchConNwkNo.SetValue(n); }
	void SetBatchConEntityNo( int n )			{ m_BatchConEntityNo.SetValue(n); }
	void SetBatchConTermIdx( int n )			{ m_BatchConTermIdx.SetValue(n); }
	void SetBatchTermDateType(int n)			{ m_BatchTermDateType.SetValue(n); }
	void SetBatchTableFilterType( int n )		{ m_BatchTableFilterType.SetValue(n); }
	void SetBatchTableFilterLevel( int n )		{ m_BatchTableFilterLevel.SetValue(n); }
	void SetBatchTableFilterEntityNo( int n )	{ m_BatchTableFilterEntityNo.SetValue(n); }
	void SetBatchDeptFilter( int n )			{ m_BatchDeptFilter.SetValue(n); }
	void SetBatchPluFilter( int n )				{ m_BatchPluFilter.SetValue(n); }
	void SetBatchSaleTimeFlag( bool b )			{ m_BatchSaleTime.SetValue(b); }
	void SetBatchPreferConsolType( int n )		{ m_BatchPreferConsolType.SetValue(n); }
	void SetBatchJobSetConsolType( bool b )		{ m_BatchJobSetConsolType.SetValue(b); }
	void SetBatchReportEcrFlag( bool b )		{ m_BatchReportEcr.SetValue(b); }
	void SetBatchReportLocFlag( bool b )		{ m_BatchReportLoc.SetValue(b); }
	void SetBatchReportAllFlag( bool b )		{ m_BatchReportAll.SetValue(b); }
	void SetBatchReportConFlag( bool b )		{ m_BatchReportCon.SetValue(b); }
	void SetBatchItemServerFlag( bool b )		{ m_BatchItemServer.SetValue(b); }
	void SetBatchTrainServerFlag( bool b )		{ m_BatchTrainServer.SetValue(b); }
	void SetBatchPMSType( int n )				{ m_BatchPMSType.SetValue(n); }
	void SetBatchPMSPendingFlag( bool b )		{ m_BatchPMSPending.SetValue(b); }
	void SetBatchPMSActiveFlag( bool b )		{ m_BatchPMSActive.SetValue(b); }
	void SetBatchPMSCompleteFlag( bool b )		{ m_BatchPMSComplete.SetValue(b); }
	void SetBatchPMSCancelledFlag( bool b )		{ m_BatchPMSCancelled.SetValue(b); }
	void SetBatchReportList( const char* sz )	{ m_BatchReportList.SetValue(sz); }
	void SetBatchReportTimeFlag( bool b )		{ m_BatchReportTime.SetValue(b); }
	
	//SET FUNCTIONS
	void SetBatchDateRoundTo( int n )			{ m_BatchDateRoundTo.SetValue(n); }
	void SetBatchTimeSlice( const char* sz )	{ m_BatchTimeSlice.SetValue(sz); }
	void SetBatchPluRangeFlag( bool b )			{ m_BatchPluRange.SetValue(b); }
	void SetBatchPluNoFrom( __int64 n )			{ m_BatchPluFrom.SetValue(n); }
	void SetBatchPluNoTo( __int64 n )			{ m_BatchPluTo.SetValue(n); }
	void SetBatchDeptFilterType( int n )		{ m_BatchDeptFilterType.SetValue(n); }
	void SetBatchModifierFilter( int n );
	void SetBatchModifierCustom( int n )		{ m_BatchModifierCustom.SetValue(n); }
	void SetBatchTaxBandFilter( int n );
	void SetBatchTaxBandCustom( int n )			{ m_BatchTaxBandCustom.SetValue(n); }
	void SetBatchPriceLevelFilter( int n );
	void SetBatchPriceLevelCustom( int n )		{ m_BatchPriceLevelCustom.SetValue(n); }
	void SetBatchEposSalesFlag( bool b )		{ m_BatchEposSales.SetValue(b); }		
	void SetBatchManualSalesFlag( bool b )		{ m_BatchManualSales.SetValue(b); }		
	void SetBatchImportSalesFlag( bool b )		{ m_BatchImportSales.SetValue(b); }
	void SetBatchPMSTerminalFlag( bool b )		{ m_BatchPMSTerminal.SetValue(b); }		
	void SetBatchMonthSortFlag( bool b )		{ m_BatchMonthSort.SetValue(b); }	
	void SetBatchDeptSortFlag( bool b )			{ m_BatchDeptSort.SetValue(b); }
	void SetBatchSupplierSortFlag( bool b )		{ m_BatchSupplierSort.SetValue(b); }
	void SetBatchSupplierFilter( int n )		{ m_BatchSupplierFilter.SetValue(n); }
	void SetBatchStockZeroSkipFlag( bool b )	{ m_BatchStockZeroSkip.SetValue(b); }
	void SetBatchPercentageFlag( bool b )		{ m_BatchPercentage.SetValue(b); }		
	void SetBatchEstProfitFlag( bool b )		{ m_BatchEstProfit.SetValue(b); }		
	void SetBatchCustomerOnlyFlag( bool b )		{ m_BatchCustomerOnly.SetValue(b); }	
	void SetBatchVIPOnlyFlag( bool b )			{ m_BatchVIPOnly.SetValue(b); }	
	void SetBatchChartType( int n )				{ m_BatchChartType.SetValue(n); }				
	void SetBatchChartBottom100Flag( bool b )	{ m_BatchChartBottom.SetValue(b); }	
	void SetBatchChartByValueFlag( bool b )		{ m_BatchChartValue.SetValue(b); }		
	void SetBatchChartSize( int n )				{ m_BatchChartSize.SetValue(n); }
	void SetBatchOfferFilter( int n )			{ m_BatchOfferFilter.SetValue(n); }
	void SetBatchChartCutOff( double d )		{ m_BatchChartCutOff.SetValue(d); }
	void SetBatchTaxDecimalPlaces( int n )		{ m_BatchTaxDP.SetValue(n); }

	//SET FUNCTIONS
	void SetBatchTAGroup( int n )				{ m_BatchTAGroup.SetValue(n); }
	void SetBatchTAEdit( int n )				{ m_BatchTAEdit.SetValue(n); }
	void SetBatchTATotalHours( int n )			{ m_BatchTATotalHours.SetValue(n); }
	void SetBatchTAOpenShiftFlag(bool b)		{ m_BatchTAOpenShift.SetValue(b); }
	void SetBatchTAClosingTimeFlag(bool b)		{ m_BatchTAClosingTime.SetValue(b); }
	void SetBatchTAServerPickType(int n)		{ m_BatchTAServerPickType.SetValue(n); }
	void SetBatchTAServerPickFrom(int n)		{ m_BatchTAServerPickFrom.SetValue(n); }
	void SetBatchTAServerPickTo(int n)			{ m_BatchTAServerPickTo.SetValue(n); }

	//SET FUNCTIONS
	void SetBatchExportFieldSales( int n, int nField );
	void SetBatchExportFieldPayment( int n, int nField );
	void SetBatchExportFieldVoid( int n, int nField );
	void SetBatchExportFieldDiscount( int n, int nField );

	//SET FUNCTIONS
	void SetMergeDeleteFlag( bool b )			{ m_MergeDelete.SetValue(b); }

	//TASK MANAGEMENT
	bool GetNextRunTime( COleDateTime& oleTimeNextRun, CString& strTimeLastRun );
	bool GetNextRunTime( COleDateTime& oleTimeNextRun, CString& strTimeLastRun, COleDateTime& oleTimeNow );

public:
	const char* GetLiveTaskFolder();
	const char* GetFilename( bool bTemp );
	const char* GetDateFilename( bool bTemp );
	void GetSelectedReportList(CArray<CPosTraySelectedReport>& arrayReports);

public:
	void QuickLoadExportFolder( const char* szTaskFilename );
	const char* GetExportFilename( const char* szType, CString& strFixedDateSuffix, COleDateTime& dateTask );
	const char* GetExportFilenameSuffix( COleDateTime& oleDateTime, CPosTrayTaskConsolInfo& infoQuick );
	const char* GetExportFilenameSuffix( COleDateTime& oleDateTime );
	int GetNextManualExportNo();

public:
	static void CreateEmailReportMap(CString strEmailByReport, CReportConsolidationArray<CSortedIntByString>& arrayEmailByReport);
	void CreateEmailReportMap(CReportConsolidationArray<CSortedIntByString>& arrayEmailByReport);

public:
	static int GetAllMonthDaysMask() { return 0x7FFFFFFF; }
	CString GetMonthDaysDisplayString();

private:
	void GetExportFieldList( CIniFileIntArray& arrayIniFile, CArray<int,int>& arrayFields );
	void SetExportFieldList( CIniFileIntArray& arrayIniFile, CArray<int,int>& arrayFields );
	void TidyExportFieldList( CIniFileIntArray& arrayIniFile, CArray<int,int>& arrayFields );
	
private:
	int m_nMode;

private:
	int m_nTaskNo;
	CString m_strDefaultNameTask;

private:
	int m_nBatchNo;
	CString m_strDefaultNameBatch;

private:
	CIniFileString m_TaskName;
	CIniFileInt m_TaskBatchNo;
	CIniFileString m_TaskEmailNickname[POSTRAY_MAX_EMAIL_NICKNAMES];
	CIniFileInt m_TaskFormatEmail;
	CIniFileString m_TaskEmailByReport;
	CIniFileInt m_TaskFormatExport;
	CIniFileString m_TaskExportFolder;
	CIniFileString m_TaskExportFilename;
	CIniFileInt m_TaskExportNameFormat;
	CIniFileBool m_TaskLocNameFlag;
	CIniFileBool m_TaskFTPFlag;
	CIniFileString m_TaskFTPFolder;
	CIniFileInt m_TaskExportDateType;
	CIniFileBool m_TaskAutoPrint;
	CIniFileInt m_LegacyTaskHour;
	CIniFileInt m_LegacyTaskMinute;
	CIniFileInt m_TaskHourStart;
	CIniFileInt m_TaskMinuteStart;
	CIniFileInt m_TaskHourEnd;
	CIniFileInt m_TaskMinuteEnd;
	CIniFileInt m_TaskFrequency;
	CIniFileBool m_TaskSun;
	CIniFileBool m_TaskMon;
	CIniFileBool m_TaskTue;
	CIniFileBool m_TaskWed;
	CIniFileBool m_TaskThu;
	CIniFileBool m_TaskFri;
	CIniFileBool m_TaskSat;
	CIniFileInt m_TaskMonthDays;
	CIniFileBool m_TaskBacklog;

private:
	CString m_strChartText;
	CString m_strTaskLastRunTime;

private:
	CIniFileString m_BatchName;
	CIniFileString m_BatchDateType;
	CIniFileString m_BatchSessionLegacy;
	CIniFileString m_BatchSessionEntity;
	CIniFileInt m_BatchConType;
	CIniFileInt m_BatchConDbNo;
	CIniFileInt m_BatchConNwkNo;
	CIniFileInt m_BatchConEntityNo;
	CIniFileInt m_BatchConTermIdx;
	CIniFileInt m_BatchTermDateType;
	CIniFileInt m_BatchTableFilterType;
	CIniFileInt m_BatchTableFilterLevel;
	CIniFileInt m_BatchTableFilterEntityNo;
	CIniFileInt m_BatchDeptFilter;
	CIniFileInt m_BatchPluFilter;
	CIniFileBool m_BatchSaleTime;
	CIniFileInt m_BatchPreferConsolType;
	CIniFileBool m_BatchJobSetConsolType;
	CIniFileBool m_BatchReportEcr;
	CIniFileBool m_BatchReportLoc;
	CIniFileBool m_BatchReportAll;
	CIniFileBool m_BatchReportCon;
	CIniFileBool m_BatchItemServer;
	CIniFileBool m_BatchTrainServer;
	CIniFileInt m_BatchPMSType;
	CIniFileBool m_BatchPMSPending;
	CIniFileBool m_BatchPMSActive;
	CIniFileBool m_BatchPMSComplete;
	CIniFileBool m_BatchPMSCancelled;
	CIniFileString m_BatchReportList;
	CIniFileBool m_BatchReportTime;
	
private:
	CIniFileInt m_BatchDateRoundTo;
	CIniFileString m_BatchTimeSlice;
	CIniFileBool m_BatchPluRange;
	CIniFileInt64 m_BatchPluFrom;
	CIniFileInt64 m_BatchPluTo;
	CIniFileInt m_BatchDeptFilterType;
	CIniFileInt m_BatchModifierFilter;
	CIniFileInt m_BatchModifierCustom;
	CIniFileInt m_BatchTaxBandFilter;
	CIniFileInt m_BatchTaxBandCustom;
	CIniFileInt m_BatchPriceLevelFilter;
	CIniFileInt m_BatchPriceLevelCustom;
	CIniFileBool m_BatchEposSales;
	CIniFileBool m_BatchManualSales;
	CIniFileBool m_BatchImportSales;
	CIniFileBool m_BatchPMSTerminal;
	CIniFileBool m_BatchMonthSort;
	CIniFileBool m_BatchDeptSort;
	CIniFileBool m_BatchSupplierSort;
	CIniFileInt m_BatchSupplierFilter;
	CIniFileBool m_BatchStockZeroSkip;
	CIniFileBool m_BatchPercentage;
	CIniFileBool m_BatchEstProfit;
	CIniFileBool m_BatchCustomerOnly;
	CIniFileBool m_BatchVIPOnly;
	CIniFileInt m_BatchChartType;
	CIniFileBool m_BatchChartBottom;
	CIniFileBool m_BatchChartValue;
	CIniFileInt m_BatchChartSize;
	CIniFileDouble m_BatchChartCutOff;
	CIniFileInt m_BatchOfferFilter;
	CIniFileInt m_BatchTaxDP;

private:
	CIniFileInt m_LegacyBatchTaxFilter;
	CIniFileInt m_LegacyBatchTaxCustom;

private:
	CIniFileInt m_BatchTAGroup;
	CIniFileInt m_BatchTAEdit;
	CIniFileInt m_BatchTATotalHours;
	CIniFileBool m_BatchTAOpenShift;
	CIniFileBool m_BatchTAClosingTime;
	CIniFileInt m_BatchTAServerPickType;
	CIniFileInt m_BatchTAServerPickFrom;
	CIniFileInt m_BatchTAServerPickTo;
	
private:
	CIniFileIntArray m_BatchExportFieldSales;
	CIniFileIntArray m_BatchExportFieldPayment;
	CIniFileIntArray m_BatchExportFieldVoid;
	CIniFileIntArray m_BatchExportFieldDiscount;
	
private:
	CIniFileBool m_MergeDelete;
	
public:
	CString m_strLiveTaskFolder;
	CString m_strFilename;
	CString m_strDateFilename;
	CString m_strExportFilename;
	CString m_strExportFilenameSuffix;
	bool m_bFatalReadError;
};

/**********************************************************************/

