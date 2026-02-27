//**********************************************************************
#include "WisePayData.h"
//**********************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CWisePayData::CWisePayData()
{
	m_bKeepXmlLogin				= FALSE;
	m_bKeepXmlFiles				= FALSE;
	m_strBalanceText			= "System Upgrade";			// text when ending balance transactions 

	m_strSchoolID				= "";
	m_strSoapVersion			= "1.2";
	m_strPassword				= "microtrend8793345tP";	// password is fixed for all systems

	m_bImportAccount1			= FALSE;
	m_nAccountID1				= 0;
	m_nAccount1Purse			= 1;
	m_nLastAccount1				= 0;

	m_bImportAccount2			= FALSE;
	m_nAccountID2				= 0;
	m_nAccount2Purse			= 1;
	m_nLastAccount2				= 0;

	m_bIncludePurse2			= FALSE;

	m_nNewIntakeAccount			= nWISEPAY_NEWINTAKE_NEXTFREE_INDEX;
	m_strStartFromUserID		= "1";

	m_bSaveEmail				= FALSE;
	m_nEmailInfoIndex			= 0;				// 0=email field, or info 1-8
	m_nYearInfoIndex			= 0;				// Year
	m_nRegInfoIndex				= 0;				// Reg
	m_nDobInfoIndex				= 0;				// Dob
	m_bAddUnknowns				= TRUE;


	CString strPath = System.GetWebPaymentPath ( "", nWEBPAYMENT_WISEPAY );	// creates folder if not found
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		::MakeSubdirectory ( strPath );

	m_strFilename = System.GetWebPaymentPath ( "WisePay.dat", nWEBPAYMENT_WISEPAY );

	m_strUploadHeader1 = "student_id,description,value,transaction_id,balance,cart_date_time";
	m_strUploadHeader2 = "student_id,description,value,product_id,transaction_id,balance,cart_date_time";
}

//******************************************************************

void CWisePayData::Read()
{
	CSSIniFile file ( SS_INI_AES );
	if ( file.Read ( m_strFilename ) == TRUE )
	{ 
		m_bKeepXmlLogin				= file.GetBool   ( "KeepXmlLogin", m_bKeepXmlLogin );
		m_bKeepXmlFiles				= file.GetBool   ( "KeepXmlFiles", m_bKeepXmlFiles );
		m_strPassword				= file.GetString ( "Password", m_strPassword );
		m_strSchoolID				= file.GetString ( "SchoolID", m_strSchoolID );
		m_strSoapVersion			= file.GetString ( "SoapVersion", m_strSoapVersion );
		m_bImportAccount1			= file.GetBool   ( "ImportAccount1", m_bImportAccount1 );
		m_nAccountID1				= file.GetInt    ( "Account1", m_nAccountID1 );
		m_nAccount1Purse			= file.GetInt	 ( "Account1Purse", m_nAccount1Purse );
		m_nLastAccount1				= file.GetInt64  ( "LastAccount1", m_nLastAccount1 );
		m_bImportAccount2			= file.GetBool   ( "ImportAccount2", m_bImportAccount2 );
		m_nAccountID2				= file.GetInt    ( "Account2", m_nAccountID2 );
		m_nAccount2Purse			= file.GetInt	 ( "Account2Purse", m_nAccount2Purse );
		m_nLastAccount2				= file.GetInt64  ( "LastAccount2", m_nLastAccount2 );
		m_bIncludePurse2			= file.GetBool	 ( "IncludePurse2", m_bIncludePurse2 );
		m_bSaveEmail				= file.GetBool	 ( "SaveEmail", m_bSaveEmail );
		m_nEmailInfoIndex			= file.GetInt	 ( "EmailInfoIndex", m_nEmailInfoIndex );
		m_nYearInfoIndex			= file.GetInt	 ( "YearInfoIndex", m_nYearInfoIndex );
		m_nRegInfoIndex				= file.GetInt	 ( "RegInfoIndex", m_nRegInfoIndex );
		m_nDobInfoIndex				= file.GetInt	 ( "DobInfoIndex", m_nDobInfoIndex );
		m_nNewIntakeAccount			= file.GetInt	 ( "NewIntakeAccount", m_nNewIntakeAccount );
		m_bAddUnknowns				= file.GetBool	 ( "AddUnknowns", m_bAddUnknowns );
		m_strBalanceText			= file.GetString ( "BalanceText", m_strBalanceText );
		m_strStartFromUserID		= file.GetString ( "StartUserID", m_strStartFromUserID );
	}
}

