//***************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//***************************************************************
#include "..\SmartPay4ManagerBgnd\ImportWondeFile.h"
#include "..\SmartPay4ManagerBgnd\ImportWondePhotoFiles.h"
//***************************************************************
#include "SetupWondeDlg.h"
//***************************************************************
static const char* szNOTDEFINED	= "Not defined";
//***************************************************************

CSetupWondeDlg::CSetupWondeDlg( CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupWondeDlg::IDD, pParent)
{
	m_strPhotoFolder = "";
	m_strImportFile = "";
	m_nImportMethod = 0;
	m_bDeleteImportFile = FALSE;
	m_bTestMode = FALSE;
	m_nMISIDIndex = 0;
	m_nAdnoTextIndex = 0;
	m_nUPNTextIndex = 0;
	m_nYearTextIndex = 0;
	m_nRegTextIndex = 0;
	m_nHouseTextIndex = 0;
	m_bSaveEmail = FALSE;
	m_nEmailTextIndex = 0;
	m_bSaveEmail2 = FALSE;
	m_nEmail2TextIndex = 0;
	m_bDeletePhotoFile = FALSE;
	m_nWebPaymentIndex1 = 0;
	m_nWebPaymentIndex2 = 0;
	m_bDisableStaffImport = FALSE;
	m_nLocateOnIndex = 0;	// 0=MIS_ID, 1=
	m_strPupilStartFrom = "";
	m_strStaffStartFrom = "";
	m_nNewIntakePupil = 0;
	m_nNewIntakeStaff = 0;
	m_nFSMGroupNo = 0;
	m_nFSMRevertGroupNo = 0;
	m_nStaffGroupNo = 0;
}

//***************************************************************

void CSetupWondeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_CHECK_SAVEEMAIL2, m_checkSaveEmail2);
	DDX_Control(pDX, IDC_CHECK_TESTMODE, m_checkTestMode);
	DDX_Control(pDX, IDC_EDIT_CARDNO_PUPIL, m_editCardNoPupil);
	DDX_Control(pDX, IDC_EDIT_CARDNO_STAFF, m_editCardNoStaff);
	DDX_Control(pDX, IDC_EDIT_IMPORTFILE, m_editImportFile);
	DDX_Control(pDX, IDC_EDIT_STAFFGROUPNO, m_editStaffGroupNo);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILE, m_strImportFile);
	DDX_Control(pDX, IDC_COMBO_SIMS, m_comboSIMS);
	DDX_CBIndex(pDX, IDC_COMBO_SIMS, m_nAdnoTextIndex);
	DDX_Control(pDX, IDC_COMBO_UPN, m_comboUPN);
	DDX_CBIndex(pDX, IDC_COMBO_UPN, m_nUPNTextIndex);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_comboYear);
	DDX_CBIndex(pDX, IDC_COMBO_YEAR, m_nYearTextIndex);
	DDX_Control(pDX, IDC_COMBO_REG, m_comboReg);
	DDX_CBIndex(pDX, IDC_COMBO_REG, m_nRegTextIndex);
	DDX_Control(pDX, IDC_COMBO_HOUSE, m_comboHouse);
	DDX_CBIndex(pDX, IDC_COMBO_HOUSE, m_nHouseTextIndex);
	DDX_Text(pDX, IDC_EDIT_GROUPNO, m_nFSMGroupNo);
	DDX_Text(pDX, IDC_EDIT_REVERTGROUPNO, m_nFSMRevertGroupNo);
	DDX_Text(pDX, IDC_EDIT_STAFFGROUPNO, m_nStaffGroupNo);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteImportFile);
	DDX_Check(pDX, IDC_CHECK_TESTMODE, m_bTestMode);
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
	DDX_Check(pDX, IDC_CHECK_SAVEEMAIL, m_bSaveEmail);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_CBIndex(pDX, IDC_COMBO_EMAIL, m_nEmailTextIndex);
	DDX_Check(pDX, IDC_CHECK_SAVEEMAIL2, m_bSaveEmail2);
	DDX_Control(pDX, IDC_COMBO_EMAIL5, m_comboEmail2);
	DDX_CBIndex(pDX, IDC_COMBO_EMAIL5, m_nEmail2TextIndex);
	DDX_Text(pDX, IDC_EDIT_PHOTOFOLDER, m_strPhotoFolder);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE2, m_bDeletePhotoFile);
	DDX_Control(pDX, IDC_COMBO_WEBPAYMENT, m_comboWebPayment1);
	DDX_CBIndex(pDX, IDC_COMBO_WEBPAYMENT, m_nWebPaymentIndex1);
	DDX_Control(pDX, IDC_COMBO_WEBPAYMENT2, m_comboWebPayment2);
	DDX_CBIndex(pDX, IDC_COMBO_WEBPAYMENT2, m_nWebPaymentIndex2);
	DDX_Control(pDX, IDC_COMBO_MISID, m_comboMISID);
	DDX_CBIndex(pDX, IDC_COMBO_MISID, m_nMISIDIndex);
	DDX_Check(pDX, IDC_CHECK_DISABLESTAFF, m_bDisableStaffImport);
	DDX_Control(pDX, IDC_COMBO_LOCATEON, m_comboLocateOn);
	DDX_CBIndex(pDX, IDC_COMBO_LOCATEON, m_nLocateOnIndex);
	DDX_Text(pDX, IDC_EDIT_CARDNO_PUPIL, m_strPupilStartFrom);
	DDX_Text(pDX, IDC_EDIT_CARDNO_STAFF, m_strStaffStartFrom);
	DDX_CBIndex(pDX, IDC_COMBO_NEWINTAKEPUPIL, m_nNewIntakePupil);
	DDX_CBIndex(pDX, IDC_COMBO_NEWINTAKESTAFF, m_nNewIntakeStaff);
	DDX_Control(pDX, IDC_COMBO_NEWINTAKEPUPIL, m_comboNewIntakePupil);
	DDX_Control(pDX, IDC_COMBO_NEWINTAKESTAFF, m_comboNewIntakeStaff);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//***************************************************************

