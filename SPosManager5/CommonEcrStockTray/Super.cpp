/**********************************************************************/
#include "LocationCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "Super.h"
/**********************************************************************/

CSuper::CSuper()
{
	m_strSpecialReportFilename = "";
	m_bPosTraySocket = FALSE;
}

/**********************************************************************/

void CSuper::Initialise()
{
	m_strSchedLogOld = "SchedOld\\SysLog.old";
	GetSyssetProgramPath( m_strSchedLogOld );

	m_strSchedLogAll = "SchedLog\\SysLog.rep";
	GetSyssetProgramPath( m_strSchedLogAll );

	m_strEmailLogAll = "SchedLog\\EmailLog.rep";
	GetSyssetProgramPath( m_strEmailLogAll );

	m_strEmailLogOld = "SchedLog\\EmailOld.rep";
	GetSyssetProgramPath( m_strEmailLogOld );

	m_strAutoDeleteCustLogAll = "SchedLog\\AutoDelCustLog.rep";
	GetSyssetProgramPath( m_strAutoDeleteCustLogAll );
	
	m_strAutoDeleteCustLogOld = "SchedLog\\AutoDelCustOld.rep";
	GetSyssetProgramPath( m_strAutoDeleteCustLogOld );
	
	m_strIDraughtLogOld = "SchedOld\\IDraughtLog.old";
	GetSyssetProgramPath( m_strIDraughtLogOld );

	m_strFNBLogOld = "SchedOld\\FNBLog.old";
	GetSyssetProgramPath( m_strFNBLogOld );

	m_strChartwellsLogOld = "SchedOld\\ChartwellsLog.old";
	GetSyssetProgramPath(m_strChartwellsLogOld);

	m_strS4LabourLogOld = "SchedOld\\S4LabourLog.old";
	GetSyssetProgramPath( m_strS4LabourLogOld );

	m_strFTPLogOld = "SchedOld\\FTPLog.old";
	GetSyssetProgramPath( m_strFTPLogOld );

	m_strSageLogAll = "SchedLog\\SageLog.rep";
	GetSyssetProgramPath( m_strSageLogAll );

	m_strSageLogOld = "SchedOld\\SageLog.old";
	GetSyssetProgramPath( m_strSageLogOld );

	m_strPosTrayLogActionAll = "SchedLog\\PosTrayLog.rep";
	GetSyssetProgramPath( m_strPosTrayLogActionAll );

	m_strPosTrayLogActionOld = "SchedLog\\PosTrayLog.old";
	GetSyssetProgramPath( m_strPosTrayLogActionOld );

	m_strPosTrayLogExportAll = "SchedLog\\ExportLog.rep";
	GetSyssetProgramPath( m_strPosTrayLogExportAll );

	m_strPosTrayLogExportOld = "SchedLog\\ExportOld.rep";
	GetSyssetProgramPath( m_strPosTrayLogExportOld );
	
	m_strIDraughtLogAll = "SchedLog\\IDraughtLog.rep";
	GetSyssetProgramPath( m_strIDraughtLogAll );

	m_strFNBLogAll = "SchedLog\\FNBLog.rep";
	GetSyssetProgramPath( m_strFNBLogAll );

	m_strChartwellsLogAll = "SchedLog\\ChartwellsLog.rep";
	GetSyssetProgramPath(m_strChartwellsLogAll);

	m_strS4LabourLogAll = "SchedLog\\S4LabourLog.rep";
	GetSyssetProgramPath( m_strS4LabourLogAll );

	m_strFTPLogAll = "SchedLog\\FTPLog.rep";
	GetSyssetProgramPath( m_strFTPLogAll );

	m_strUserLog = "SchedLog\\USERLOG.REP";
	GetSyssetProgramPath( m_strUserLog );
	
	m_strUserLogOld = "SchedOld\\USEROLD.REP";
	GetSyssetProgramPath( m_strUserLogOld );
	
	m_strBackupLogToRead = "SchedLog\\BackUp.Log";
	GetSyssetProgramPath( m_strBackupLogToRead );

	m_strFileWriteLogNew = "SchedLog\\WriteLog.dat";
	GetSyssetProgramPath( m_strFileWriteLogNew );

	m_strFileWriteLogOld = "SchedLog\\WriteLog.old";
	GetSyssetProgramPath( m_strFileWriteLogOld );

	m_strWebAPICallLogNew = "SchedLog\\WebAPICallLog.dat";
	GetSyssetProgramPath( m_strWebAPICallLogNew );

	m_strWebAPICallLogOld = "SchedLog\\WebAPICallLog.old";
	GetSyssetProgramPath( m_strWebAPICallLogOld );

#ifdef STOCKMAN_SYSTEM
	m_strBackupLogToWrite.Format ( "Sysset\\%s\\SchedLog\\Backup.Log", Sysset.GetProgramFolder( SYSSET_STKMANSPOSV4 ) );
	m_strBackupFolderToWrite.Format ( "Sysset\\%s\\SchedLog", Sysset.GetProgramFolder( SYSSET_STKMANSPOSV4 ) );
#else
	m_strBackupLogToWrite.Format ( "Sysset\\%s\\SchedLog\\Backup.Log", Sysset.GetProgramFolder( SYSSET_ECRMANSPOSV4 ) );
	m_strBackupFolderToWrite.Format ( "Sysset\\%s\\SchedLog", Sysset.GetProgramFolder( SYSSET_ECRMANSPOSV4 ) );
#endif

	m_strBackupLogParams = "SchedLog\\backup.prm";
	GetSyssetProgramPath( m_strBackupLogParams );

	m_strReportFilename = "REPORT.REP";
	GetTempPath( m_strReportFilename );
	
	m_strHTMLFilename = "REPORT.HTM";
	GetTempPath( m_strHTMLFilename );

	m_strEmailLog = "REPORT2.REP";
	GetTempPath( m_strEmailLog );

	m_strOrderLog = "REPORT3.REP";
	GetTempPath( m_strOrderLog );

	m_strPluStart = "1";
	m_strPluEnd = "99999999999999";
	m_strStockStart = "1";
	m_strStockEnd = "9999999999999999";
	m_strBarcodeEnd = "999999999999999";
}

/**********************************************************************/

long CSuper::MaxLogLocFilesize()		
{
	long lResult = 2048000;

	if ( DealerFlags.GetLogAllCashRSPFilesFlag() == TRUE )
		lResult *= 16;
	else if ( DealerFlags.GetReadStatus() == READ_STATUS_NONE )
		lResult *= 16;

	return lResult;
}

/**********************************************************************/

long CSuper::MaxLogSysFilesize()		
{	
	long lResult =  4096000;
	
	if ( DealerFlags.GetLogAllCashRSPFilesFlag() == TRUE )
		lResult *= 16;
	else if ( DealerFlags.GetReadStatus() == READ_STATUS_NONE )
		lResult *= 16;

	return lResult;
}

/**********************************************************************/

const char* CSuper::ReportFilename()
{
	if ( m_strSpecialReportFilename != "" )
		return m_strSpecialReportFilename;
	else
		return m_strReportFilename;
}

/**********************************************************************/
