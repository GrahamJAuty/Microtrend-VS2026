//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\TucasiSOAPImport.h"
//*******************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//*******************************************************************

void CTucasiSoapImport::DisplayImportResults( int nRecordCount )
{
	if ( m_nValidCount != 0 )								// check for any valid imports
	{
		CString strFile;										// master import log file
		strFile.Format ( "%s.dat", m_strLogFileName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//*******************************************************************

void CTucasiSoapImport::LogMessage ( CString strMsg  )
{
	MessageLogger.LogTucasiSOAPMessage( strMsg );
}

//******************************************************************
