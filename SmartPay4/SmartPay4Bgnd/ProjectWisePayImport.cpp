//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayImport.h"
//*******************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//*******************************************************************

void CWisePayImport::DisplayImportResults( int nRecordCount, int nAccountID )
{		
	if ( m_nValidCount != 0 )
	{
		CString strFile;											// master import log file
		strFile.Format ( "%s.dat", m_strLogFileName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//*******************************************************************

void CWisePayImport::LogMessage( CString strMsg  )
{
	MessageLogger.LogWisePayMessage( strMsg );
}

//******************************************************************

