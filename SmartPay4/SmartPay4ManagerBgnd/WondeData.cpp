//*******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "WondeData.h"
//*******************************************************************

CWondeData::CWondeData()
	: m_strImportFilename(_T(""))
	, m_bTestMode(FALSE)
	, m_nImportMethod(nIMPORT_METHOD_ALL)
	, m_bDeleteImportFile(TRUE)
	, m_nWebPaymentIndex1(1)							// pupil webpayment = None,MIS_ID,UPN,AdNo
	, m_nWebPaymentIndex2(1)							// staff webpayment = None,MIS_ID
	, m_nLocateOnIndex(0)								// 0=MIS_ID, 1=UPN,2=AdminNo
	, m_nFSMGroupNo(2)
	, m_nFSMRevertGroupNo(1)
	, m_nStaffGroupNo(3)
	, m_nMISIDUserTextNo(0)								// 0=notdefined, Info 1-8
	, m_nYearUserTextNo(3)								// 0=notdefined, Info 1-8
	, m_nRegUserTextNo(4)								// 0=notdefined, Info 1-8
	, m_nHouseUserTextNo(0)								// 0=notdefined, Info 1-8
	, m_nAdNoUserTextNo(7)								// 0=notdefined, Info 1-8 
	, m_nUPNUserTextNo(8)								//		""
	, m_bSavePupilEmail(TRUE)
	, m_nEmailUserTextNo(0)								// Pupil (0=email or info 1-8 )
	, m_bSaveStaffEmail(TRUE)							// Save staff work email	
	, m_nEmail2UserTextNo(0)							// Staff (0=email or info 1-8 )
	, m_strPhotoFolder(_T(""))
	, m_bDeletePhotoFile(TRUE)
	, m_bDisableStaffImport(FALSE)
	, m_nNewPupilIntakeIndex(nWONDE_NEWINTAKE_NEXTFREE_INDEX)
	, m_nNewStaffIntakeIndex(nWONDE_NEWINTAKE_NEXTFREE_INDEX)
	, m_strPupilStartFrom("1")
	, m_strStaffStartFrom("1")
{
	m_strFilename = Filenames.GetWondeDataFilename();
}

//*******************************************************************

bool CWondeData::Read()
{
	CSSIniFile file;
	bool bReply = file.Read ( m_strFilename );

	m_bTestMode				= file.GetBool	 ( "TestMode",		m_bTestMode );
	m_strImportFilename		= file.GetString ( "ImportFile",	m_strImportFilename );
	m_nWebPaymentIndex1		= file.GetInt	 ( "WebIndex1",		m_nWebPaymentIndex1 );
	m_nWebPaymentIndex2		= file.GetInt	 ( "WebIndex2",		m_nWebPaymentIndex2 );
	m_nLocateOnIndex		= file.GetInt	 ( "LocateOn",		m_nLocateOnIndex );
	m_bDeleteImportFile		= file.GetBool	 ( "DeleteImport",	m_bDeleteImportFile );
	m_nStaffGroupNo			= file.GetInt	 ( "StaffGroup",	m_nStaffGroupNo );
	m_nFSMGroupNo			= file.GetInt	 ( "FSMGroup",		m_nFSMGroupNo );
	m_nFSMRevertGroupNo		= file.GetInt	 ( "FSMRevert",		m_nFSMRevertGroupNo );
	m_nMISIDUserTextNo		= file.GetInt	 ( "MISNo",			m_nMISIDUserTextNo );
	m_nUPNUserTextNo		= file.GetInt	 ( "UPNNo",			m_nUPNUserTextNo );
	m_nAdNoUserTextNo		= file.GetInt	 ( "AdNo",			m_nAdNoUserTextNo );
	m_nYearUserTextNo		= file.GetInt	 ( "YearNo",		m_nYearUserTextNo );
	m_nRegUserTextNo		= file.GetInt	 ( "RegNo",			m_nRegUserTextNo );
	m_nHouseUserTextNo		= file.GetInt	 ( "HouseNo",		m_nHouseUserTextNo );
	m_nImportMethod			= file.GetInt	 ( "ImportMethod", m_nImportMethod );
	m_bSavePupilEmail		= file.GetBool   ( "SaveEmail1",	m_bSavePupilEmail );
	m_nEmailUserTextNo		= file.GetInt	 ( "Email1No",		m_nEmailUserTextNo );		// Pupil (0=email or info 1-8 )
	m_bSaveStaffEmail		= file.GetBool   ( "SaveEmail2",	m_bSaveStaffEmail );
	m_nEmail2UserTextNo		= file.GetInt	 ( "Email2No",		m_nEmail2UserTextNo );		// Staff (0=email or info 1-8 )
	m_strPhotoFolder		= file.GetString ( "PhotoFolder",	m_strPhotoFolder );
	m_bDeletePhotoFile		= file.GetBool	 ( "DeletePhoto",	m_bDeletePhotoFile );
	m_bDisableStaffImport	= file.GetBool	 ( "NoStaff",		m_bDisableStaffImport );
	m_strPupilStartFrom		= file.GetString ( "StartPupil",	m_strPupilStartFrom );
	m_strStaffStartFrom		= file.GetString ( "StartStaff",	m_strStaffStartFrom );
	m_nNewPupilIntakeIndex	= file.GetInt	 ( "IntakePupil",	m_nNewPupilIntakeIndex );
	m_nNewStaffIntakeIndex	= file.GetInt	 ( "IntakeStaff",	m_nNewStaffIntakeIndex );


	return bReply;
}

