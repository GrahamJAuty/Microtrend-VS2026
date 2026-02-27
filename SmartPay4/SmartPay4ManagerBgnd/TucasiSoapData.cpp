//#include "SystemData.h"
//#include "GlobalFunctions.h"
#include "TucasiSoapData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTucasiSoapData::CTucasiSoapData()
{
	m_bKeepXmlLogin				= FALSE;
	m_bKeepXmlFiles				= FALSE;
	m_strUserName				= "";
	m_strPassword				= "";
	m_strSchoolID				= "";
	m_strSoapVersion			= "1.1";
	m_nLastPaymentID			= 0;
	m_nAccountPurse				= 1;
	m_bImportPayments			= TRUE;
	m_bExportUpdates			= TRUE;
	m_bExportBalances			= TRUE;

	CString strPath = System.GetWebPaymentPath ( "", nWEBPAYMENT_TUCASI );	// creates folder if not found
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		::MakeSubdirectory ( strPath );

	m_strFilename = System.GetWebPaymentPath ( "Tucasi.dat", nWEBPAYMENT_TUCASI );

	m_strUploadPurchaseHeader = "description,paymentAmount,paymentDate,pupilName,pupilNumber,transactionItemNumber,transactionNumber";
}

CString CTucasiSoapData::GetUploadPurchaseHeader()
{
	return m_strUploadPurchaseHeader;
}

//******************************************************************

void CTucasiSoapData::Read()
{
	CSSIniFile file ( SS_INI_AES );
	if ( file.Read ( m_strFilename ) == TRUE )
	{
		m_bKeepXmlLogin		= file.GetBool   ( "KeepXmlLogin", m_bKeepXmlLogin );
		m_bKeepXmlFiles		= file.GetBool   ( "KeepXmlFiles", m_bKeepXmlFiles );
		m_strUserName		= file.GetString ( "Username", m_strUserName );
		m_strPassword		= file.GetString ( "Password", m_strPassword );
		m_strSchoolID		= file.GetString ( "SchoolID", m_strSchoolID );
		m_strSoapVersion	= file.GetString ( "SoapVersion", m_strSoapVersion );
		m_nLastPaymentID	= file.GetInt64  ( "LastPaymentID", m_nLastPaymentID );
		m_nAccountPurse		= file.GetInt  ( "AccountPurse", 1 );
		m_bImportPayments	= file.GetBool   ( "ImportPayments", m_bImportPayments );
		m_bExportUpdates	= file.GetBool   ( "ExportUpdates", m_bExportUpdates );
		m_bExportBalances	= file.GetBool   ( "ExportBalances", m_bExportBalances );
		m_bIncludePurse2	= file.GetBool ( "IncludePurse2", FALSE );
	}
}

//*********************************************************************

void CTucasiSoapData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "KeepXmlLogin", m_bKeepXmlLogin );
	file.Set ( "KeepXmlFiles", m_bKeepXmlFiles );
	file.Set ( "Username", m_strUserName );
	file.Set ( "Password", m_strPassword );
	file.Set ( "SchoolID", m_strSchoolID );
	file.Set ( "SoapVersion", m_strSoapVersion );
	file.Set ( "LastPaymentID", m_nLastPaymentID );
	file.Set ( "AccountPurse", m_nAccountPurse );
	file.Set ( "ImportPayments", m_bImportPayments );
	file.Set ( "ExportUpdates", m_bExportUpdates );
	file.Set ( "ExportBalances", m_bExportBalances );
	file.Set ( "IncludePurse2", m_bIncludePurse2 );

	file.Write ( m_strFilename );
}

//******************************************************************

bool CTucasiSoapData::IsSet()
{
	if ( m_strSchoolID == "" )	return FALSE;
	if ( m_strUserName == "" )	return FALSE;
	if ( m_strPassword == "" )	return FALSE;

	return TRUE;
}

//******************************************************************
// get full path of where to save demo system login file

CString CTucasiSoapData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_TUCASI );
	return strPathname;	
}

//******************************************************************

