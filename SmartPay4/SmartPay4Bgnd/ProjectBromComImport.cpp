//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\BromComImport.h"
//*******************************************************************

void CBromComImport::LogMessage ( CString strMsg  )
{
	return MessageLogger.LogBromComMessage( strMsg );
}

//******************************************************************

void CBromComImport::DisplayImportResults( int nRecordCount )
{
	// sort out WebPayment list file & update main pending files if required
	if ( m_nValidPaymentCount != 0 )								
	{
		CString strFile;											// master import log file
		strFile.Format ( "%s.dat", (const char*) m_strLogFileName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//******************************************************************