BEGIN_MESSAGE_MAP(CSetupWondeDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, OnButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_VIEW2, OnButtonView2)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILE, OnKillFocusImportFile)
	ON_BN_CLICKED(IDC_CHECK_SAVEEMAIL, OnToggleSaveEmail)
	ON_BN_CLICKED(IDC_CHECK_SAVEEMAIL2, OnToggleSaveEmail2)
	ON_BN_CLICKED(IDC_CHECK_DISABLESTAFF, OnToggleDisableStaff)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWINTAKEPUPIL, OnSelectNewIntakePupil)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWINTAKESTAFF, OnSelectNewIntakeStaff)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//***********************************************************************

void CSetupWondeDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();

	m_bTestMode = m_data.m_bTestMode;
	m_strImportFile = m_data.m_strImportFilename;
	m_bDeleteImportFile = m_data.m_bDeleteImportFile;
	m_nMISIDIndex = m_data.m_nMISIDUserTextNo;
	m_nAdnoTextIndex = m_data.m_nAdNoUserTextNo;
	m_nUPNTextIndex = m_data.m_nUPNUserTextNo;
	m_nYearTextIndex = m_data.m_nYearUserTextNo;
	m_nRegTextIndex = m_data.m_nRegUserTextNo;
	m_nHouseTextIndex = m_data.m_nHouseUserTextNo;
	m_nFSMGroupNo = m_data.m_nFSMGroupNo;
	m_nFSMRevertGroupNo = m_data.m_nFSMRevertGroupNo;
	m_bDisableStaffImport = m_data.m_bDisableStaffImport;
	m_nStaffGroupNo = m_data.m_nStaffGroupNo;
	m_nImportMethod = m_data.m_nImportMethod;
	m_nEmailTextIndex = m_data.m_nEmailUserTextNo;
	m_bSaveEmail = m_data.m_bSavePupilEmail;
	m_nEmail2TextIndex = m_data.m_nEmail2UserTextNo;
	m_bSaveEmail2 = m_data.m_bSaveStaffEmail;
	m_strPhotoFolder = m_data.m_strPhotoFolder;
	m_bDeletePhotoFile = m_data.m_bDeletePhotoFile;
	m_nWebPaymentIndex1 = m_data.m_nWebPaymentIndex1;		// pupil web ref
	m_nWebPaymentIndex2 = m_data.m_nWebPaymentIndex2;		// staff web ref
	m_nLocateOnIndex = m_data.m_nLocateOnIndex;
	m_strPupilStartFrom = m_data.m_strPupilStartFrom;
	m_strStaffStartFrom = m_data.m_strStaffStartFrom;
	m_nNewIntakePupil = m_data.m_nNewPupilIntakeIndex;
	m_nNewIntakeStaff = m_data.m_nNewStaffIntakeIndex;
}

