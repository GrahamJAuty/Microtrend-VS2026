/**********************************************************************/
#include "SPOSKeyboardMessageLogger.h"
/**********************************************************************/

CSPOSKeyboardMessageLogger::CSPOSKeyboardMessageLogger() 
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardMessageLogger::Reset()
{
	m_LogFileWrite.SetFilename( "" );
	m_nUserID = 0;
}

/**********************************************************************/

void CSPOSKeyboardMessageLogger::Initialise( const char* szFilename, bool bStockman, int nUserID )
{
	m_LogFileWrite.SetFilename( szFilename );
	m_nUserID = nUserID;
}

/**********************************************************************/

void CSPOSKeyboardMessageLogger::LogFileWrite( const char* szFilename, int nResult, int nLastError )
{
	CString strFilename = szFilename;
	strFilename.MakeUpper();

	{
		CString strProgram = m_bStockman ? "P43" : "P42";

		int nPos = strFilename.Find( strProgram );
		if ( nPos >= 1 )
			strFilename = strFilename.Right( strFilename.GetLength() - nPos );
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

	CCSV csv;
	csv.Add( 1 );
	csv.Add( m_nUserID );
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
