/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MessageLogger.h"
/**********************************************************************/

CMessageLoggerInfo::CMessageLoggerInfo()
{
	m_strMessage = "";
	m_strStatus = "";
	m_nHideLevel = 0;
	m_nNetworkIdx = 0;
	m_bAddNetworkName = TRUE;
	m_bAddDate = TRUE;
	m_bAddTimeOfDay = TRUE;
	m_bLeadingLineNetwork = FALSE;
	m_bFollowingLineNetwork = FALSE;
	m_bLeadingLineSystem = FALSE;
	m_bFollowingLineSystem = FALSE;
	m_bLogSystemStartStop = FALSE;
};

/**********************************************************************/

CEmailLoggerInfo::CEmailLoggerInfo()
{
	m_strEmail = "";
	m_strFilename = "";
	m_strSubject = "";
	m_strResult = "";
	m_strDate = "";
	m_strTime = "";
	m_nHideLevel = 0;
}

/**********************************************************************/

CExportLoggerInfo::CExportLoggerInfo()
{
	m_strType = "";
	m_strTargetFolder = "";
	m_strTargetFile = "";
	m_strResult = "";
	m_strDate = "";
	m_strTime = "";
	m_nHideLevel = 0;
}

/**********************************************************************/

CAutoDeleteCustLogInfo::CAutoDeleteCustLogInfo()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	m_strDate.Format( "%2.2d/%2.2d/%4.4d",
		timeNow.GetDay(),
		timeNow.GetMonth(),
		timeNow.GetYear() );

	m_strTime.Format( "%2.2d:%2.2d:%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	m_nLocIdx = 0;
	m_bOneOff = TRUE;
	m_strInfo = "";
	m_nHideLevel = 0;
}

/**********************************************************************/

CMessageLogger::CMessageLogger() 
{
	m_strLastDate = "";
	m_nLineIndex = 0;
	m_strChecksumSeed = "";
	m_strLastEmailMessage = "";
	m_strLastNetworkMessage = "";
	m_strLastSystemMessage = "";
}

/**********************************************************************/

void CMessageLogger::Initialise()
{
	m_LogFileScheduler.SetFilename( Super.SchedLogAll() );
	CFilenameUpdater FnUp( SysFiles::SystemLogNew );
	m_LogFileGrayLog.SetFilename( FnUp.GetFilenameToUse() );
	m_LogFileEmail.SetFilename( Super.EmailLogAll() );
	m_LogFileExport.SetFilename( Super.PosTrayLogExportAll() );
	m_LogFileWrite.SetFilename( Super.FileWriteLogNew() );
	m_LogFileWebAPICall.SetFilename( Super.WebAPICallLogNew() );
	m_LogFileAutoDeleteCustLog.SetFilename( Super.AutoDeleteCustLogAll() );
}

/**********************************************************************/

void CMessageLogger::LogSchedulerSystem( CMessageLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.SchedLogAll();
	CString	strOld = Super.SchedLogOld();
	
	CheckMaxLogSysFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Communications Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 300 );
		ReportFile.AddColumn ( "",TA_LEFT, 900 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteHeader();
		ReportFile.Close();
	}

	CString strLine;
	GetSchedulerLogLine( info, strLine );

	if ( info.m_bLeadingLineSystem == TRUE ) m_LogFileScheduler.WriteLine( "<LI>" );
	m_LogFileScheduler.WriteLine( strLine );
	if ( info.m_bFollowingLineSystem == TRUE ) m_LogFileScheduler.WriteLine( "<LI>" );

	m_strLastSystemMessage = strLine;
}

/**********************************************************************/

