//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "ParentPayData.h"
//**********************************************************************

static const char* szROLLNO_TEXT	= "Roll number";
static const char* szMISIDM_TEXT	= "MIS ID";
static const char* szUPN_TEXT		= "UPN";

//*******************************************************************

CParentPayData::CParentPayData()
{
	m_bKeepXmlLogin				= FALSE;
	m_bKeepXmlFiles				= FALSE;
	m_strSchoolID				= "";
	m_strUsername				= "";
	m_strPassword				= "";
	m_strSoapVersion			= "1.1";
	m_bImportAccount1			= FALSE;
	m_nAccountID1				= 0;
	m_nAccount1Purse			= 1;
	m_nMealSession1				= 1;
	m_nLastAccount1				= 0;
	m_bImportAccount2			= FALSE;
	m_nAccountID2				= 0;
	m_nAccount2Purse			= 1;
	m_nMealSession2				= 0;
	m_nLastAccount2				= 0;
	m_bImportWithPreview		= TRUE;
	m_nLastDinerSnapshotID		= 0;
	m_nLastBalanceSnapshotID	= 0;
	m_nLastPosTxnID				= 0;								// last ID sent to ParentPay
	m_nNextPosTxnID				= 0;								// next ID for new jobd
	m_strPosID					= "Sharp1";
	m_strVendorName				= "Microtrend";
	m_nNewIntakeAccount			= nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX;	// add new accounts 
	m_strStartFromUserID		= "1";
	m_nLegacyLocateBy			= nPARENTPAY_LOCATEBY_MISID_INDEX;		// locate exting account by
	m_nYearInfoIndex			= 3;				// Year		0-not defined, info1 - info8
	m_nRegInfoIndex				= 4;				// Reg		0-not defined, info1 - info8
	m_nRollNoInfoIndex			= 6;				// RollNo	0-not defined, info1 - info8
	m_nUPNInfoIndex				= 7;				// UPN		0-not defined, info1 - info8
	m_nMisIDInfoIndex			= 8;				// MIS ID	0-not defined, info1 - info8
	m_nDobInfoIndex				= 0;				// Dob
	m_bAddUnknowns				= TRUE;
	m_bExportPurse2Sales		= FALSE;

	CString strPath = System.GetWebPaymentPath ( "", nWEBPAYMENT_PARENTPAY );	// create "P44\ParentPay" folder if not found
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	m_strFilename = System.GetWebPaymentPath ( "ParentPay.dat", nWEBPAYMENT_PARENTPAY );
}

//*******************************************************************

void CParentPayData::Read()
{
	CSSIniFile file ( SS_INI_AES );
	if ( file.Read ( m_strFilename ) == TRUE )
	{ 
		m_bKeepXmlLogin				= file.GetBool  ( "KeepXmlLogin", m_bKeepXmlLogin );
		m_bKeepXmlFiles				= file.GetBool  ( "DemoSystem", m_bKeepXmlFiles );
		m_strSchoolID				= file.GetString ( "SchoolID", "" );
		m_strUsername				= file.GetString ( "Username", "" );
		m_strPassword				= file.GetString ( "Password", "" );
		m_strPosID					= file.GetString ( "PosID", m_strPosID );
		m_bImportAccount1			= file.GetBool ( "ImportPayments", FALSE );
		m_nAccountID1				= file.GetInt64( "ServiceID", 0);
		m_nAccount1Purse			= file.GetInt  ( "AccountPurse", m_nAccount1Purse );
		m_nMealSession1				= file.GetInt  ( "MealSession1", m_nMealSession1 );
		m_nLastAccount1				= file.GetInt64( "LastPaymentID", 0);
		m_bImportAccount2			= file.GetBool ( "ImportPayments2", FALSE );
		m_nAccountID2				= file.GetInt64( "ServiceID2", 0);
		m_nAccount2Purse			= file.GetInt  ( "AccountPurse2", 1 );
		m_nMealSession2				= file.GetInt  ( "MealSession2", m_nMealSession2 );
		m_nLastAccount2				= file.GetInt64( "LastPaymentID2", 0);
		m_nLastDinerSnapshotID		= file.GetInt64 ( "DinerSnapshot", m_nLastDinerSnapshotID );	
		m_nLastBalanceSnapshotID	= file.GetInt64 ( "BalanceSnapshot", m_nLastBalanceSnapshotID );
		m_nMisIDInfoIndex			= file.GetInt ( "MisIDInfoIndex", m_nMisIDInfoIndex );
		m_nUPNInfoIndex				= file.GetInt ( "UPNInfoIndex", m_nUPNInfoIndex );
		m_nRollNoInfoIndex			= file.GetInt ( "RollNoInfoIndex", m_nRollNoInfoIndex );
		m_nYearInfoIndex			= file.GetInt ( "YearInfoIndex", m_nYearInfoIndex );
		m_nRegInfoIndex				= file.GetInt ( "RegInfoIndex", m_nRegInfoIndex );
		m_nDobInfoIndex				= file.GetInt ( "DobInfoIndex", m_nDobInfoIndex );
		m_nNewIntakeAccount			= file.GetInt ( "NewIntakeAccount", m_nNewIntakeAccount );
		m_nLegacyLocateBy			= file.GetInt ( "LocateBy", m_nLegacyLocateBy );
		m_bAddUnknowns				= file.GetBool ( "AddUnknowns", m_bAddUnknowns );
		m_strStartFromUserID		= file.GetString ( "StartUserID", m_strStartFromUserID );

		m_nLastPosTxnID				= file.GetInt64 ( "LastPosTxnID", m_nLastPosTxnID );
		m_nNextPosTxnID				= file.GetInt64 ( "NextPosTxnID", m_nNextPosTxnID );		// added v1.14b

		m_bExportPurse2Sales		= file.GetBool( "ExportPurse2Sales", 0 );

		if (m_nNextPosTxnID == 0)
		{
			// set next to lastID + 1 if not set
			m_nNextPosTxnID = m_nLastPosTxnID + 1;
		}
	}
}