//***********************************************************************

BOOL CSetupWondeDlg::CSSAutoShutdownPostInitDialog()
{
	m_comboMISID.AddString ( szNOTDEFINED );
	m_comboUPN.AddString ( szNOTDEFINED );
	m_comboSIMS.AddString ( szNOTDEFINED );
	m_comboYear.AddString ( szNOTDEFINED );
	m_comboReg.AddString ( szNOTDEFINED );
	m_comboHouse.AddString ( szNOTDEFINED );
	m_comboEmail.AddString ( Account::Email.Label );			// 0=Email field, else infor 1-8
	m_comboEmail2.AddString ( Account::Email.Label );			// 0=Email field, else infor 1-8

	CString strDisplayText = "";
	
// add info texts

	for ( int n = 1 ; n <= nMAX_INFOTEXTS ; n++ )
	{
		CSQLRepositoryUserText RepoText;
		strDisplayText = RepoText.GetDisplayInfoText(n);

		m_comboMISID.AddString ( strDisplayText );
		m_comboUPN.AddString ( strDisplayText );
		m_comboSIMS.AddString ( strDisplayText );
		m_comboYear.AddString ( strDisplayText );
		m_comboReg.AddString ( strDisplayText );
		m_comboHouse.AddString ( strDisplayText );
		m_comboEmail.AddString ( strDisplayText );
		m_comboEmail2.AddString ( strDisplayText );
	}

	m_comboMISID.SetCurSel ( m_nMISIDIndex );
	m_comboUPN.SetCurSel ( m_nUPNTextIndex );
	m_comboSIMS.SetCurSel ( m_nAdnoTextIndex );
	m_comboYear.SetCurSel ( m_nYearTextIndex );
	m_comboReg.SetCurSel ( m_nRegTextIndex );
	m_comboHouse.SetCurSel ( m_nHouseTextIndex );
	m_comboEmail.SetCurSel ( m_nEmailTextIndex );
	m_comboEmail2.SetCurSel ( m_nEmail2TextIndex );

	if ( m_bTestMode == FALSE )
	{
		m_checkTestMode.ShowWindow(SW_HIDE);
	}

	m_comboLocateOn.AddString ( m_data.GetLocateOnText ( nWONDE_LOCATEON_MIS_INDEX ) );
	m_comboLocateOn.AddString ( m_data.GetLocateOnText ( nWONDE_LOCATEON_ADNO_INDEX ) );
	m_comboLocateOn.AddString ( m_data.GetLocateOnText ( nWONDE_LOCATEON_UPN_INDEX ) );
	m_comboLocateOn.SetCurSel ( m_nLocateOnIndex );

	m_comboWebPayment1.AddString ( m_data.GetWebPaymentText ( nWONDE_WEBPAYMENT_NONE_INDEX ) );
	m_comboWebPayment1.AddString ( m_data.GetWebPaymentText ( nWONDE_WEBPAYMENT_MIS_INDEX ) );
	m_comboWebPayment1.AddString ( m_data.GetWebPaymentText ( nWONDE_WEBPAYMENT_UPN_INDEX ) );
	m_comboWebPayment1.AddString ( m_data.GetWebPaymentText ( nWONDE_WEBPAYMENT_ADNO_INDEX ) );
	m_comboWebPayment1.SetCurSel ( m_nWebPaymentIndex1 );

	m_comboWebPayment2.AddString ( m_data.GetWebPaymentText ( nWONDE_WEBPAYMENT_NONE_INDEX ) );
	m_comboWebPayment2.AddString ( m_data.GetWebPaymentText ( nWONDE_WEBPAYMENT_MIS_INDEX ) );
	m_comboWebPayment2.SetCurSel ( m_nWebPaymentIndex2 );

	m_comboNewIntakePupil.AddString ( m_data.GetNewIntakeText ( nWONDE_NEWINTAKE_MIS_INDEX) );
	m_comboNewIntakePupil.AddString ( m_data.GetNewIntakeText ( nWONDE_NEWINTAKE_ADNO_INDEX	 ) );
	m_comboNewIntakePupil.AddString ( m_data.GetNewIntakeText ( nWONDE_NEWINTAKE_NEXTFREE_INDEX ) );
	m_comboNewIntakePupil.SetCurSel ( m_nNewIntakePupil );
	
	m_comboNewIntakeStaff.AddString ( m_data.GetNewIntakeText ( nWONDE_NEWINTAKE_MIS_INDEX) );
	m_comboNewIntakeStaff.AddString ( m_data.GetNewIntakeText ( nWONDE_NEWINTAKE_ADNO_INDEX	 ) );
	m_comboNewIntakeStaff.AddString ( m_data.GetNewIntakeText ( nWONDE_NEWINTAKE_NEXTFREE_INDEX ) );
	m_comboNewIntakeStaff.SetCurSel ( m_nNewIntakeStaff );
	
	SetDisplay();

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//*****************************************************************************

void CSetupWondeDlg::OnToggleDisableStaff()	
{ 
	SetDisplay(); 
}

//*******************************************************************

void CSetupWondeDlg::OnToggleSaveEmail()
{
	SetDisplay(); 
	m_comboEmail.SetFocus();
}

//*******************************************************************

void CSetupWondeDlg::OnToggleSaveEmail2()
{
	SetDisplay(); 
	m_comboEmail2.SetFocus();
}

//*******************************************************************

void CSetupWondeDlg::OnSelectNewIntakePupil()
{
	SetDisplay();

	if (m_nNewIntakePupil == nWONDE_NEWINTAKE_NEXTFREE_INDEX)
	{
		m_editCardNoPupil.SetFocus();
	}
}

//*******************************************************************

void CSetupWondeDlg::OnSelectNewIntakeStaff()
{
	SetDisplay();

	if (m_nNewIntakeStaff == nWONDE_NEWINTAKE_NEXTFREE_INDEX)
	{
		m_editCardNoStaff.SetFocus();
	}
}

//*******************************************************************

void CSetupWondeDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ::FileExists ( m_strImportFile);
		m_buttonView.EnableWindow(bAllowed);

// pupil data

		m_comboEmail.EnableWindow(m_bSaveEmail);
		m_comboEmail2.EnableWindow(m_bSaveEmail2);

		bAllowed = ( m_nNewIntakePupil == nWONDE_NEWINTAKE_NEXTFREE_INDEX ) ? TRUE : FALSE;
		m_editCardNoPupil.EnableWindow(bAllowed );

// staff data

		bAllowed = !m_bDisableStaffImport;
		m_comboNewIntakeStaff.EnableWindow(bAllowed);
		m_editCardNoStaff.EnableWindow(bAllowed);
		m_editStaffGroupNo.EnableWindow(bAllowed);
		m_comboWebPayment2.EnableWindow(bAllowed);
		m_checkSaveEmail2.EnableWindow(bAllowed);
		m_comboEmail2.EnableWindow(bAllowed);

		if (bAllowed == TRUE && m_nNewIntakeStaff != nWONDE_NEWINTAKE_NEXTFREE_INDEX)
		{
			bAllowed = FALSE;
		}

		m_editCardNoStaff.EnableWindow(bAllowed);
	}
}