void CMessageLogger::LogIDraughtSystem( CMessageLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.IDraughtLogAll();
	CString	strOld = Super.IDraughtLogOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "i Draught Export Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 300 );
		ReportFile.AddColumn ( "",TA_LEFT, 900 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteHeader();
		ReportFile.Close();
	}

	CString strLine;
	GetSchedulerLogLine( info, strLine );

	CSSFile fileSched;
	if ( fileSched.Open( strNew, "ab" ) == TRUE )
	{
		if ( info.m_bLeadingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
		fileSched.WriteLine( strLine );
		if ( info.m_bFollowingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
	}
}

/**********************************************************************/

void CMessageLogger::LogFNBSystem( CMessageLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.FNBLogAll();
	CString	strOld = Super.FNBLogOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "FNB Export Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 100 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		info.m_bLeadingLineNetwork = FALSE;
	}

	CString strLine;
	GetSchedulerLogLine( info, strLine );

	CSSFile fileSched;
	if ( fileSched.Open( strNew, "ab" ) == TRUE )
	{
		if ( info.m_bLeadingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
		fileSched.WriteLine( strLine );
		if ( info.m_bFollowingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
	}
}

/**********************************************************************/

void CMessageLogger::LogChartwellsSystem(CMessageLoggerInfo& info)
{
	CreateSyssetSubdirectory("SchedLog");
	CreateSyssetSubdirectory("SchedOld");

	CString	strNew = Super.ChartwellsLogAll();
	CString	strOld = Super.ChartwellsLogOld();
	CheckMaxLogSysFileSize(strNew, strOld);

	if (::FileExists(strNew) == FALSE)
	{
		CReportFile ReportFile;
		ReportFile.Open(strNew);
		ReportFile.SetStyle1("Chartwells Export Log");
		ReportFile.AddColumn("", TA_LEFT, 100);
		ReportFile.AddColumn("", TA_LEFT, 1200);
		ReportFile.AddColumn("", TA_RIGHT, 400);
		ReportFile.AddColumn("", TA_LEFT, 1200);
		ReportFile.WriteHeader();
		ReportFile.Close();
		info.m_bLeadingLineNetwork = FALSE;
	}

	CString strLine = "";
	GetSchedulerLogLine(info, strLine);

	CSSFile fileSched;
	if (fileSched.Open(strNew, "ab") == TRUE)
	{
		if (info.m_bLeadingLineNetwork == TRUE) fileSched.WriteLine("<LI>");
		fileSched.WriteLine(strLine);
		if (info.m_bFollowingLineNetwork == TRUE) fileSched.WriteLine("<LI>");
	}
}

/**********************************************************************/

void CMessageLogger::LogS4LabourSystem( CMessageLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.S4LabourLogAll();
	CString	strOld = Super.S4LabourLogOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "S4 Labour Export Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 100 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		info.m_bLeadingLineNetwork = FALSE;
	}

	CString strLine;
	GetSchedulerLogLine( info, strLine );

	CSSFile fileSched;
	if ( fileSched.Open( strNew, "ab" ) == TRUE )
	{
		if ( info.m_bLeadingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
		fileSched.WriteLine( strLine );
		if ( info.m_bFollowingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
	}
}

/**********************************************************************/

void CMessageLogger::LogFTPSystem( CMessageLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.FTPLogAll();
	CString	strOld = Super.FTPLogOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "FTP Export Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 100 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		info.m_bLeadingLineNetwork = FALSE;
	}

	CString strLine;
	GetSchedulerLogLine( info, strLine );

	CSSFile fileSched;
	if ( fileSched.Open( strNew, "ab" ) == TRUE )
	{
		if ( info.m_bLeadingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
		fileSched.WriteLine( strLine );
		if ( info.m_bFollowingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
	}
}

/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

void CMessageLogger::CheckPosTrayLogFile()
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.PosTrayLogActionAll();
	CString	strOld = Super.PosTrayLogActionOld();
	CheckMaxLogSysFileSize( strNew, strOld );

	if ( ::FileExists ( strNew ) == FALSE )
	{	
		CReportFile ReportFile;
		ReportFile.Open( strNew );
		ReportFile.SetStyle1 ( "PosTray Task Log" );
		ReportFile.AddColumn ( "Event Time",TA_LEFT, 420 );
		ReportFile.AddColumn ( "Task Type",TA_LEFT, 450 );
		ReportFile.AddColumn ( "Task No",TA_LEFT, 200 );
		ReportFile.AddColumn ( "Task Name",TA_LEFT, 500 );
		ReportFile.AddColumn ( "Last Check",TA_LEFT, 420 );
		ReportFile.AddColumn ( "Last Run",TA_LEFT, 420 );
		ReportFile.AddColumn ( "Task Due",TA_LEFT, 420 );
		ReportFile.AddColumn ( "Action",TA_LEFT, 500 );
		ReportFile.AddColumn ( "Result",TA_LEFT, 500 );
		ReportFile.WriteHeader();
		ReportFile.Close();
	}
}

/**********************************************************************/

void CMessageLogger::LogPosTraySystem( CPosTrayEmailScheduleInfo& info, CPosTrayTask& Task, const char* szAction, const char* szResult )
{
	CheckPosTrayLogFile();

	CSSFile fileLog;
	if ( fileLog.Open( Super.PosTrayLogActionAll(), "ab" ) == TRUE )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strTaskType;
		switch( info.m_nTaskType )
		{
		case EMAILTASK_TYPE_PROCESSTRAN:		strTaskType = "PROC_TRAN";	break;
		case EMAILTASK_TYPE_EPOS:				strTaskType = "REP_EPOS";	break;
		case EMAILTASK_TYPE_HIST:				strTaskType = "REP_HIST";	break;
		case EMAILTASK_TYPE_TIMEATTEND:			strTaskType = "REP_TMATT";	break;
		case EMAILTASK_TYPE_STKLEVEL:			strTaskType = "REP_ECRSTK";	break;
		case EMAILTASK_TYPE_EXPORT_SALES:		strTaskType = "EXP_SALE";	break;
		case EMAILTASK_TYPE_EXPORT_PAYMENT:		strTaskType = "EXP_PAYM";	break;
		case EMAILTASK_TYPE_EXPORT_DISCOUNT:	strTaskType = "EXP_DISC";	break;
		case EMAILTASK_TYPE_EXPORT_VOID:		strTaskType = "EXP_VOID";	break;
		default:								strTaskType = "UNKNOWN";	break;
		}

		CString strLastRunTime = "";
		{
			CString strCheck = Task.GetLastRunTime(FALSE);
			if ( strCheck.GetLength() == 12 )
			{
				strLastRunTime.Format( "%s/%s/%s %s:%s",
					(const char*) strCheck.Mid(6,2),
					(const char*) strCheck.Mid(4,2),
					(const char*) strCheck.Left(4),
					(const char*) strCheck.Mid(8,2),
					(const char*) strCheck.Mid(10,2) );
			}
		}

		CCSV csv( '\t' );
		csv.Add( GetPosTrayLogDateTime( timeNow ) );
		csv.Add( strTaskType );
		csv.Add( info.m_nTaskNo );
		csv.Add( info.m_strTaskName );
		csv.Add( GetPosTrayLogDateTime( info.m_timeTaskChecked ) );
		csv.Add( strLastRunTime );
		csv.Add( GetPosTrayLogDateTime( info.m_timeTaskDue ) );
		csv.Add( szAction );
		csv.Add( szResult );
		fileLog.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CMessageLogger::LogPosTrayInitialiseSystem( bool bResult )
{
	CheckPosTrayLogFile();

	CSSFile fileLog;
	if ( fileLog.Open( Super.PosTrayLogActionAll(), "ab" ) == TRUE )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CCSV csv( '\t' );
		csv.Add( GetPosTrayLogDateTime( timeNow ) );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "INIT SYSTEM" );
		csv.Add( bResult ? "OK" : "FAIL" );
		fileLog.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CMessageLogger::LogPosTrayClose( int nResult )
{
	CheckPosTrayLogFile();

	CSSFile fileLog;
	if ( fileLog.Open( Super.PosTrayLogActionAll(), "ab" ) == TRUE )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CCSV csv( '\t' );
		csv.Add( GetPosTrayLogDateTime( timeNow ) );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "CLOSE POSTRAY" );

		switch( nResult )
		{
		case 0:		csv.Add( "NO" );			break;
		case 1:		csv.Add( "YES (User)" );	break;
		case 2:		csv.Add( "YES (System)" );	break;
		default:	csv.Add( "Unknown" );		break;
		}

		fileLog.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

const char* CMessageLogger::GetPosTrayLogDateTime( COleDateTime& datetime )
{
	if ( datetime.m_status == COleDateTime::valid )
	{
		m_strPosTrayLogDateTime.Format( "%2.2d/%2.2d/%4.4d  %2.2d:%2.2d:%2.2d",
			datetime.GetDay(),
			datetime.GetMonth(),
			datetime.GetYear(),
			datetime.GetHour(),
			datetime.GetMinute(),
			datetime.GetSecond() );
	}
	else
	{
		m_strPosTrayLogDateTime = "Invalid";
	}

	return m_strPosTrayLogDateTime;
}

/**********************************************************************/
#endif
/**********************************************************************/

void CMessageLogger::LogSchedulerNetwork( CMessageLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString strNew = dbNetwork.GetFilePathSyssetSchedLogAll( info.m_nNetworkIdx );
	CString	strOld = dbNetwork.GetFilePathSyssetSchedLogOld( info.m_nNetworkIdx );
	CheckMaxLogNwkFileSize( strNew, strOld );

	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Communications Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 300 );
		ReportFile.AddColumn ( "",TA_LEFT, 900 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteHeader();
		ReportFile.Close();
	}

	CString strLine;
	GetSchedulerLogLine( info, strLine );

	CSSFile fileSched;
	if ( fileSched.Open( strNew, "ab" ) == TRUE )
	{
		if ( info.m_bLeadingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
		fileSched.WriteLine( strLine );
		if ( info.m_bFollowingLineNetwork == TRUE ) fileSched.WriteLine( "<LI>" );
	}

	m_strLastNetworkMessage = strLine;
}

/**********************************************************************/

void CMessageLogger::GetSchedulerLogLine( CMessageLoggerInfo& info, CString& strLine )
{
	//TAB SEPARATED STATUS LINE
	//1 ... Network name
	//2 ... Message
	//3 ... Date
	//4 ... Time + Status

	strLine = "";

	CCSV csv('\t');

	if ( info.m_bLogSystemStartStop == FALSE )
	{
		if ( info.m_bAddNetworkName == TRUE )
			csv.Add( dbNetwork.GetName( info.m_nNetworkIdx ) );
		else
			csv.Add( "" );

		csv.Add( info.m_strMessage );
	}
	else
	{
		CString strMsg = "<..>";
		strMsg += info.m_strMessage;

		csv.Add( strMsg );
		csv.Add( "" );
	}

	CTime tm = CTime::GetCurrentTime();
		
	if ( info.m_bAddDate == TRUE )
	{
		CString strDate = tm.Format ( "%a %b %d %Y" );
		csv.Add( strDate );
	}
	else
		csv.Add( "" );

	if ( info.m_bAddTimeOfDay == TRUE )
	{	
		CString strTime = tm.Format ( "%H:%M:%S" );

		if ( info.m_strStatus != "" )
		{
			strTime += " ";
			strTime += info.m_strStatus;
		}

		csv.Add( strTime );
	}
	else
		csv.Add( "" );

	if ( info.m_nHideLevel > 0 )
		strLine.Format ( "<HIDE%1.1d>%s", info.m_nHideLevel, csv.GetLine() );
	else
		strLine = csv.GetLine();
}	

/**********************************************************************/

void CMessageLogger::LogSystem( int nContext, int nType, bool bDate )
{
	CDWordArray dwDummy;
	CStringArray strDummy;
	LogSystem( nContext, nType, bDate, dwDummy, strDummy );
}

/**********************************************************************/

void CMessageLogger::LogSystem( int nContext, int nType, bool bDate, DWORD dwExtra )
{
	CDWordArray dwExtraArray;
	dwExtraArray.Add( dwExtra );
	CStringArray strDummy;
	LogSystem( nContext, nType, bDate, dwExtraArray, strDummy );
}

/**********************************************************************/

void CMessageLogger::LogSystem( int nContext, int nType, bool bDate, const char* szExtra )
{
	CDWordArray dwDummy;
	CStringArray strExtra;
	strExtra.Add( szExtra );
	LogSystem( nContext, nType, bDate, dwDummy, strExtra );
}

/**********************************************************************/

void CMessageLogger::LogSystem( int nContext, int nType, bool bDate, CDWordArray& dwExtra )
{
	CStringArray strDummy;
	LogSystem( nContext, nType, bDate, dwExtra, strDummy );
}

/**********************************************************************/

void CMessageLogger::LogSystem( int nContext, int nType, bool bDate, CStringArray& strExtra )
{
	CDWordArray dwDummy;
	LogSystem( nContext, nType, bDate, dwDummy, strExtra );
}

/**********************************************************************/

void CMessageLogger::LogSystem ( int nContext, int nType, bool bDate, CDWordArray& dwExtra, CStringArray& strExtra )
{
	CreateDataSubdirectory( "SysFiles" );
	
	CTime tm = CTime::GetCurrentTime();
	CString strTime = tm.Format ( "%H%M%S" );
	CString strDate = tm.Format ( "%Y%m%d" );

	//ONLY LOG DATE AT START OF SESSION OR AFTER CHANGE
	if ( ( strDate == m_strLastDate ) && ( bDate == FALSE ) )
		strDate.Format ( "%d", ++m_nLineIndex );
	else
	{
		m_strLastDate = strDate;
		m_nLineIndex = 0;
	}
	
	//CHECK DIGIT IS BASED ON PREVIOUS LINE
	CString strChecksum = "";
	if ( m_strChecksumSeed != "" )
	{
		int nChecksumSeed = atoi( m_strChecksumSeed );

		strChecksum.Format ( "%1.1d%1.1d%1.1d",
				( nChecksumSeed / 3 ) % 10,
				( nChecksumSeed / 7 ) % 10,
				( nChecksumSeed / 9 ) % 10 );
	}

	//PREPARE CHECK DIGIT SEED FOR NEXT LINE
	m_strChecksumSeed.Format ( "%1.1d%1.1d%1.1d%s",
		nContext % 10,
		nType % 10,
		m_nLineIndex % 10,
		(const char*) strTime );

	CCSV csv;
		
	csv.Add( 3 );
	csv.Add( strDate );
	csv.Add( strTime );
	csv.Add( Sysset.GetInstanceNo() );
	csv.Add( strChecksum );
	csv.Add( dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) );
	csv.Add( nContext );
	csv.Add( nType );
	
	for ( int n = 0; n < dwExtra.GetSize(); n++ )
		csv.Add( (int) dwExtra.GetAt(n) );

	for ( int n = 0; n < strExtra.GetSize(); n++ )
		csv.Add( strExtra.GetAt(n) );
	
	CFilenameUpdater FnUpNew( SysFiles::SystemLogNew );
	CString strNew = FnUpNew.GetFilenameToUse();

	CFilenameUpdater FnUpOld( SysFiles::SystemLogOld );
	CString strOld = FnUpOld.GetFilenameToUse();

	CheckMaxLogNwkFileSize( strNew, strOld );

	m_LogFileGrayLog.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CMessageLogger::CheckMaxLogSysFileSize( CString& strNew, CString& strOld )
{
	CFileStatus Status;
	if ( CFile::GetStatus ( strNew, Status ) == TRUE )
	{
		if ( Status.m_size > Super.MaxLogSysFilesize() )	
		{
			CopyFile( strNew, strOld, FALSE );
			CFileRemover FileRemover( strNew );
		}
	}
}

/**********************************************************************/

void CMessageLogger::CheckMaxLogNwkFileSize( CString& strNew, CString& strOld )
{
	CFileStatus Status;
	if ( CFile::GetStatus ( strNew, Status ) == TRUE )
	{
		if ( Status.m_size > Super.MaxLogLocFilesize() )	
		{
			CopyFile( strNew, strOld, FALSE );
			CFileRemover FileRemover( strNew );
		}
	}
}

/**********************************************************************/

void CMessageLogger::LogSchedulerMessage ( CMessageLoggerInfo& info, bool bNetwork, bool bSystem, bool bTemp )
{	
	bool bLogged = FALSE;

	if ( bNetwork == TRUE )
	{
		LogSchedulerNetwork( info );
		bLogged = TRUE;
	}
	
	if ( bSystem == TRUE )
	{
		LogSchedulerSystem( info );
		bLogged = TRUE;
	}

	if ( ( bLogged == FALSE ) || ( bTemp == FALSE ) )
		return;
		
	CFilenameUpdater FnUp ( SysFiles::TempCommsLog );
	CString strFilename = FnUp.GetFilenameToUse();

	CString strMessage;
	if ( bNetwork == TRUE )
		strMessage = GetLastNetworkMessage();
	else
		strMessage = GetLastSystemMessage();

	if ( ::FileExists ( strFilename ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strFilename );
		ReportFile.SetStyle1 ( "Communications Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 300 );
		ReportFile.AddColumn ( "",TA_LEFT, 900 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 1200 );
		ReportFile.WriteLine ( strMessage );
		ReportFile.Close();
	}
	else
	{
		CSSFile fileLog;
		if ( fileLog.Open ( strFilename, "ab" ) == TRUE )
			fileLog.WriteLine ( strMessage );
	}
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void CMessageLogger::LogFTPConnect( int nNetworkIdx, int nSSErrorNo, int nWindowsErrorNo, bool bTemp )
{
	CMessageLoggerInfo info;
	info.m_nNetworkIdx = nNetworkIdx;
	info.m_strMessage = "FTP Connect";
	info.m_strStatus = GetFTPSchedulerErrorText( nSSErrorNo, nWindowsErrorNo );
	info.m_bAddNetworkName = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_nHideLevel = 0;
	LogSchedulerMessage( info, TRUE, TRUE, bTemp );
}

/**********************************************************************/

void CMessageLogger::LogFTPDisconnect( int nNetworkIdx, bool bTemp )
{
	CMessageLoggerInfo info;
	info.m_nNetworkIdx = nNetworkIdx;
	info.m_strMessage = "FTP Disconnect";
	info.m_strStatus = GetFTPSchedulerErrorText( COMMS_ERROR_NONE, 0 );
	info.m_bAddNetworkName = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_nHideLevel = 0;
	LogSchedulerMessage( info, TRUE, TRUE, bTemp );
}

/**********************************************************************/

void CMessageLogger::LogFTPCommsResult( CJobListCSVRecord& JobInfo, int nSSErrorNo, int nWindowsErrorNo, const char* szExtraInfo, bool bTemp )
{
	CString strExtra = szExtraInfo;
	if ( strExtra != "" )
		strExtra.Format( " (%s)", szExtraInfo );

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = JobInfo.GetNetworkIdx();
	info.m_strMessage = JobInfo.GetLogLine();
	info.m_strStatus = GetFTPSchedulerErrorText( nSSErrorNo, nWindowsErrorNo );
	info.m_strStatus += strExtra;
	info.m_bAddNetworkName = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_nHideLevel = 0;
	LogSchedulerMessage( info, TRUE, TRUE, bTemp );
}

/**********************************************************************/

void CMessageLogger::LogFTPFileTransfer( const char* szSource, const char* szDest, bool bTemp )
{
	CMessageLoggerInfo info;
	info.m_nNetworkIdx = 0;
	info.m_strMessage = "";
	info.m_strMessage += szSource;
	info.m_strMessage += " => " ;
	info.m_strMessage += szDest;
	info.m_strStatus = "";
	info.m_bAddNetworkName = FALSE;
	info.m_bAddTimeOfDay = FALSE;
	info.m_nHideLevel = 0;
	LogSchedulerMessage( info, TRUE, TRUE, bTemp );
}

/**********************************************************************/

const char* CMessageLogger::GetFTPSchedulerErrorText( int nSSErrorNo, int nWindowsErrorNo )
{
	m_strFTPSchedulerErrorText = GetFTPSimpleErrorText( nSSErrorNo );

	switch( nSSErrorNo )
	{	
	case COMMS_ERROR_FTP_FILE_PUT:
	case COMMS_ERROR_FTP_CONNECT:
		{
			CString strExtra;
			strExtra.Format( " (%d)", nWindowsErrorNo );
			m_strFTPSchedulerErrorText += strExtra;
		}
		break;
	}

	return m_strFTPSchedulerErrorText;
}

/**********************************************************************/

const char* CMessageLogger::GetFTPSimpleErrorText( int nError )
{
	switch ( nError )
	{
	case COMMS_ERROR_NONE:				
		m_strFTPSimpleErrorText = "Ok";
		break;

	case COMMS_ERROR_FTP_CONNECT:
		m_strFTPSimpleErrorText = "Unable to establish FTP connection";
		break;

	case COMMS_ERROR_FTP_FOLDER_DIR:
		m_strFTPSimpleErrorText = "Unable to read FTP directory";
		break;

	case COMMS_ERROR_FTP_FOLDER_CHANGE:
		m_strFTPSimpleErrorText = "Unable to change FTP directory";
		break;

	case COMMS_ERROR_FTP_FOLDER_DEST:
		m_strFTPSimpleErrorText = "Unable to open FTP sales pending folder";
		break;

	case COMMS_ERROR_FTP_FILE_PUT:
		m_strFTPSimpleErrorText = "Unable to send file over FTP";
		break;

	case COMMS_ERROR_FTP_FILE_GET:
		m_strFTPSimpleErrorText = "Unable to receive file over FTP";
		break;

	case COMMS_ERROR_FTP_FILE_RENAME:
		m_strFTPSimpleErrorText = "Unable to rename file over FTP";
		break;

	case COMMS_ERROR_FTP_FILE_DELETE:
		m_strFTPSimpleErrorText = "Unable to delete file over FTP";
		break;

	case COMMS_ERROR_FTP_FILE_AUTOQUIT:	
		m_strFTPSimpleErrorText = "Too many FTP transfer errors";
		break;

	case COMMS_ERROR_FTP_FILE_TIMEOUT:
		m_strFTPSimpleErrorText = "FTP transfer time out";
		break;

	case COMMS_ERROR_FTP_FILE_FAIL:
		m_strFTPSimpleErrorText = "Fail";
		break;

	default:						
		m_strFTPSimpleErrorText = "Unknown";
		break;
	}

	return m_strFTPSimpleErrorText;
}

/**********************************************************************/
#endif
/**********************************************************************/

void CMessageLogger::LogEmailSystem( CEmailLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString strNew = Super.EmailLogAll();
	CString strOld = Super.EmailLogOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	bool bNewFile = FALSE;
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Email Log" );
		ReportFile.AddColumn ( "Date",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Time",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Filename",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Subject",TA_LEFT, 1500 );
		ReportFile.AddColumn ( "Address",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Result",TA_LEFT, 700 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		bNewFile = TRUE;
	}

	CString strLine = GetEmailLogLine( info );
	m_LogFileEmail.WriteLine( strLine );
	m_strLastEmailMessage = strLine;
}

/**********************************************************************/

const char* CMessageLogger::GetEmailLogLine( CEmailLoggerInfo& info )
{	
	CCSV csv('\t');
		
	csv.Add( info.m_strDate );
	csv.Add( info.m_strTime );
	csv.Add( info.m_strFilename );
	csv.Add( info.m_strSubject );
	csv.Add( info.m_strEmail );
	csv.Add( info.m_strResult );

	if ( info.m_nHideLevel > 0 )
		m_strEmailLogLine.Format ( "<HIDE%1.1d>%s", info.m_nHideLevel, csv.GetLine() );
	else
		m_strEmailLogLine = csv.GetLine();

	return m_strEmailLogLine;
}	

/**********************************************************************/

void CMessageLogger::LogExportSystem( CExportLoggerInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString strNew = Super.PosTrayLogExportAll();
	CString strOld = Super.PosTrayLogExportOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	bool bNewFile = FALSE;
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "PosTray Export Log" );
		ReportFile.AddColumn ( "Date",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Time",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Type",TA_LEFT, 500 );
		ReportFile.AddColumn ( "Target Folder",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Target File",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Result",TA_LEFT, 700 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		bNewFile = TRUE;
	}

	CString strLine = GetExportLogLine( info );
	m_LogFileExport.WriteLine( strLine );
	m_strLastExportMessage = strLine;
}

/**********************************************************************/

const char* CMessageLogger::GetExportLogLine( CExportLoggerInfo& info )
{	
	CCSV csv('\t');
		
	csv.Add( info.m_strDate );
	csv.Add( info.m_strTime );
	csv.Add( info.m_strType );
	csv.Add( info.m_strTargetFolder );
	csv.Add( info.m_strTargetFile );
	csv.Add( info.m_strResult );

	if ( info.m_nHideLevel > 0 )
		m_strExportLogLine.Format ( "<HIDE%1.1d>%s", info.m_nHideLevel, csv.GetLine() );
	else
		m_strExportLogLine = csv.GetLine();

	return m_strExportLogLine;
}	

/**********************************************************************/

void CMessageLogger::LogFileWrite( const char* szFilename, int nResult, int nLastError )
{
	CString strFilename = szFilename;
	strFilename.MakeUpper();

	{
		CString strProgram = SysInfo.IsStockSystem() ? "P43" : "P42";

		int nPos = strFilename.Find( strProgram );
		if ( nPos >= 1 )
			strFilename = strFilename.Right( strFilename.GetLength() - nPos );
	}

	CString strLogFileNew = Super.FileWriteLogNew();
	CString strLogFileOld = Super.FileWriteLogOld();
	
	{
		CFileStatus Status;
		if ( CFile::GetStatus ( strLogFileNew, Status ) == TRUE )
		{
			if ( Status.m_size > 2048000 )	
			{
				CopyFile( strLogFileNew, strLogFileOld, FALSE );
				CFileRemover FileRemover( strLogFileNew );
			}
		}
	}
	
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay() );

	CString strTime;
	strTime.Format( "%2.2d%2.2d%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	int nPasswordIdx = 0;
	if ( PasswordArray.IsMasterPassword() == FALSE )
		nPasswordIdx = PasswordArray.GetPasswordIdx();

#ifdef POSTRAY_UTILITY
	nPasswordIdx = 99;
#endif

	CCSV csv;
	csv.Add( 1 );
	csv.Add( nPasswordIdx );
	csv.Add( strDate );
	csv.Add( strTime );
	csv.Add( nResult );

	if ( 1005 == nResult )
		csv.Add( nLastError );
	else
		csv.Add( "" );

	csv.Add( strFilename );

	m_LogFileWrite.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CMessageLogger::LogWebAPICall( const char* szCall )
{
	CString strLogFileNew = Super.WebAPICallLogNew();
	CString strLogFileOld = Super.WebAPICallLogOld();
	
	{
		CFileStatus Status;
		if ( CFile::GetStatus ( strLogFileNew, Status ) == TRUE )
		{
			if ( Status.m_size > 2048000 )	
			{
				CopyFile( strLogFileNew, strLogFileOld, FALSE );
				CFileRemover FileRemover( strLogFileNew );
			}
		}
	}
	
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay() );

	CString strTime;
	strTime.Format( "%2.2d%2.2d%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	int nPasswordIdx = 0;
	if ( PasswordArray.IsMasterPassword() == FALSE )
		nPasswordIdx = PasswordArray.GetPasswordIdx();

#ifdef POSTRAY_UTILITY
	nPasswordIdx = 99;
#endif

	CCSV csv;
	csv.Add( 1 );
	csv.Add( nPasswordIdx );
	csv.Add( strDate );
	csv.Add( strTime );
	csv.Add( szCall );

	m_LogFileWebAPICall.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CMessageLogger::LogAutoDeleteCustomer( CAutoDeleteCustLogInfo& info )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString strNew = Super.AutoDeleteCustLogAll();
	CString strOld = Super.AutoDeleteCustLogOld();
	CheckMaxLogSysFileSize( strNew, strOld );
	
	bool bNewFile = FALSE;
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Auto Delete Customer Log" );
		ReportFile.AddColumn ( "Date",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Time",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Location",TA_LEFT, 500 );
		ReportFile.AddColumn ( "Customer Type",TA_LEFT, 400 );
		ReportFile.AddColumn ( "Info",TA_LEFT, 1500 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		bNewFile = TRUE;
	}

	CString strLine = GetAutoDeleteCustomerLogLine( info );

	if ( ( strLine != "<LI>" ) || ( m_strLastAutoDeleteCustLogMessage != "<LI>" ) )
	{
		m_LogFileAutoDeleteCustLog.WriteLine( strLine );
		m_strLastAutoDeleteCustLogMessage = strLine;
	}
}

/**********************************************************************/

const char* CMessageLogger::GetAutoDeleteCustomerLogLine( CAutoDeleteCustLogInfo& info )
{	
	if ( info.m_strInfo == "<LI>" )
	{
		m_strAutoDeleteCustomerLogLine = "<LI>";
	}
	else
	{
		CCSV csv('\t');
			
		csv.Add( info.m_strDate );
		csv.Add( info.m_strTime );

		if ( info.m_nLocIdx >= 0 )
		{
			csv.Add( dbLocation.GetName( info.m_nLocIdx ) );
			csv.Add( info.m_bOneOff ? "One-Off" : "Account" );
		}
		else
		{
			csv.Add( "" );
			csv.Add( "" );
		}

		csv.Add( info.m_strInfo );

		if ( info.m_nHideLevel > 0 )
			m_strAutoDeleteCustomerLogLine.Format ( "<HIDE%1.1d>%s", info.m_nHideLevel, csv.GetLine() );
		else
			m_strAutoDeleteCustomerLogLine = csv.GetLine();
	}

	return m_strAutoDeleteCustomerLogLine;
}	

/**********************************************************************/