//*********************************************************************

void CParentPayData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "KeepXmlLogin", m_bKeepXmlLogin );
	file.Set ( "DemoSystem", m_bKeepXmlFiles );
	file.Set ( "SchoolID", m_strSchoolID );
	file.Set ( "Username", m_strUsername );
	file.Set ( "Password", m_strPassword );
	file.Set ( "PosID", m_strPosID );
	file.Set ( "ImportPayments", m_bImportAccount1 );
	file.Set ( "ServiceID", m_nAccountID1);
	file.Set ( "AccountPurse", m_nAccount1Purse );
	file.Set ( "MealSession1", m_nMealSession1 );
	file.Set ( "LastPaymentID", m_nLastAccount1 );
	file.Set ( "ImportPayments2", m_bImportAccount2 );
	file.Set ( "ServiceID2", m_nAccountID2 );
	file.Set ( "AccountPurse2", m_nAccount2Purse );
	file.Set ( "MealSession2", m_nMealSession2 );
	file.Set ( "LastPaymentID2", m_nLastAccount2 );
	file.Set ( "DinerSnapshot", m_nLastDinerSnapshotID );	
	file.Set ( "BalanceSnapshot", m_nLastBalanceSnapshotID );
	file.Set ( "MisIDInfoIndex", m_nMisIDInfoIndex );
	file.Set ( "UPNInfoIndex", m_nUPNInfoIndex );
	file.Set ( "RollNoInfoIndex", m_nRollNoInfoIndex );
	file.Set ( "YearInfoIndex", m_nYearInfoIndex );
	file.Set ( "RegInfoIndex", m_nRegInfoIndex );
	file.Set ( "DobInfoIndex", m_nDobInfoIndex );
	file.Set ( "NewIntakeAccount", m_nNewIntakeAccount );
	file.Set ( "LocateBy", m_nLegacyLocateBy );
	file.Set ( "AddUnknowns", m_bAddUnknowns );
	file.Set ( "StartUserID", m_strStartFromUserID );

	file.Set ( "LastPosTxnID", m_nLastPosTxnID );
	file.Set ( "NextPosTxnID", m_nNextPosTxnID );		// added v1.14b

	file.Set ( "ExportPurse2Sales", m_bExportPurse2Sales );

	file.Write ( m_strFilename );
}

//******************************************************************

__int64 CParentPayData::GetNextPosTxnID ( bool bIncrementReqd )
{
	__int64 nReply = m_nNextPosTxnID;

	if (bIncrementReqd == TRUE)
	{
		++m_nNextPosTxnID;
	}

	return nReply;
}

//******************************************************************

bool CParentPayData::IsSet()
{
	if ( m_strSchoolID == "" )						return FALSE;
	if ( m_strUsername == "" )						return FALSE;
	if ( m_strPassword == "" )						return FALSE;
	if ( m_nAccountID1 == 0 && m_nAccountID2 == 0 )	return FALSE;
	return TRUE;
}

//******************************************************************

bool CParentPayData::ImportPayments()
{
	if ( m_bImportAccount1 == TRUE )	return TRUE;
	if ( m_bImportAccount2 == TRUE )	return TRUE;
	return FALSE;
}

//******************************************************************
// get full path of where to save demo system login file
CString CParentPayData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_PARENTPAY );
	return strPathname;	
}

//******************************************************************

CString CParentPayData::GetWebPaymentListFilename ( int nServiceNo )
{
	CString strBuf = ( nServiceNo == 1 ) ? "Pplist.dat" : "Pplist2.dat"; 

	CString strListName = System.GetWebPaymentPath ( strBuf, nWEBPAYMENT_PARENTPAY );
	return strListName;
}
	