//*******************************************************************

bool CSetupWondeDlg::ValidateInput()
{
	if ( m_nMISIDIndex == 0 && m_nAdnoTextIndex == 0 && m_nUPNTextIndex == 0 )
	{
		if ( Prompter.YesNo( "No database fields have been defined for saving MIS_ID, Adno or UPN is this correct?", "Verification Required" ) == IDNO )
		{
			m_comboSIMS.SetFocus();
			return FALSE;
		}
	}

	CString strError = "";

	if ( m_nMISIDIndex == 0 && m_nLocateOnIndex == nWONDE_LOCATEON_MIS_INDEX )
	{
		strError = "Database field to save MIS_ID value has not been defined!";
		m_comboMISID.SetFocus();
	}

	if ( m_nAdnoTextIndex == 0 && m_nLocateOnIndex == nWONDE_LOCATEON_ADNO_INDEX )
	{
		strError = "Database field to save Admin No value has not been defined!";
		m_comboSIMS.SetFocus();
	}

	if ( m_nUPNTextIndex == 0 && m_nLocateOnIndex == nWONDE_LOCATEON_UPN_INDEX )
	{
		strError = "Database field to save UPN value has not been defined!";
		m_comboUPN.SetFocus();
	}


	if ( m_nMISIDIndex != 0 && m_nAdnoTextIndex != 0 )
	{
		if ( m_nMISIDIndex == m_nAdnoTextIndex )
		{
			strError = "Conflict between MIS_ID and Admin No database Info fields!";
			m_comboSIMS.SetFocus();
		}
	}

	if ( m_nMISIDIndex != 0 && m_nUPNTextIndex != 0 )
	{
		if ( m_nMISIDIndex == m_nUPNTextIndex )
		{
			strError = "Conflict between MIS_ID and UPN database Info fields!";
			m_comboSIMS.SetFocus();
		}
	}

	if ( m_nAdnoTextIndex != 0 && m_nUPNTextIndex != 0 )
	{
		if ( m_nAdnoTextIndex == m_nUPNTextIndex )
		{
			strError = "Conflict between Admin No and UPN database Info fields!";
			m_comboSIMS.SetFocus();
		}
	}


	if ( m_nYearTextIndex != 0 && ( m_nYearTextIndex == m_nRegTextIndex ) )
	{
		strError = "Conflict between Year and Reg fields specified!";
		m_comboYear.SetFocus();
	}

	if ( m_bDisableStaffImport == FALSE )
	{
		if ( m_nLocateOnIndex == nWONDE_LOCATEON_ADNO_INDEX )
		{
			if (Prompter.YesNo("Are you sure you wish to locate staff accounts using the Admin No field (y/n)", "Please confirm", TRUE) == IDNO)
			{
				return FALSE;
			}
		}

		if (m_nLocateOnIndex == nWONDE_LOCATEON_UPN_INDEX)
		{
			if (Prompter.YesNo("Are you sure you wish to locate staff accounts using the UPN field (y/n)", "Please confirm", TRUE) == IDNO)
			{
				return FALSE;
			}
		}


		if ( m_nNewIntakeStaff == nWONDE_NEWINTAKE_ADNO_INDEX )
		{
			if ( Prompter.YesNo( "Are you sure you wish to add new staff accounts using Admin No data (y/n)", "Please confirm", TRUE ) == IDNO )
			{
				m_comboNewIntakeStaff.SetFocus();
				return FALSE;
			}
		}
	}


	if ( m_strImportFile == "" )
	{
		if ( Prompter.YesNo( "No import filename has been specified'!\n\nIs this correct (y/n)", "Please confirm", TRUE ) == IDNO )
		{
			m_editImportFile.SetFocus();
			return FALSE;
		}
	}
	
	if ( strError != "" )
	{
		Prompter.Error( strError, "Entry Error" );
		return FALSE;
	}

	return TRUE;
}