//*********************************************************************

void CWisePayData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "KeepXmlLogin", m_bKeepXmlLogin );
	file.Set ( "KeepXmlFiles", m_bKeepXmlFiles );
	file.Set ( "Password", m_strPassword );
	file.Set ( "SchoolID", m_strSchoolID );
	file.Set ( "SoapVersion", m_strSoapVersion );
	file.Set ( "Account1", m_nAccountID1 );
	file.Set ( "ImportAccount1", m_bImportAccount1 );
	file.Set ( "Account1Purse", m_nAccount1Purse );
	file.Set ( "LastAccount1", m_nLastAccount1 );
	file.Set ( "ImportAccount2", m_bImportAccount2 );
	file.Set ( "Account2", m_nAccountID2 );
	file.Set ( "Account2Purse", m_nAccount2Purse );
	file.Set ( "LastAccount2", m_nLastAccount2 );
	file.Set ( "IncludePurse2", m_bIncludePurse2 );
	file.Set ( "SaveEmail", m_bSaveEmail );
	file.Set ( "EmailInfoIndex", m_nEmailInfoIndex );
	file.Set ( "YearInfoIndex", m_nYearInfoIndex );
	file.Set ( "RegInfoIndex", m_nRegInfoIndex );
	file.Set ( "DobInfoIndex", m_nDobInfoIndex );
	file.Set ( "NewIntakeAccount", m_nNewIntakeAccount );
	file.Set ( "AddUnknowns", m_bAddUnknowns );
	file.Set ( "BalanceText", m_strBalanceText );
	file.Set ( "StartUserID", m_strStartFromUserID );

	file.Write ( m_strFilename );
}

//******************************************************************

bool CWisePayData::IsSet()
{
	if ( m_strSchoolID == "" )								return FALSE;
	if ( m_bImportAccount1 == TRUE && m_nAccountID1 == 0 )	return FALSE;
	if ( m_bImportAccount2 == TRUE && m_nAccountID2 == 0 )	return FALSE;
	return TRUE;
}

bool CWisePayData::ImportPayments()
{
	if ( m_bImportAccount1 == TRUE )	return TRUE;
	if ( m_bImportAccount2 == TRUE )	return TRUE;
	return FALSE;
}

//******************************************************************

CString CWisePayData::GetWebPaymentListFilename ( int nAccountNo )
{
	int nAccountID = ( nAccountNo == 1 ) ? m_nAccountID1 : m_nAccountID2; 

	CString strFile;	strFile.Format ( "List%d.dat", nAccountID );

	CString strListName = System.GetWebPaymentPath ( strFile, nWEBPAYMENT_WISEPAY );
	return strListName;
}

//******************************************************************

CString CWisePayData::GetWebPaymentExceptionFilename ( int nAccountNo )
{
	int nAccountID = ( nAccountNo == 1 ) ? m_nAccountID1 : m_nAccountID2; 

	CString strBuf;		strBuf.Format ( "Wpexc%d.dat", nAccountID );
	
	CString strFilename = System.GetWebPaymentPath ( strBuf, nWEBPAYMENT_WISEPAY  );
	return strFilename;
}

//*******************************************************************

const char* CWisePayData::GetNewIntakeAddedByText ( int nIndex )
{
	if ( nIndex == -1 )
		nIndex = m_nNewIntakeAccount;

	if ( nIndex == nWISEPAY_NEWINTAKE_MISID_INDEX )		return "uID";		// create account by using MisID 
	if ( nIndex == nWISEPAY_NEWINTAKE_NEXTFREE_INDEX)	return "Next free";		// next free record
	return "";
}

//******************************************************************
// get full path of where to save demo system login file

CString CWisePayData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_WISEPAY );
	return strPathname;	
}

//******************************************************************
