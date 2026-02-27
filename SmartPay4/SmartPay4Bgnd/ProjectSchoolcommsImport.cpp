//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SchoolcommsImport.h"
//*******************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//*******************************************************************

void CSchoolcommsImport::LogMessage ( CString strMsg  )
{
	return MessageLogger.LogSchoolCommsMessage( strMsg );
}

//******************************************************************

void CSchoolcommsImport::DisplayImportResults( int nRecordCount )
{
	// sort out WebPayment list file & update main pending files if required
	if ( m_nValidCount != 0 )								
	{
		CString strFile;											// master import log file
		strFile.Format ( "%s.dat", m_strLogFileName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//******************************************************************
