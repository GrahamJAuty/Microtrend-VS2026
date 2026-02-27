//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\TucasiJSON.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
#include "..\SmartPay4ManagerBgnd\TucasiJsonImport.h"
//*******************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//*******************************************************************

void CTucasiJsonImport::LogMessage ( CString strMsg  )
{
	MessageLogger.LogTucasiJSONMessage( strMsg );
}

//******************************************************************

void CTucasiJsonImport::DisplayImportResults( int nRecordCount )
{
	if ( m_nValidCount != 0 )								// check for any valid imports
	{		
		CString strFile;										// master import log file
		strFile.Format ( "%s.dat", m_strLogFileName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//******************************************************************
