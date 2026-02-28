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
	CString strFolder = Filenames.GetServerLogFolder();
	CreateSubdirectory( strFolder );

	CString strNew = Filenames.GetTrayLogFilename();
	
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
	strPCIdClient.Format( "%10.10d", Sysset.GetPCIdClient() );

	CString strPCIdHost = "";
	strPCIdHost.Format("%10.10d", Sysset.GetPCIdHost());

	CString strPCId = strPCIdClient;

	if ( Sysset.GetPCIdClient() != Sysset.GetPCIdHost() )
	{
		strPCId += "/";
		strPCId += strPCIdHost;
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
