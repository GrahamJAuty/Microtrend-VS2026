//*******************************************************************
#include "FileRecycler.h"
#include "GlobalFunctions.h"
#include "ServerData.h"
#include "GlobalState.h"
//*******************************************************************
#include "MessageLogger.h"
//*******************************************************************

CMessageLogger::CMessageLogger(void)
{
	m_strLastMemberMojoMessage = "";
	m_lastMemberMojoTime = COleDateTime::GetCurrentTime();
}

//*******************************************************************

CMessageLogger::~CMessageLogger(void)
{
}

//*******************************************************************

void CMessageLogger::CheckMaxLogFileSize( CString& strNew, CString& strOld )
{
	CFileStatus Status;
	if ( CFile::GetStatus ( strNew, Status ) == TRUE )
	{
		if ( Status.m_size > 0x200000 )	
		{
			CopyFile( strNew, strOld, FALSE );
			CFileRecycler FileRecycler;
			FileRecycler.SendToRecycleBin( strNew );
		}
	}
}

/**********************************************************************/

void CMessageLogger::LogMemberMojoImportMessage(CString strMessage, bool bForce)
{
	bool bCanLog = TRUE;
	if ( (strMessage == m_strLastMemberMojoMessage) && ( FALSE == bForce ) )
	{
		if (COleDateTime::GetCurrentTime() < m_lastMemberMojoTime + COleDateTimeSpan(0, 0, 5, 0))
		{
			bCanLog = FALSE;
		}
	}

	if (TRUE == bCanLog)
	{
		LogSystemMessage(strMessage);
		m_strLastMemberMojoMessage = strMessage;
		m_lastMemberMojoTime = COleDateTime::GetCurrentTime();
	}
}

/**********************************************************************/

void CMessageLogger::LogSystemMessage( CString strMessage )
{
	LogMessage( "System", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogWinServiceMessage( CString strMessage )
{
	LogMessage( "Service Control", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogMessage( CString strContext, CString strMessage )
{
#ifdef LOYSERVER
	if ( TRUE == g_GlobalState.IsServerDesktopApp() )
	{
		strContext += " (DA)";
	}
	else
	{
		strContext += " (SV)";
	}
#endif

	CString strFolder = Filenames.GetServerLogFolder();
	CreateSubdirectory( strFolder );

	CString strNew = Filenames.GetServerLogFilename();
	CString strOld = Filenames.GetServerOldFilename();

	CheckMaxLogFileSize(strNew, strOld);

	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Loyalty Server Log" );
		ReportFile.AddColumn ( "Date",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Time",TA_LEFT, 250 );
		ReportFile.AddColumn ( "PC ID",TA_LEFT, 300 );
		ReportFile.AddColumn ( "Context",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Info",TA_LEFT, 1500 );
		ReportFile.WriteHeader();
		ReportFile.Close();
	}

	CString strDate = "";
	CString strTime = "";
	SolutionGlobalFunctions::GetMessageLogDateTime(strDate, strTime);

	CString strPCIdClient = "";
	CString strPCIdHost = "";
	strPCIdClient.Format( "%10.10d", Sysset.GetPCIdClient() );
	strPCIdHost.Format("%10.10d", Sysset.GetPCIdHost());

	CString strPCID = strPCIdClient;
	if ( strPCIdClient != strPCIdHost )
	{
		strPCID += CString("/") + strPCIdHost;
	}
	
	CCSV csv( '\t' );
	csv.Add( strDate );
	csv.Add( strTime );
	csv.Add( strPCID );
	csv.Add( strContext );
	csv.Add( strMessage );

	CSSFile fileLog;
	if ( fileLog.Open( strNew, "ab" ) == TRUE )
	{
		fileLog.WriteLine( csv.GetLine() );
	}
}

//*******************************************************************

void CMessageLogger::LogThreadDiagnostic(int nType, int DbNo, int nFolderSet)
{
	//INTENTIONAL USE OF BITWISE OPERATOR
	if ((Server.GetThreadDiagnosticsFlags() & 1 ) == 0)
	{
		return;
	}

	CString strFilename = "";
	
	switch (nType)
	{
	case THREADLOG_REQIN:
		strFilename = "THREADLOYREQIN";
		break;

	case THREADLOG_REQOUT:
		strFilename = "THREADLOYREQOUT";
		break;

	case THREADLOG_RSPIN:
		strFilename = "THREADLOYRSPIN";
		break;

	case THREADLOG_RSPOUT:
		strFilename = "THREADLOYRSPOUT";
		break;

	case THREADLOG_LOGIN:
		strFilename = "THREADLOYLOGIN";
		break;

	case THREADLOG_LOGOUT:
		strFilename = "THREADLOYLOGOUT";
		break;

	case THREADLOG_REQTIDYIN:
		strFilename = "THREADLOYREQTIDYIN";
		break;

	case THREADLOG_REQTIDYOUT:
		strFilename = "THREADLOYREQTIDYOUT";
		break;
	}

	if (strFilename == "")
	{
		return;
	}

	CString strFolder = Filenames.GetServerLogFolder();
	CreateSubdirectory(strFolder);

	CString strFilepath = "";
	strFilepath.Format("%s\\%s_%2.2d_%2.2d.dat", 
		(const char*) strFolder, 
		(const char*) strFilename,
		DbNo, 
		nFolderSet);

	CSSFile fileLog;
	if (fileLog.Open(strFilepath, "wb") == TRUE)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		
		CString strTime = "";
		strTime.Format("%2.2d/%2.2d/%4.4d  %2.2d:%2.2d:%2.2d",
			timeNow.GetDay(),
			timeNow.GetMonth(),
			timeNow.GetYear(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond());

		fileLog.WriteLine(strTime);	
	}
}

//*******************************************************************
