//*******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "SimsData.h"
//*******************************************************************
static const char* szADNO_TEXT	= "Adno";
static const char* szPIDM_TEXT	= "Person_id";
static const char* szUPN_TEXT	= "UPN";
//*******************************************************************

CSimsData::CSimsData()
	: m_strImportFilename(_T(""))
	, m_strSimsPhotoFolder(_T(""))
	, m_bDeleteImportFile(FALSE)
	, m_bDeletePhotoFile(FALSE)
	, m_nFSMGroupNo(2)
	, m_nFSMRevertGroupNo(1)
	, m_nYearContactNo(3)								// Contact No for Year Group
	, m_nRegContactNo(4)								// Contact No for Reg Group
	, m_bSaveEmail(TRUE)
	, m_nEmailContactNo(0)								// 0=email field, else info1=8
	, m_nSIMSUserTextNo(6)								// USerText field to use for SIMS membership database 
	, m_nUPNUserTextNo(7)								// UserText field to use for UPN membership database
	, m_nPIDUserTextNo(8)								// USerText field to use for Person_idS membership database 
	, m_nDobNo(1)										// Dob1 or 2 to put SIMS dob in
	, m_nNewIntakeAccount(nSIMS_NEWINTAKE_PID_INDEX)	// Create account for new intake
	, m_nLocateBy(nSIMS_LOCATEBY_PID_INDEX)				// locate exting account by
	, m_bAddUnknowns(FALSE)
	, m_nLocatePhotoBy(nSIMS_LOCATEPHOTOBY_ADNO_INDEX)
	, m_nImportMethod(nIMPORT_METHOD_ALL)
	, m_strStartFromUserID("1")
{
	m_strFilename = Filenames.GetSIMSImportDataFilename();
}

//*******************************************************************

bool CSimsData::Read()
{
	CSSIniFile file;
	bool bReply = file.Read ( m_strFilename );

	m_strImportFilename		= file.GetString ( "ImportFile",	m_strImportFilename );
	m_strSimsPhotoFolder	= file.GetString ( "PhotoFolder",	m_strSimsPhotoFolder );
	m_bDeleteImportFile		= file.GetBool	 ( "DeleteImport",	m_bDeleteImportFile );
	m_bDeletePhotoFile		= file.GetBool	 ( "DeletePhoto",	m_bDeletePhotoFile );
	m_nFSMGroupNo			= file.GetInt	 ( "FSMGroup",		m_nFSMGroupNo );
	m_nFSMRevertGroupNo		= file.GetInt	 ( "FSMRevert",		m_nFSMRevertGroupNo );
	m_nPIDUserTextNo		= file.GetInt	 ( "PIDNo",			m_nPIDUserTextNo );
	m_nUPNUserTextNo		= file.GetInt	 ( "UPNNo",			m_nUPNUserTextNo );
	m_nSIMSUserTextNo		= file.GetInt	 ( "SIMSNo",		m_nSIMSUserTextNo );
	m_nYearContactNo		= file.GetInt	 ( "YearNo",		m_nYearContactNo );
	m_nRegContactNo			= file.GetInt	 ( "RegNo",			m_nRegContactNo );
	m_bSaveEmail			= file.GetBool   ( "SaveEmail",		m_bSaveEmail );
	m_nEmailContactNo		= file.GetInt	 ( "EmailNo",		m_nEmailContactNo );
	m_nDobNo				= file.GetInt	 ( "DobNo",			m_nDobNo );
	m_nNewIntakeAccount		= file.GetInt	 ( "NewIntake",		m_nNewIntakeAccount );
	m_nLocateBy				= file.GetInt	 ( "LocateBy",		m_nLocateBy );
	m_bAddUnknowns			= file.GetBool	 ( "AddUnknowns",	m_bAddUnknowns );
	m_nLocatePhotoBy		= file.GetInt	 ( "LocatePhotoBy",	m_nLocatePhotoBy );
	m_nImportMethod			= file.GetInt	 ( "ImportMethod",	m_nImportMethod );
	m_strStartFromUserID	= file.GetString ( "StartUserID",	m_strStartFromUserID );

	return bReply;
}

//*********************************************************************

bool CSimsData::Save()
{
	CSSIniFile file;

	file.Set ( "ImportFile",	m_strImportFilename );
	file.Set ( "PhotoFolder",	m_strSimsPhotoFolder );
	file.Set ( "DeleteImport",	m_bDeleteImportFile );
	file.Set ( "DeletePhoto",	m_bDeletePhotoFile );
	file.Set ( "FSMGroup",		m_nFSMGroupNo );
	file.Set ( "FSMRevert",		m_nFSMRevertGroupNo );
	file.Set ( "PIDNo",			m_nPIDUserTextNo );
	file.Set ( "UPNNo",			m_nUPNUserTextNo );
	file.Set ( "SIMSNo",		m_nSIMSUserTextNo );
	file.Set ( "YearNo",		m_nYearContactNo );
	file.Set ( "RegNo",			m_nRegContactNo );
	file.Set ( "SaveEmail",		m_bSaveEmail );
	file.Set ( "EmailNo",		m_nEmailContactNo );
	file.Set ( "DobNo",			m_nDobNo );
	file.Set ( "NewIntake",		m_nNewIntakeAccount );
	file.Set ( "LocateBy",		m_nLocateBy );
	file.Set ( "AddUnknowns",	m_bAddUnknowns );
	file.Set ( "LocatePhotoBy",	m_nLocatePhotoBy );
	file.Set ( "ImportMethod",	m_nImportMethod );
	file.Set ( "StartUserID",	m_strStartFromUserID );

	return file.Write ( m_strFilename );
}