//*******************************************************************************

void CSetupWondeDlg::OnKillFocusImportFile()
{
	if (UpdateData() == TRUE)
	{
		SetDisplay();
	}
}

//*******************************************************************************

void CSetupWondeDlg::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CFileDialog dlg ( TRUE, "json", "", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "Import file (*.json)|*.json|All Files (*.*)|*.*||", this );
		dlg.m_ofn.lpstrTitle = "Locate Wonde JSON Import file";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if ( DoFileDialog(dlg) == IDOK )
		{
			m_strImportFile = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//*******************************************************************************

void CSetupWondeDlg::OnButtonBrowse2()
{
	if ( UpdateData() == TRUE )
	{
		CString strPathname;
		if ( DoBrowseFolder ( strPathname, "Locate Wonde photo folder",  NULL, this ) == TRUE )
		{
			m_strPhotoFolder = strPathname;
			UpdateData(FALSE);
		}
	}
}

//*******************************************************************************

void CSetupWondeDlg::OnButtonView()	
{ 
	DisplayFile(0); 
}

//*******************************************************************************

void CSetupWondeDlg::OnButtonView2()	
{ 
	DisplayFile(1); 
}

//*******************************************************************************

void CSetupWondeDlg::DisplayFile(int nType)
{
	if (UpdateData() == TRUE)
	{
		if (ValidateInput() == FALSE)
		{
			return;
		}

		CWondeData data;									// make temp data for display function
		data.m_bTestMode = (m_bTestMode != 0);
		data.m_strImportFilename = m_strImportFile;
		data.m_nWebPaymentIndex1 = m_nWebPaymentIndex1;
		data.m_nWebPaymentIndex2 = m_nWebPaymentIndex2;
		data.m_nLocateOnIndex = m_nLocateOnIndex;
		data.m_bDeleteImportFile = (m_bDeleteImportFile != 0);
		data.m_nMISIDUserTextNo = m_nMISIDIndex;
		data.m_nAdNoUserTextNo = m_nAdnoTextIndex;
		data.m_nUPNUserTextNo = m_nUPNTextIndex;
		data.m_nYearUserTextNo = m_nYearTextIndex;
		data.m_nRegUserTextNo = m_nRegTextIndex;
		data.m_nHouseUserTextNo = m_nHouseTextIndex;
		data.m_nFSMGroupNo = m_nFSMGroupNo;
		data.m_nFSMRevertGroupNo = m_nFSMRevertGroupNo;
		data.m_bDisableStaffImport = (m_bDisableStaffImport != 0);
		data.m_nStaffGroupNo = m_nStaffGroupNo;
		data.m_nImportMethod = m_nImportMethod;
		data.m_nEmailUserTextNo = m_nEmailTextIndex;
		data.m_bSavePupilEmail = (m_bSaveEmail != 0);
		data.m_nEmail2UserTextNo = m_nEmail2TextIndex;
		data.m_bSaveStaffEmail = (m_bSaveEmail2 != 0);
		data.m_strPhotoFolder = m_strPhotoFolder;
		data.m_bDeletePhotoFile = (m_bDeletePhotoFile != 0);
		data.m_strPupilStartFrom = m_strPupilStartFrom;
		data.m_strStaffStartFrom = m_strStaffStartFrom;
		data.m_nNewPupilIntakeIndex = m_nNewIntakePupil;
		data.m_nNewStaffIntakeIndex = m_nNewIntakeStaff;

		if (nType == 1)			// display photo list
		{
			CImportWondePhotoFiles wonde(&data, this);
			wonde.ShowImports();
		}
		else						// display JSON file
		{
			CImportWondeFile wondeFile(&data, this);
			wondeFile.ShowImports(m_strImportFile);
		}
	}
}

//*******************************************************************************

void CSetupWondeDlg::OnButtonAltKey1()
{
	m_checkTestMode.ShowWindow(SW_SHOW);
}

//*******************************************************************************

void CSetupWondeDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (ValidateInput() == FALSE)
		{
			return;
		}

		m_data.m_bTestMode = (m_bTestMode != 0);
		m_data.m_strImportFilename = m_strImportFile;
		m_data.m_nWebPaymentIndex1 = m_nWebPaymentIndex1;
		m_data.m_nWebPaymentIndex2 = m_nWebPaymentIndex2;
		m_data.m_nLocateOnIndex = m_nLocateOnIndex;
		m_data.m_bDeleteImportFile = (m_bDeleteImportFile != 0);
		m_data.m_nMISIDUserTextNo = m_nMISIDIndex;
		m_data.m_nAdNoUserTextNo = m_nAdnoTextIndex;
		m_data.m_nUPNUserTextNo = m_nUPNTextIndex;
		m_data.m_nYearUserTextNo = m_nYearTextIndex;
		m_data.m_nRegUserTextNo = m_nRegTextIndex;
		m_data.m_nHouseUserTextNo = m_nHouseTextIndex;
		m_data.m_nFSMGroupNo = m_nFSMGroupNo;
		m_data.m_nFSMRevertGroupNo = m_nFSMRevertGroupNo;
		m_data.m_bDisableStaffImport = (m_bDisableStaffImport != 0);
		m_data.m_nStaffGroupNo = m_nStaffGroupNo;
		m_data.m_nImportMethod = m_nImportMethod;
		m_data.m_nEmailUserTextNo = m_nEmailTextIndex;
		m_data.m_bSavePupilEmail = (m_bSaveEmail != 0);
		m_data.m_nEmail2UserTextNo = m_nEmail2TextIndex;
		m_data.m_bSaveStaffEmail = (m_bSaveEmail2 != 0);
		m_data.m_strPhotoFolder = m_strPhotoFolder;
		m_data.m_bDeletePhotoFile = (m_bDeletePhotoFile != 0);
		m_data.m_strPupilStartFrom = m_strPupilStartFrom;
		m_data.m_strStaffStartFrom = m_strStaffStartFrom;
		m_data.m_nNewPupilIntakeIndex = m_nNewIntakePupil;
		m_data.m_nNewStaffIntakeIndex = m_nNewIntakeStaff;

		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************************
