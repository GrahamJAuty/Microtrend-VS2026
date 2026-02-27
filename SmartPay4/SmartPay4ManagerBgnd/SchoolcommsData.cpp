//**********************************************************************
#include "SchoolcommsData.h"
//**********************************************************************

CSchoolcommsData::CSchoolcommsData()
{
	m_bKeepXmlLogin = FALSE;
	m_bKeepXmlFiles = FALSE;
	m_strSoapVersion = "1.1";
	m_strUsername = "";
	m_strPassword = "";
	m_strMemorable = "";
	m_strSchoolID = "";
	m_nLastPaymentID = 0;
	m_nAccountPurse = 1;
	m_nMaxTransactionLines = 4000;
	m_bIncludePurse2 = FALSE;

	CString strPath = System.GetWebPaymentPath("", nWEBPAYMENT_SCHOOLCOMMS);	// creates folder if not found
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	m_strFilename = System.GetWebPaymentPath("Scomms.dat", nWEBPAYMENT_SCHOOLCOMMS);
	m_strUploadHeader = "a:ChargedAmount,a:CurrentMemberBalance,a:DescriptionOfPurchasedItem,a:FreeSchoolMeal,a:FullAmount,a:PartnerTransactionID,a:ReceiptID,a:SIMSPersonID,a:TransactionDate,a:TransactionType";
}

//**********************************************************************

void CSchoolcommsData::Read()
{
	CSSIniFile file(SS_INI_AES);
	if (file.Read(m_strFilename) == TRUE)
	{
		m_bKeepXmlLogin = file.GetBool("KeepXmlLogin", m_bKeepXmlLogin);
		m_bKeepXmlFiles = file.GetBool("KeepXmlFiles", m_bKeepXmlFiles);
		m_strSoapVersion = file.GetString("SoapVersion", "1.1");
		m_strSchoolID = file.GetString("SchoolID", "");
		m_strUsername = file.GetString("Username", "");
		m_strPassword = file.GetString("Password", "");
		m_strMemorable = file.GetString("Memorable", "");
		m_nLastPaymentID = file.GetInt64("LastPaymentID", 0);
		m_nAccountPurse = file.GetInt("AccountPurse", 1);
		m_nMaxTransactionLines = file.GetInt("MaxTransactionLines", 4000);
		m_bIncludePurse2 = file.GetBool("IncludePurse2", FALSE);
	}
}

//**********************************************************************

void CSchoolcommsData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "KeepXmlLogin", m_bKeepXmlLogin );
	file.Set ( "KeepXmlFiles", m_bKeepXmlFiles );
	file.Set ( "SoapVersion", m_strSoapVersion );
	file.Set ( "SchoolID", m_strSchoolID );
	file.Set ( "Username", m_strUsername );
	file.Set ( "Password", m_strPassword );
	file.Set ( "Memorable", m_strMemorable );
	file.Set ( "LastPaymentID", m_nLastPaymentID );	
	file.Set ( "AccountPurse", m_nAccountPurse );
	file.Set ( "MaxTransactionLines", m_nMaxTransactionLines );
	file.Set ( "IncludePurse2", m_bIncludePurse2 );

	file.Write ( m_strFilename );
}

//**********************************************************************

bool CSchoolcommsData::IsSet()
{
	if (m_strSchoolID == "")
	{
		return FALSE;
	}

	if (m_strUsername == "")
	{
		return FALSE;
	}

	if (m_strPassword == "")
	{
		return FALSE;
	}

	if (m_strMemorable == "")
	{
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************
// get full path of where to save demo system login file

CString CSchoolcommsData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_SCHOOLCOMMS );
	return strPathname;	
}

//**********************************************************************

