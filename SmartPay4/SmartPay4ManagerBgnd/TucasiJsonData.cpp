//#include "GlobalFunctions.h"
#include "TucasiJsonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
	const char* szIPAddress = "vm1007uat24.tucasi.com";
	const char* szObjectName = "GatewayWebapp/data/schoolid/2009888/cashlessv2/transactions?lastTransactionNum=0";
	const char* szBody = "";
	const char* szResponseFilename = "transactions.json";
	const char* szUsername = "sharp_test@tucasi.com";
	const char* szPassword = "Sh@rp123";
	const char* szProviderName = "Microtrend";
	const char* szProviderVersion = "0.01";
*/

CTucasiJsonData::CTucasiJsonData()
{
	m_strProviderName			= "Microtrend";					// not saved
	m_strIPAddress				= "";
	m_strUsername				= "";
	m_strPassword				= "";
	m_strSchoolID				= "";
	m_nLastPaymentID			= 0;
	m_nAccountPurse				= 1;
	m_nMaxTransactionLines		= 1000;
	m_bImportPayments			= TRUE;
	m_bExportBalances			= TRUE;
	m_bIncludePurse2			= FALSE;
	m_bKeepJsonFile				= FALSE;

	m_nAccountNameFormat		= 0;									// not saved 0=Forename Surname, 1=CardName, 2-Surname, 3=SurnameForename

	CString strPath = System.GetWebPaymentPath ( "", nWEBPAYMENT_TUCASIv2 );	// creates folder if not found
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		::MakeSubdirectory ( strPath );

	m_strFilename = System.GetWebPaymentPath ( "Tv2data.dat", nWEBPAYMENT_TUCASIv2 );

	m_strUploadPurchaseHeader	= "transactionNumber,transactionItemNumber,paymentDate,paymentAmount,description,balance,accountID,accountName";
	m_strPaymentHeader			= "transactionNumber,transactionItemNumber,paymentDate,paymentAmount,description,accountID,accountName";
}

//******************************************************************

void CTucasiJsonData::Read()
{
	CSSIniFile file ( SS_INI_AES );
	if ( file.Read ( m_strFilename ) == TRUE )
	{
		m_strIPAddress				= file.GetString ( "IPAddress", m_strIPAddress );
		m_strUsername				= file.GetString ( "Username", m_strUsername );
		m_strPassword				= file.GetString ( "Password", m_strPassword );
		m_strSchoolID				= file.GetString ( "SchoolID", m_strSchoolID );
		m_nLastPaymentID			= file.GetInt64 ( "LastPaymentID", m_nLastPaymentID );
		m_nAccountPurse				= file.GetInt	( "AccountPurse", m_nAccountPurse );
		m_nMaxTransactionLines		= file.GetInt   ( "MaxTransactionLines", m_nMaxTransactionLines );
		m_bImportPayments			= file.GetBool  ( "ImportPayments", m_bImportPayments );
		m_bExportBalances			= file.GetBool  ( "ExportBalances", m_bExportBalances );
		m_bIncludePurse2			= file.GetBool	( "IncludePurse2", m_bIncludePurse2 );
		m_bKeepJsonFile				= file.GetBool	( "KeepJsonFile", m_bKeepJsonFile );
	}
}

//*********************************************************************

void CTucasiJsonData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "IPAddress", m_strIPAddress );
	file.Set ( "Username", m_strUsername );
	file.Set ( "Password", m_strPassword );
	file.Set ( "SchoolID", m_strSchoolID );
	file.Set ( "LastPaymentID", m_nLastPaymentID );	
	file.Set ( "AccountPurse", m_nAccountPurse );
	file.Set ( "MaxTransactionLines", m_nMaxTransactionLines );
	file.Set ( "ImportPayments", m_bImportPayments );
	file.Set ( "ExportBalances", m_bExportBalances );
	file.Set ( "IncludePurse2", m_bIncludePurse2 );
	file.Set ( "KeepJsonFile", m_bKeepJsonFile );

	file.Write ( m_strFilename );
}

//******************************************************************

bool CTucasiJsonData::IsSet()
{
	if ( m_strIPAddress == "" )		return FALSE;
	if ( m_strSchoolID == "" )		return FALSE;
	if ( m_strPassword == "" )		return FALSE;
	return TRUE;
}

//******************************************************************
// get full path of where to save demo system login file
CString CTucasiJsonData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_TUCASIv2 );
	return strPathname;	
}

//******************************************************************
// Log of Json GET\POST responses ( only POST at the moment )
CString CTucasiJsonData::GetResponseLogFilename()
{
	CString strPath = System.GetWebPaymentPath ( "GpLogs", nWEBPAYMENT_TUCASIv2 );	// creates folder if not found
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		::MakeSubdirectory ( strPath );

	CSSDate date;	date.SetCurrentDate();

	CString strFilename;	strFilename.Format ( "%s\\pr-%d%2.2d.dat", strPath, date.GetYear(), date.GetMonth() );	// post responses
	return strFilename;	
}

