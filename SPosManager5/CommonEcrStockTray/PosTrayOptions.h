#pragma once
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

class CPosTrayOptions 
{
public:
	CPosTrayOptions();
	void Reset();

	bool Read();
	bool Write();
	
	void CopyFrom( CPosTrayOptions& source );

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

public:
	void ResetRunNowList();
	void AddToRunNowList(int nTaskType, int nTaskNo);
	void ProcessRunNowList();

private:
	void ReloadFromIniFile( CEnhancedIniFile& file );
	void PrepareIniFile( CEnhancedIniFile& file );

public:
	//GENERAL TAB
	bool GetPosTrayRealTimeFlag()				{ return m_RealTime.GetValue(); }
	bool GetPosTrayProcessTranFlag()			{ return m_ProcessTran.GetValue(); }
	bool GetDisableHibernateFlag()				{ return m_NoHibernate.GetValue(); }
	int GetRealTimeCommsSpeed()					{ return m_CommsSpeed.GetValue(); }
	bool GetReportTaskEposFlag()				{ return (m_ReportTaskEpos.GetValue() == 2); }
	bool GetReportTaskHistoryFlag()				{ return (m_ReportTaskHistory.GetValue() == 2); }
	bool GetReportTaskStockLevelFlag()			{ return m_ReportTaskStockLevel.GetValue(); }
	bool GetReportTaskTimeAttendFlag()			{ return m_ReportTaskTimeAttend.GetValue(); }
	bool GetManageBackupFlag()					{ return m_ManageBackup.GetValue(); }	
	bool GetExportTaskSalesFlag()				{ return (m_ExportTaskSales.GetValue() == 2); }
	bool GetExportTaskPaymentFlag()				{ return (m_ExportTaskPayment.GetValue() == 2); }
	bool GetExportTaskVoidFlag()				{ return m_ExportTaskVoid.GetValue(); }
	bool GetExportTaskDiscountFlag()			{ return m_ExportTaskDiscount.GetValue(); }
	bool GetFNBTaskFlag()						{ return (m_FNBTask.GetValue() == 2); }
	bool GetChartwellsTaskFlag()				{ return m_ChartwellsTask.GetValue(); }
	bool GetS4LabourTaskFlag()					{ return m_S4LabourTask.GetValue(); }
	bool GetEditReportFilenamesFlag()			{ return m_EditFilenames.GetValue(); }
	bool GetEditExportFieldnamesFlag()			{ return m_EditFieldnames.GetValue(); }
	bool GetManualExportFlag()					{ return m_ManualExport.GetValue(); }
	bool GetBusinessDateFlag()					{ return m_BusinessDate.GetValue(); }
	bool GetSkipEmptyFlag()						{ return m_SkipEmpty.GetValue(); }
	bool GetSkipNoChangeFlag()					{ return m_SkipNoChange.GetValue(); }
	int GetExportDateType()						{ return m_ExportDateType.GetValue(); }				

	//FNB TAB
	int GetFNBDelayTime()						{ return m_FNBDelayTime.GetValue(); }

	//CHARTWELLS TAB
	bool GetChartwellsNextDayFlag()				{ return m_ChartwellsNextDay.GetValue(); }
	int GetChartwellsExportTime()				{ return m_ChartwellsExportTime.GetValue(); }

	//S4 LABOUR TAB
	int GetS4LabourExportHour()					{ return m_S4LabourExportHour.GetValue(); }
	const char* GetS4LabourFTPServer()			{ return m_S4LabourFTPServer.GetValue(); }
	const char* GetS4LabourFTPUserName()		{ return m_S4LabourFTPUserName.GetValue(); }
	const char* GetS4LabourFTPPassword()		{ return m_S4LabourFTPPassword.GetValue(); }
	
	//LIPA TAB
	int GetLIPAExportHour()						{ return m_LIPAExportHour.GetValue(); }
	const char* GetLIPAExportFolder()			{ return m_LIPAExportFolder.GetValue(); }
	int GetLIPAExportLocation()					{ return m_LIPAExportLocation.GetValue(); }

	//FTP TAB
	const char* GetFTPServer()					{ return m_FTPServer.GetValue(); }
	const char* GetFTPUserName()				{ return m_FTPUserName.GetValue(); }
	const char* GetFTPPassword()				{ return m_FTPPassword.GetValue(); }
	bool GetFTPUnixModeFlag()					{ return m_FTPUnixMode.GetValue(); }
	
	//GENERAL TAB
	void SetPosTrayRealTimeFlag( bool b )		{ m_RealTime.SetValue(b); }
	void SetPosTrayProcessTranFlag(bool b)		{ m_ProcessTran.SetValue(b); }
	void SetDisableHibernateFlag( bool b )		{ m_NoHibernate.SetValue(b); }
	void SetRealTimeCommsSpeed( int n )			{ m_CommsSpeed.SetValue(n); }
	void SetReportTaskEposFlag( bool b )		{ m_ReportTaskEpos.SetValue(b ? 2 : 1); }
	void SetReportTaskHistoryFlag( bool b )		{ m_ReportTaskHistory.SetValue(b ? 2 : 1); }
	void SetReportTaskStockLevelFlag( bool b )	{ m_ReportTaskStockLevel.SetValue(b); }
	void SetReportTaskTimeAttendFlag(bool b)	{ m_ReportTaskTimeAttend.SetValue(b); }
	void SetManageBackupFlag(bool b)			{ m_ManageBackup.SetValue(b); }
	void SetExportTaskSalesFlag( bool b )		{ m_ExportTaskSales.SetValue(b ? 2 : 1); }
	void SetExportTaskPaymentFlag( bool b )		{ m_ExportTaskPayment.SetValue(b ? 2 : 1); }
	void SetExportTaskVoidFlag( bool b )		{ m_ExportTaskVoid.SetValue(b); }
	void SetExportTaskDiscountFlag( bool b )	{ m_ExportTaskDiscount.SetValue(b); }
	void SetFNBTaskFlag( bool b )				{ m_FNBTask.SetValue(b ? 2 : 1); }
	void SetChartwellsTaskFlag(bool b)			{ m_ChartwellsTask.SetValue(b); }
	void SetS4LabourTaskFlag( bool b )			{ m_S4LabourTask.SetValue(b); }
	void SetEditReportFilenamesFlag( bool b )	{ m_EditFilenames.SetValue(b); }
	void SetEditExportFieldnamesFlag( bool b )	{ m_EditFieldnames.SetValue(b); }
	void SetManualExportFlag( bool b )			{ m_ManualExport.SetValue(b); }
	void SetBusinessDayFlag( bool b )			{ m_BusinessDate.SetValue(b); }
	void SetSkipEmptyFlag(bool b)				{ m_SkipEmpty.SetValue(b); }
	void SetSkipNoChangeFlag(bool b)			{ m_SkipNoChange.SetValue(b); }
	void SetExportDateType( int n )				{ m_ExportDateType.SetValue(n); }

