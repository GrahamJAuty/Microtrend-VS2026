//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
#include "..\SmartPay4ManagerBgnd\SquidImport.h"
//*******************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//*******************************************************************

void CSquidImport::DisplayImportResults( int nRecordCount )
{
	// sort out WebPayment list file & update main pending files if required
	if ( m_nValidCount != 0 )								
	{
		CString strFile;	
		strFile.Format ( "%s.dat", m_strLogFileName );		// master import log file
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//*******************************************************************

void CSquidImport::LogMessage( CString strMsg )
{
	MessageLogger.LogSQuidMessage( strMsg );
}

//******************************************************************
