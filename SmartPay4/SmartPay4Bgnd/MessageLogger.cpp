//*******************************************************************
#include "..\SmartPay4Shared\GlobalState.h"
//*******************************************************************
#include "MessageLogger.h"
//*******************************************************************

CMessageLogger::CMessageLogger(void)
{
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

void CMessageLogger::LogSystemMessage( CString strMessage )
{
	LogMessage( "System", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogImportDatabaseMessage( CString strMessage )
{
	LogMessage( "Import Database", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogWinServiceMessage( CString strMessage )
{
	LogMessage( "Service Control", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogParentMailMessage( CString strMessage )
{
	LogMessage( "Parent Mail", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogParentPayMessage( CString strMessage )
{
	LogMessage( "Parent Pay", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogSchoolCommsMessage( CString strMessage )
{
	LogMessage( "School Comms", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogBromComMessage(CString strMessage)
{
	LogMessage("BromCom", strMessage);
}

/**********************************************************************/

void CMessageLogger::LogPebbleMessage(CString strMessage)
{
	LogMessage("Pebble", strMessage);
}

/**********************************************************************/

void CMessageLogger::LogSQuidMessage( CString strMessage )
{
	LogMessage( "sQuid", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogTucasiJSONMessage( CString strMessage )
{
	LogMessage( "Tucasi JSON", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogTucasiSOAPMessage( CString strMessage )
{
	LogMessage( "Tucasi SOAP", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogWisePayMessage( CString strMessage )
{
	LogMessage( "WisePay", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogMessage( CString strContext, CString strMessage )
{
#ifdef SYSTEMTYPE_BACKGROUND
	if (TRUE == g_GlobalState.IsServerDesktopApp())
	{
		strContext += " (DA)";
	}
	else
	{
		strContext += " (SV)";
	}
#endif

	CString strFolder = Filenames.GetBackgroundLogFolder();
	CreateSubdirectory( strFolder );

	CString strNew = Filenames.GetBackgroundLogFilename();
	CString strOld = Filenames.GetBackgroundOldFilename();
	CheckMaxLogFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "SmartPay Background Processor Log" );
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
	GetMessageLogDateTime(strDate, strTime);

	CString strPCIdClient = "";
	CString strPCIdHost = "";
	strPCIdClient.Format("%10.10d", Sysset.GetPCIdClient());
	strPCIdHost.Format("%10.10d", Sysset.GetPCIdHost());

	CString strPCId = strPCIdClient;
	if (strPCIdClient != strPCIdHost)
	{
		strPCId += CString("/") + strPCIdHost;
	}
	
	CCSV csv( '\t' );
	csv.Add( strDate );
	csv.Add( strTime );
	csv.Add( strPCId );
	csv.Add( strContext );
	csv.Add( strMessage );

	CSSFile fileLog;
	if ( fileLog.Open( strNew, "ab" ) == TRUE )
	{
		fileLog.WriteLine( csv.GetLine() );
	}
}

//*******************************************************************