	//FNB TAB
	void SetFNBDelayTime( int n )				{ m_FNBDelayTime.SetValue(n); } 

	//CHARTWELLS TAB
	void SetChartwellsNextDayFlag(bool b)		{ m_ChartwellsNextDay.SetValue(b); }
	void SetChartwellsExportTime(int n)			{ m_ChartwellsExportTime.SetValue(n); }

	//S4 LABOUR TAB
	void SetS4LabourExportHour( int n )			{ m_S4LabourExportHour.SetValue(n); } 
	void SetS4LabourFTPServer( const char* sz )	{ m_S4LabourFTPServer.SetValue(sz); }
	void SetS4LabourFTPUserName( const char* sz ){ m_S4LabourFTPUserName.SetValue(sz); }
	void SetS4LabourFTPPassword( const char* sz ){ m_S4LabourFTPPassword.SetValue(sz); }

	//LIPA
	void SetLIPAExportHour( int n )				{ m_LIPAExportHour.SetValue(n); }
	void SetLIPAExportFolder( const char* sz )	{ m_LIPAExportFolder.SetValue(sz); }
	void SetLIPAExportLocation( int n )			{ m_LIPAExportLocation.SetValue(n); }
	
	//FTP TAB
	void SetFTPServer( const char* sz )			{ m_FTPServer.SetValue(sz); }
	void SetFTPUserName( const char* sz )		{ m_FTPUserName.SetValue(sz); }
	void SetFTPPassword( const char* sz )		{ m_FTPPassword.SetValue(sz); }
	void SetFTPUnixModeFlag( bool b )			{ m_FTPUnixMode.SetValue(b); }
	
	//TEMPORARY FLAGS, NOT SAVED TO FILE
	bool GetTemporaryStartupFlag(){ return m_bTemporaryStartup; }
	void SetTemporaryStartupFlag( bool b ){ m_bTemporaryStartup = b; }

	//COMPOSITE CHECK OF EMAIL TASK OPTIONS
	bool AreTasksEnabled();
	bool AreExportsEnabled();
	bool GotFTPSettings();
	
private:
	//GENERAL TAB
	CIniFileBool m_RealTime;
	CIniFileBool m_ProcessTran;
	CIniFileBool m_NoHibernate;
	CIniFileInt m_CommsSpeed;

	CIniFileInt m_ReportTaskEpos;
	CIniFileInt m_ReportTaskHistory;
	CIniFileBool m_ReportTaskStockLevel;
	CIniFileBool m_ReportTaskTimeAttend;
	CIniFileBool m_ManageBackup;
	CIniFileInt m_ExportTaskSales;
	CIniFileInt m_ExportTaskPayment;
	CIniFileBool m_ExportTaskVoid;
	CIniFileBool m_ExportTaskDiscount;
	CIniFileInt m_FNBTask;
	CIniFileBool m_ChartwellsTask;
	CIniFileBool m_S4LabourTask;

	CIniFileBool m_EditFilenames;
	CIniFileBool m_EditFieldnames;
	CIniFileBool m_ManualExport;
	CIniFileBool m_BusinessDate;
	CIniFileBool m_SkipEmpty;
	CIniFileBool m_SkipNoChange;
	CIniFileInt m_ExportDateType;

	//FNB TAB
	CIniFileInt m_FNBDelayTime;

	//CHARTWELLS TAB
	CIniFileBool m_ChartwellsNextDay;
	CIniFileInt m_ChartwellsExportTime;

	//S4 LABOUR TAB
	CIniFileInt m_S4LabourExportHour;
	CIniFileString m_S4LabourFTPServer;
	CIniFileString m_S4LabourFTPUserName;
	CIniFileString m_S4LabourFTPPassword;

	//LIPA TAB
	CIniFileInt m_LIPAExportHour;
	CIniFileString m_LIPAExportFolder;
	CIniFileInt m_LIPAExportLocation;
	
	//FTP TAB
	CIniFileString m_FTPServer;
	CIniFileString m_FTPUserName;
	CIniFileString m_FTPPassword;
	CIniFileBool m_FTPUnixMode;

	//TEMPORARY FLAGS, NOT SAVED TO FILE
	bool m_bTemporaryStartup;
	
public:
	bool m_bFatalReadError;

private:
	CReportConsolidationArray<CSortedIntAndInt> m_arrayRunNow;
};

/**********************************************************************/
extern CPosTrayOptions PosTrayOptions; /* global */
/**********************************************************************/

