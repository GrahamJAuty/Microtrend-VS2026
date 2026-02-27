//*******************************************************************
// Monthly WebPayment log of Transactions \ Purchases sent to web site
//*******************************************************************

//#include "SystemData.h"
//#include "GlobalFunctions.h"
#include "WebPaymentExportTnLog.h"

//*******************************************************************

CWebPaymentExportTnLog::CWebPaymentExportTnLog ( int nWebPaymentType, const char* szPrefix )
{
	m_date.SetCurrentDate();

	CString strName;	strName.Format ( "%s%d%2.2d.dat", szPrefix,  m_date.GetYear(), m_date.GetMonth() );  // Tnyyyymm.dat or Toyyyymm.dat

	m_strLogFilename = System.GetWebPaymentPath ( strName, nWebPaymentType );
}

//*******************************************************************

void CWebPaymentExportTnLog::AddDateStamp ( const char* szLogFile, int nLineCount )
{
	if ( nLineCount < 1 )
		return;

	CSSTime time;		time.SetCurrentTime();
	CString strLine;	strLine.Format ( "Collated %s @ %2.2d:%2.2d:%2.2d : Lines %d", m_date.GetDate(), time.GetHour(), time.GetMinute(), time.GetSecond(), nLineCount );

	CSSFile fileOut;
	fileOut.Open ( szLogFile, "ab" );
	fileOut.WriteLine ( strLine );
	fileOut.WriteLine ( "" );
	fileOut.Close();
}

//*******************************************************************
// Log the new updates - Update file may\may not have no header line - but fields are in szHeader order

bool CWebPaymentExportTnLog::SaveLog ( const char* szUpdatesFile, const char* szHeader )
{
	bool bReply			= TRUE;								// assume all ok
	CString strHeader	= szHeader;
	bool bFileHasHeader = ( strHeader == "" ) ? TRUE : FALSE;
	int nLineCount		= ::GetFileLineCount ( szUpdatesFile );

	if ( bFileHasHeader == FALSE )							// if starting a new log file & the update file has no header
	{														// need to add header to log file
		if ( ::GetFileLineCount ( m_strLogFilename ) < 1 )	// current ..\{WebPayment}\TN... log
		{
			CSSFile fileOut;
			fileOut.Open ( m_strLogFilename, "wb" );
			fileOut.WriteLine ( szHeader );					// add header
			fileOut.Close();
		}
	}
	else
		--nLineCount;										// ignore header in update file line count

	if ( nLineCount > 0 )									// don't log empty update file
	{
		if ( ( bReply = AppendDataFile ( szUpdatesFile, m_strLogFilename, bFileHasHeader ) ) == TRUE )
			AddDateStamp ( m_strLogFilename, nLineCount );
	}

	return bReply;
}

//**********************************************************************
