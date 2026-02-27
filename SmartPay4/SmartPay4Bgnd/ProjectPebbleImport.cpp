//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\PebbleImport.h"
//*******************************************************************

void CPebbleImport::LogMessage ( CString strMsg  )
{
	return MessageLogger.LogPebbleMessage( strMsg );
}

//******************************************************************

void CPebbleImport::DisplayImportResults(CSQLTranPebbleImportStage2& Tran )
{
	// sort out WebPayment list file & update main pending files if required
	if ( m_nValidPaymentCount != 0 )								
	{
		CString strFile = ""; // master import log file
		strFile.Format ( "%s.dat", (const char*) Tran.GetLogFileName() );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( Tran.GetTempLogFile(), strFile );
	}
}

//******************************************************************
