//*******************************************************************
#include "GlobalFunctions.h"
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

void CMessageLogger::LogSystemMessage( CString strMessage )
{
	LogMessage( "TrayIcon", strMessage );
}

/**********************************************************************/

void CMessageLogger::LogMessage( CString strContext, CString strMessage )
{
#ifdef BGNDICON
	CString strFolder = Filenames.GetBackgroundLogFolder();
#else
	CString strFolder = Filenames.GetServerLogFolder();
#endif
	
	CreateSubdirectory( strFolder );

	CString strNew = Filenames.GetTrayLogFilename();
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );

#ifdef BGNDICON
		ReportFile.SetStyle1 ( "SmartPay Background TrayIcon Log" );
#else
		ReportFile.SetStyle1 ( "SmartPay Server TrayIcon Log");
#endif

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