//*********************************************************************

bool CWondeData::Save()
{
	CSSIniFile file;

	file.Set ( "TestMode",		m_bTestMode );
	file.Set ( "ImportFile",	m_strImportFilename );
	file.Set ( "WebIndex1",		m_nWebPaymentIndex1 );
	file.Set ( "WebIndex2",		m_nWebPaymentIndex2 );
	file.Set ( "LocateOn",		m_nLocateOnIndex );
	file.Set ( "DeleteImport",	m_bDeleteImportFile );
	file.Set ( "StaffGroup",	m_nStaffGroupNo );
	file.Set ( "FSMGroup",		m_nFSMGroupNo );
	file.Set ( "FSMRevert",		m_nFSMRevertGroupNo );
	file.Set ( "MISNo",			m_nMISIDUserTextNo );
	file.Set ( "UPNNo",			m_nUPNUserTextNo );
	file.Set ( "AdNo",			m_nAdNoUserTextNo );
	file.Set ( "YearNo",		m_nYearUserTextNo );
	file.Set ( "RegNo",			m_nRegUserTextNo );
	file.Set ( "HouseNo",		m_nHouseUserTextNo );
	file.Set ( "ImportMethod",	m_nImportMethod );
	file.Set ( "SaveEmail1",	m_bSavePupilEmail );
	file.Set ( "Email1No",		m_nEmailUserTextNo );								// 0=email field, else info1=8
	file.Set ( "SaveEmail2",	m_bSaveStaffEmail );
	file.Set ( "Email2No",		m_nEmail2UserTextNo );								// 0=email field, else info1=8
	file.Set ( "PhotoFolder",	m_strPhotoFolder );
	file.Set ( "DeletePhoto",	m_bDeletePhotoFile );
	file.Set ( "NoStaff",		m_bDisableStaffImport );
	file.Set ( "StartPupil",	m_strPupilStartFrom );
	file.Set ( "StartStaff",	m_strStaffStartFrom );
	file.Set ( "IntakePupil",	m_nNewPupilIntakeIndex );
	file.Set ( "IntakeStaff",	m_nNewStaffIntakeIndex );

	return file.Write ( m_strFilename );
}

//*********************************************************************

const char* CWondeData::GetWebPaymentText ( int nIndex )
{
	if ( nIndex == nWONDE_WEBPAYMENT_NONE_INDEX )	return "None";			// Pupil \ Staff
	if ( nIndex == nWONDE_WEBPAYMENT_MIS_INDEX )	return "MIS_ID";		// Pupil\ Staff
	if ( nIndex == nWONDE_WEBPAYMENT_UPN_INDEX )	return "UPN";			// Pupil only
	if ( nIndex == nWONDE_WEBPAYMENT_ADNO_INDEX )	return "Admin No";		// Pupil only
	return "";
}

//*********************************************************************

const char* CWondeData::GetNewIntakeText ( int nIndex )
{
	if ( nIndex == nWONDE_NEWINTAKE_MIS_INDEX)			return "MIS_ID";
	if ( nIndex == nWONDE_NEWINTAKE_ADNO_INDEX )		return "Admin No";
	if ( nIndex == nWONDE_NEWINTAKE_NEXTFREE_INDEX )	return "Next Free";
	return "";
}

//*********************************************************************

const char* CWondeData::GetMISIDTextLabel()		{ return GetTextLabel( m_nMISIDUserTextNo ); }	//
const char* CWondeData::GetAdminNoTextLabel()	{ return GetTextLabel( m_nAdNoUserTextNo ); }	//
const char* CWondeData::GetUPNTextLabel()		{ return GetTextLabel( m_nUPNUserTextNo ); }	// 0=NotDefined,1=Member#,2-9=info1=8

const char* CWondeData::GetTextLabel ( int nInfo )
{
	if ( nInfo == 1 )	return Account::Info1.Label;
	if ( nInfo == 2 )	return Account::Info2.Label;
	if ( nInfo == 3 )	return Account::Info3.Label;
	if ( nInfo == 4 )	return Account::Info4.Label;
	if ( nInfo == 5 )	return Account::Info5.Label;
	if ( nInfo == 6 )	return Account::Info6.Label;
	if ( nInfo == 7 )	return Account::Info7.Label;
	if ( nInfo == 8 )	return Account::Info8.Label;

	return "";
}

//*********************************************************************

const char* CWondeData::GetLocateOnLabel()
{
	if ( m_nLocateOnIndex == nWONDE_LOCATEON_MIS_INDEX )		return GetMISIDTextLabel();
	if ( m_nLocateOnIndex == nWONDE_LOCATEON_ADNO_INDEX )		return GetAdminNoTextLabel();
	if ( m_nLocateOnIndex == nWONDE_LOCATEON_UPN_INDEX )		return GetUPNTextLabel();
	return "";
}

//*********************************************************************

const char* CWondeData::GetLocateOnText ( int nIndex )
{
	if ( nIndex == nWONDE_LOCATEON_MIS_INDEX )	return "MIS_ID";			// Pupil\ Staff
	if ( nIndex == nWONDE_LOCATEON_ADNO_INDEX )	return "Admin No";			// Pupil only
	if ( nIndex == nWONDE_LOCATEON_UPN_INDEX )	return "UPN";				// Pupil only
	return "";
}