CString CParentPayData::GetWebPaymentExceptionFilename ( int nServiceNo )
{
	CString strBuf = ( nServiceNo == 1 ) ? "Ppexc.dat" : "Ppexc2.dat"; 

	CString strListName = System.GetWebPaymentPath ( strBuf, nWEBPAYMENT_PARENTPAY );
	return strListName;
}

//******************************************************************

CString CParentPayData::GetTextLabel ( int n )
{
	CString strResult = "";

	switch (n)
	{
	case 1:
		strResult = Account::Info1.Label;
		break;

	case 2:
		strResult = Account::Info2.Label;
		break;

	case 3:
		strResult = Account::Info3.Label;
		break;

	case 4:
		strResult = Account::Info4.Label;
		break;

	case 5:
		strResult = Account::Info5.Label;
		break;

	case 6:
		strResult = Account::Info6.Label;
		break;

	case 7:
		strResult = Account::Info7.Label;
		break;

	case 8:
		strResult = Account::Info8.Label;
		break;
	}

	return strResult;
}

//*******************************************************************

CString CParentPayData::GetLegacyLocateByLabel()
{
	CString strResult = "";

	switch (m_nLegacyLocateBy)
	{
	case nPARENTPAY_LOCATEBY_ROLLNO_INDEX:
		strResult = GetLocateByLabelRollNo();	
		break;

	case nPARENTPAY_LOCATEBY_UPN_INDEX:
		strResult = GetLocateByLabelUPN();
		break;

	case nPARENTPAY_LOCATEBY_MISID_INDEX:
		strResult = GetLocateByLabelMisID();
		break;
	}

	return strResult;
}

//*******************************************************************

CString CParentPayData::GetLocateByLabelRollNo()
{
	// 0-not defined, info1 - info8
	return GetTextLabel(m_nRollNoInfoIndex);
}

//*******************************************************************

CString CParentPayData::GetLocateByLabelUPN()
{
	return GetTextLabel(m_nUPNInfoIndex);
}

//*******************************************************************

CString CParentPayData::GetLocateByLabelMisID()
{
	return GetTextLabel(m_nMisIDInfoIndex);
}

//*******************************************************************

CString CParentPayData::GetLegacyLocateByText(int nIndex)
{
	CString strResult = "";

	if (nIndex == -1)
	{
		nIndex = m_nLegacyLocateBy;
	}

	switch (nIndex)
	{
	case nPARENTPAY_LOCATEBY_ROLLNO_INDEX:
		strResult = szROLLNO_TEXT;
		break;

	case nPARENTPAY_LOCATEBY_UPN_INDEX:
		strResult = szUPN_TEXT;
		break;

	case nPARENTPAY_LOCATEBY_MISID_INDEX:
		strResult = szMISIDM_TEXT;
		break;
	}

	return strResult;
}

//*******************************************************************

const char* CParentPayData::GetNewIntakeAddedByText ( int nIndex )
{
	if ( nIndex == -1 )
		nIndex = m_nNewIntakeAccount;

	if ( nIndex == nPARENTPAY_NEWINTAKE_MISID_INDEX )	return szMISIDM_TEXT;	// create account by using MisID 
	if ( nIndex == nPARENTPAY_NEWINTAKE_ROLLNO_INDEX )	return szROLLNO_TEXT;	// create account by using RollNo
	if ( nIndex == nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX)	return "Next free";		// next free record
	return "";
}

//*********************************************************************
// if fields used to locate records - they must exist in import file

bool CParentPayData::RollNoRequired()
{
	bool bNeeded = FALSE;
	//bNeeded |= (nPARENTPAY_LOCATEBY_ROLLNO_INDEX == m_nLegacyLocateBy);
	bNeeded |= (nPARENTPAY_LOCATEBY_ROLLNO_INDEX == m_nNewIntakeAccount);
	return bNeeded;
}

//*********************************************************************

bool CParentPayData::UPNRequired()
{
	bool bNeeded = FALSE;
	//bNeeded |= (nPARENTPAY_LOCATEBY_UPN_INDEX == m_nLegacyLocateBy);
	return bNeeded;
}

//*********************************************************************

bool CParentPayData::MISIDRequired()
{
	bool bNeeded = FALSE;
	//bNeeded |= (nPARENTPAY_LOCATEBY_MISID_INDEX == m_nLegacyLocateBy);
	bNeeded |= (nPARENTPAY_LOCATEBY_MISID_INDEX == m_nNewIntakeAccount);
	return bNeeded;
}

//************************************************************************

int CParentPayData::GetMealSessionNo ( int nPurseNo )
{
	if ((TRUE == m_bImportAccount1) && (m_nAccount1Purse == nPurseNo))
	{
		return m_nMealSession1;
	}

	if ((TRUE == m_bImportAccount2) && (m_nAccount2Purse == nPurseNo))
	{
		return m_nMealSession2;
	}

	return 0;
}

//************************************************************************
