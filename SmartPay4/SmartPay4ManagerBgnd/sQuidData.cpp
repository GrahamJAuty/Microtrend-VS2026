//#include "GlobalFunctions.h"
#include "SquidData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define nLENGTH_CARDNO		16
//#define nLENGTH_CVV			3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSquidData::CSquidData()
{
	m_bKeepXmlLogin				= FALSE;
	m_bKeepXmlFiles				= FALSE;
	m_bKeepLinkLog				= TRUE;
	m_strIPAddress				= "services.squidcard.com";
	m_strSoapVersion			= "1.1";
	m_strMerchantID				= "";
	m_strIssuerID				= "";
	m_strPassword				= "";
	m_nAccountPurse				= 1;
	m_nMaxTransactionLines		= 1000;
	m_nSquidCardHolderField		= 7;						// 0=not defined else Info 1- 8 ( 7= default UPN)
	m_nSquidExtRefField			= 8;						// 0=not defined else Info 1- 8 ( 8= default MIS_ID)
	m_nSquidCardField			= 0;						// 0=not defined else Info 1- 8
	m_nSquidCVVField			= 0;						// 0=not defined else Info 1- 8

	CString strPath = System.GetWebPaymentPath ( "", nWEBPAYMENT_SQUID );	// creates folder if not found
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		::MakeSubdirectory ( strPath );

	m_strFilename = System.GetWebPaymentPath ( "sQuid.dat", nWEBPAYMENT_SQUID );

	m_strUploadHeader = "amount,closingBalance,dateAndTime,description,merchantId,purseAccountNumber,rpu,rpuSequence,transactionNumber,transactionType";
}

//******************************************************************

void CSquidData::Read()
{
	CSSIniFile file ( SS_INI_AES );
	if ( file.Read ( m_strFilename ) == TRUE )
	{
		m_bKeepXmlLogin				= file.GetBool   ( "KeepXmlLogin", m_bKeepXmlLogin );
		m_bKeepXmlFiles				= file.GetBool   ( "KeepXmlFiles", m_bKeepXmlFiles );
		m_bKeepLinkLog				= file.GetBool	 ( "KeepLinkLog",  m_bKeepLinkLog );
		m_strIPAddress				= file.GetString ( "IPAddress",	   m_strIPAddress );
		m_strSoapVersion			= file.GetString ( "SoapVersion", m_strSoapVersion );
		m_strMerchantID				= file.GetString ( "MerchantID", m_strMerchantID );
		m_strIssuerID				= file.GetString ( "IssuerID", m_strIssuerID );
		m_strPassword				= file.GetString ( "Password", m_strPassword );
		m_nAccountPurse				= file.GetInt    ( "AccountPurse", m_nAccountPurse );
		m_nMaxTransactionLines		= file.GetInt    ( "MaxTxLines", m_nMaxTransactionLines );
		m_nSquidCardHolderField		= file.GetInt	 ( "CardHolderField", m_nSquidCardHolderField );
		m_nSquidExtRefField			= file.GetInt	 ( "ExtRefField", m_nSquidExtRefField );
		m_nSquidCardField			= file.GetInt    ( "CardField", m_nSquidCardField );
		m_nSquidCVVField			= file.GetInt    ( "CVVField", m_nSquidCVVField );
	}
}

//*********************************************************************

void CSquidData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "KeepXmlLogin", m_bKeepXmlLogin );
	file.Set ( "KeepXmlFiles", m_bKeepXmlFiles );
	file.Set ( "KeepLinkLog",  m_bKeepLinkLog );
	file.Set ( "IPAddress", m_strIPAddress );
	file.Set ( "SoapVersion", m_strSoapVersion );
	file.Set ( "MerchantID", m_strMerchantID	);
	file.Set ( "IssuerID", m_strIssuerID );
	file.Set ( "Password", m_strPassword );
	file.Set ( "AccountPurse", m_nAccountPurse );
	file.Set ( "MaxTxLines", m_nMaxTransactionLines );
	file.Set ( "CardHolderField", m_nSquidCardHolderField );
	file.Set ( "ExtRefField", m_nSquidExtRefField );
	file.Set ( "CardField", m_nSquidCardField );
	file.Set ( "CVVField", m_nSquidCVVField );

	file.Write ( m_strFilename );
}

//******************************************************************

bool CSquidData::IsSet()
{
	if ( m_strIPAddress == "" )			return FALSE;
	if ( m_strMerchantID == "" )		return FALSE;
	if ( m_strIssuerID == "" )			return FALSE;
	if ( m_strPassword == "" )			return FALSE;
	return TRUE;
}

//******************************************************************
// get full path of where to save demo system login file

CString CSquidData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_SQUID );
	return strPathname;	
}

//******************************************************************

void CSquidData::SetSquidCardNo (CSQLRowAccountFull& RowAccount, const char* szSquidAccountNo )
{
	if (m_nSquidCardField != 0)										// 0=not defined else Info 1- 8
	{
		RowAccount.SetInfo(m_nSquidCardField, szSquidAccountNo);
	}
}

//******************************************************************

void CSquidData::SetSquidCVV (CSQLRowAccountFull& RowAccount, const char* szSquidCVV )
{
	if (m_nSquidCVVField != 0)										// 0=not defined else Info 1- 8
	{
		RowAccount.SetInfo(m_nSquidCVVField, szSquidCVV);
	}
}

//**********************************************************************

CString CSquidData::GetSquidLinkReference1(CSQLRowAccountFull& RowAccount)
{
	CString strBuf = RowAccount.GetInfo(m_nSquidCardHolderField);
	return strBuf;
}

//**********************************************************************

CString CSquidData::GetSquidLinkReference2(CSQLRowAccountFull& RowAccount)
{
	CString strBuf = RowAccount.GetInfo(m_nSquidExtRefField);
	return strBuf;
}

//**********************************************************************

CString CSquidData::GetSquidCardNo (CSQLRowAccountFull& RowAccount)
{
	CString strBuf = RowAccount.GetInfo ( m_nSquidCardField );
	return strBuf;
}

//******************************************************************