//*********************************************************************

const char* CSimsData::GetSIMSTextLabel()	{ return GetTextLabel( m_nSIMSUserTextNo ); }	//
const char* CSimsData::GetUPNTextLabel()	{ return GetTextLabel( m_nUPNUserTextNo ); }	// 0=NotDefined,1=Member#,2-9=info1=8
const char* CSimsData::GetPIDTextLabel()	{ return GetTextLabel( m_nPIDUserTextNo ); }	//

const char* CSimsData::GetTextLabel ( int nInfo )
{
	if ( nInfo == 1 )	return Account::MemberID.Label;
	if ( nInfo == 2 )	return Account::Info1.Label;
	if ( nInfo == 3 )	return Account::Info2.Label;
	if ( nInfo == 4 )	return Account::Info3.Label;
	if ( nInfo == 5 )	return Account::Info4.Label;
	if ( nInfo == 6 )	return Account::Info5.Label;
	if ( nInfo == 7 )	return Account::Info6.Label;
	if ( nInfo == 8 )	return Account::Info7.Label;
	if ( nInfo == 9 )	return Account::Info8.Label;

	return "";
}

//*********************************************************************

const char* CSimsData::GetLocateByLabel()
{
	if ( m_nLocateBy == nSIMS_LOCATEBY_ADNO_INDEX )	return GetSIMSTextLabel();
	if ( m_nLocateBy == nSIMS_LOCATEBY_UPN_INDEX )	return GetUPNTextLabel();
	if ( m_nLocateBy == nSIMS_LOCATEBY_PID_INDEX )	return GetPIDTextLabel();
	return "";
}

const char* CSimsData::GetLocateByText ( int nIndex )
{
	if ( nIndex == nSIMS_LOCATEBY_ADNO_INDEX )	return szADNO_TEXT;
	if ( nIndex == nSIMS_LOCATEBY_UPN_INDEX )	return szUPN_TEXT;
	if ( nIndex == nSIMS_LOCATEBY_PID_INDEX )	return szPIDM_TEXT;
	return "";
}

//*******************************************************************

const char* CSimsData::GetLocatePhotoByLabel()
{
	if ( m_nLocatePhotoBy == nSIMS_LOCATEPHOTOBY_ADNO_INDEX )	return GetSIMSTextLabel();
	if ( m_nLocatePhotoBy == nSIMS_LOCATEPHOTOBY_UPN_INDEX )	return GetUPNTextLabel();
	if ( m_nLocatePhotoBy == nSIMS_LOCATEPHOTOBY_PID_INDEX )	return GetPIDTextLabel();
	return "";
}

const char* CSimsData::GetLocatePhotoByText ( int nIndex )
{
	if ( nIndex == -1 )
		nIndex = m_nLocatePhotoBy;

	if ( nIndex == nSIMS_LOCATEPHOTOBY_ADNO_INDEX )	return szADNO_TEXT;
	if ( nIndex == nSIMS_LOCATEPHOTOBY_UPN_INDEX )	return szUPN_TEXT;
	if ( nIndex == nSIMS_LOCATEPHOTOBY_PID_INDEX )	return szPIDM_TEXT;
	return "";
}

//*******************************************************************

const char* CSimsData::GetNewIntakeAddedByText ( int nIndex )
{
	if ( nIndex == -1 )
		nIndex = m_nNewIntakeAccount;

	if ( nIndex == nSIMS_NEWINTAKE_ADNO_INDEX )		return szADNO_TEXT;
	if ( nIndex == nSIMS_NEWINTAKE_PID_INDEX )		return szPIDM_TEXT;
	if ( nIndex == nSIMS_NEWINTAKE_NEXTFREE_INDEX )	return "Next free";
	return "";
}


//*********************************************************************
// if fields used to locate records - they must exist in import file

bool CSimsData::SIMSRequired()
{
	if (  m_nLocateBy			== nSIMS_LOCATEBY_ADNO_INDEX )		return TRUE;
	if ( m_nNewIntakeAccount	== nSIMS_NEWINTAKE_ADNO_INDEX )		return TRUE;
	return FALSE;
}

bool CSimsData::UPNRequired()
{
	return ( m_nLocateBy == nSIMS_LOCATEBY_UPN_INDEX ) ? TRUE : FALSE;
}

bool CSimsData::PIDRequired()
{
	if ( m_nLocateBy			== nSIMS_LOCATEBY_PID_INDEX )		return TRUE;
	if ( m_nNewIntakeAccount	== nSIMS_NEWINTAKE_PID_INDEX )		return TRUE;
	return FALSE;
}

//*********************************************************************
