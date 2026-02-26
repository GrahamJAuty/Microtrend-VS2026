/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporterDefines.h"
 
#include "RepCSV.h"
/**********************************************************************/
#include "CashRSPVersionChecker.h"
/**********************************************************************/

CCashRSPVersionChecker::CCashRSPVersionChecker(void)
{
	Reset();
}

/**********************************************************************/

CCashRSPVersionChecker::~CCashRSPVersionChecker(void)
{
}

/**********************************************************************/

void CCashRSPVersionChecker::Reset()
{
	m_strPendingTransactionVersionLine = "";
	m_strCurrentTransactionVersionLine = "";
	m_strPendingTransactionAppLine = "";
	m_strCurrentTransactionAppLine = "";
	m_nPendingTransactionVersionNo = 0;
	m_nCurrentTransactionVersionNo = 0;
	m_nCurrentTransactionAppType = 1;
	m_nPendingTransactionAppType = 1;
	m_nFailedCashRSPVersion = 0;
	m_nFailedAppType = 0;
}

/**********************************************************************/

void CCashRSPVersionChecker::ResetTransactionVersions()
{
	m_nPendingTransactionVersionNo = 0;
	m_nCurrentTransactionVersionNo = 0;
	m_strPendingTransactionVersionLine = "";
	m_strCurrentTransactionVersionLine = "";
}

/**********************************************************************/

void CCashRSPVersionChecker::ResetApplicationTypes()
{
	m_nPendingTransactionAppType = 1;
	m_nCurrentTransactionAppType = 1;
	m_strPendingTransactionAppLine = "";
	m_strCurrentTransactionAppLine = "";
}

/**********************************************************************/

void CCashRSPVersionChecker::ResetPendingTransactionInfo()
{
	m_nPendingTransactionVersionNo = 0;
	m_nPendingTransactionAppType = 1;
	m_strPendingTransactionVersionLine = "";
	m_strPendingTransactionAppLine = "";
}

/**********************************************************************/

void CCashRSPVersionChecker::ResetCurrentLines()
{
	m_strCurrentTransactionVersionLine = "";
	m_strCurrentTransactionAppLine = "";
}

/**********************************************************************/

void CCashRSPVersionChecker::ForceSetVersionInfo( int nAppType, int nVersionNo )
{
	m_nCurrentTransactionAppType = nAppType;
	m_nCurrentTransactionVersionNo = nVersionNo;
}

/**********************************************************************/

void CCashRSPVersionChecker::ApplyPendingTransactionInfo()
{
	m_nCurrentTransactionVersionNo = m_nPendingTransactionVersionNo;
	m_nPendingTransactionVersionNo = 0;
	
	m_strCurrentTransactionVersionLine = m_strPendingTransactionVersionLine;
	m_strPendingTransactionVersionLine = "";
	
	m_nCurrentTransactionAppType = m_nPendingTransactionAppType;
	m_nPendingTransactionAppType = 1;

	m_strCurrentTransactionAppLine = m_strPendingTransactionAppLine;
	m_strPendingTransactionAppLine = "";
}

/**********************************************************************/

void CCashRSPVersionChecker::ProcessCashRSPVersionLine( CCSVEposTermLine& csvIn )
{
	m_nPendingTransactionVersionNo = csvIn.GetVersionNo();
	
	if ( ValidateCashRSPVersion( m_nPendingTransactionVersionNo ) == TRUE )
		m_nFailedCashRSPVersion = 0;
	else
		m_nFailedCashRSPVersion = m_nPendingTransactionVersionNo;
}

/**********************************************************************/

void CCashRSPVersionChecker::ProcessCashRSPVersionLine( CCSVEposTermLine& csvIn, CString& strBuffer )
{
	m_strPendingTransactionVersionLine = strBuffer;
	ProcessCashRSPVersionLine( csvIn );
}

/**********************************************************************/

bool CCashRSPVersionChecker::ValidateCashRSPVersion( int nVersion )
{
	return CheckCashRSPVersion( nVersion, 14, FALSE );
}

/**********************************************************************/

bool CCashRSPVersionChecker::CheckCashRSPVersion( int nLineVersion, int nWantVersion, bool bAllowZero )
{
	if ( 0 == nLineVersion )
		return bAllowZero;
	else
		return ( ( nLineVersion >= nWantVersion ) && ( nLineVersion <= MAX_CASHRSP_VERSION ) );
}

/**********************************************************************/

void CCashRSPVersionChecker::ProcessAppTypeLine( CCSVEposTermLine& csvIn )
{
	m_nPendingTransactionAppType = csvIn.GetApplicationType();
	
	if ( ValidateAppType( m_nPendingTransactionAppType ) == TRUE )
		m_nFailedAppType = 0;
	else
		m_nFailedAppType = m_nPendingTransactionAppType;
}

/**********************************************************************/

void CCashRSPVersionChecker::ProcessAppTypeLine( CCSVEposTermLine& csvIn, CString& strBuffer )
{
	m_strPendingTransactionAppLine = strBuffer;
	ProcessAppTypeLine( csvIn );
}

/**********************************************************************/

bool CCashRSPVersionChecker::ValidateAppType( int nType )
{
	return ( CheckAppType( nType, 1, FALSE ) );
}

/**********************************************************************/

bool CCashRSPVersionChecker::CheckAppType( int nLineVersion, int nWantVersion, bool bAllowZero )
{
	if ( 0 == nLineVersion )
		return bAllowZero;
	else
		return ( ( nLineVersion >= nWantVersion ) && ( nLineVersion <= 2 ) );
}

/**********************************************************************/

void CCashRSPVersionChecker::ShowCashRSPFormatFailMessage( bool bNewSales )
{
	CString strSource = "";
	CString strAction = "";
	if ( TRUE == bNewSales )
	{
		strSource = "pending CashRSP data";
		strAction = "process these sales";
	}
	else
	{
		strSource = "CashRSP history log";
		strAction = "create this report";
	}

	if ( 0 != m_nFailedCashRSPVersion )
	{
		CString strMsg;
		strMsg.Format ( "Unknown data format in the %s (V%d).\n\n%s will require an update in order to %s.",
			(const char*) strSource,
			m_nFailedCashRSPVersion,
			AfxGetAppName(),
			(const char*) strAction );

		Prompter.ContactDealer( strMsg );
	}
	else if ( 0 != m_nFailedAppType )
	{
		CString strMsg;
		strMsg.Format ( "Unknown application type in the %s (type %d).\n\n%s will require an update in order to %s.",
			(const char*) strSource,
			m_nFailedAppType,
			AfxGetAppName(),
			(const char*) strAction );

		Prompter.ContactDealer( strMsg );
	}
}

/**********************************************************************/




