// Save CSV web payment export logs - log file has header
//
// Tayyyymm.dat - account echo log
// Teyyyymm.dat - payment echo log
// Tzyyyymm.dat - balance export log
//
//*******************************************************************

//#include "SystemData.h"
//#include "GlobalFunctions.h"
#include "WebPaymentExportCsvLog.h"

//*******************************************************************

CWebPaymentExportCsvLog::CWebPaymentExportCsvLog()
{
}

//*******************************************************************
// File has header line

bool CWebPaymentExportCsvLog::SaveLog ( const char* szPrefix, const char* szUpdateFile )
{
	int nLineCount = ::GetFileLineCount ( szUpdateFile ) - 1;	// skip header
	return AppendLog ( szPrefix, szUpdateFile, nLineCount );
}

bool CWebPaymentExportCsvLog::SaveLog ( const char* szPrefix , const char* szUpdateFile, int nLineCount )
{
	return AppendLog ( szPrefix, szUpdateFile, nLineCount );
}

//*******************************************************************

bool CWebPaymentExportCsvLog::AppendLog ( const char* szPrefix, const char* szUpdateFile, int nLineCount )
{
	bool bReply = TRUE;

	if ( nLineCount > 0 )										// don't log empty update file
	{
		CSSDate date;			date.SetCurrentDate();
		CString strFilename;	strFilename.Format ( "%s%d%2.2d.dat", szPrefix, date.GetYear(), date.GetMonth() );		// Txyyyymm / Tayyyymm / Teyyyymm

		CString strLogFile = System.GetWebPaymentPath ( strFilename );

		if ( ( bReply = AppendDataFile ( szUpdateFile, strLogFile, TRUE ) ) == TRUE )			//append files with header
		{
			CSSTime time;		time.SetCurrentTime();
			CString strLine;	strLine.Format ( "Collated %s @ %2.2d:%2.2d:%2.2d : Lines %d", date.GetDate(), time.GetHour(), time.GetMinute(), time.GetSecond(), nLineCount );

			CSSFile fileOut;
			fileOut.Open ( strLogFile, "ab" );
			fileOut.WriteLine ( strLine );
			fileOut.WriteLine ( "" );
			fileOut.Close();
		}
	}
	return bReply;
}

//*******************************************************************

int CWebPaymentExportCsvLog::StripLog ( const char* szSource, const char* szDestination )
{
	int nCount = -1;

	CSSFile fileIn;
	if ( fileIn.Open ( szSource, "rb" ) == TRUE )
	{
		CSSFile fileOut;
		if ( fileOut.Open ( szDestination, "wb" ) == TRUE )
		{
			nCount = 0;

			CString strBuf;
			fileIn.ReadLine ( strBuf );						// read header
			fileOut.WriteString ( strBuf );					// save header
	
			while ( fileIn.ReadLine ( strBuf ) == TRUE )
			{
				int nLength = strBuf.GetLength();
				if ( nLength > 3 )							// skip blank line
				{
					if ( strBuf.Left(8) != "Collated" )		// skip text lines
					{
						fileOut.WriteString ( strBuf );		// save log line
						++nCount;
					}
				}
			}

			fileOut.Close();
		}
		fileIn.Close();
	}

	return nCount;
}

//*******************************************************************

