//***************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//***************************************************************
#include "..\SmartPay4ManagerBgnd\ImportSimsFile.h"
#include "..\SmartPay4ManagerBgnd\ImportSimsPhotoFiles.h"
//***************************************************************
#include "ReportCSVFile.h"
//***************************************************************
#include "SetupSimsDlg.h"
//***************************************************************
static const char* szNOTDEFINED	= "Not defined";
static const char* szERROR1		= "Conflict between SIMS Adno and UPN database Info fields!";
static const char* szERROR2		= "Conflict between SIMS Adno and Person_id database Info fields!";
static const char* szERROR3		= "Conflict between UPN and Person_id database Info fields!";
//***************************************************************

CSetupSimsDlg::CSetupSimsDlg ( CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupSimsDlg::IDD, pParent)
{
	m_nAdnoTextIndex = 0;
	m_nUPNTextIndex = 0;
	m_nPIDTextIndex = 0;
	m_nNewIntakeAccount = 0;
	m_nYearTextIndex = 0;
	m_nRegTextIndex = 0;
	m_bSaveEmail = FALSE;
	m_nEmailTextIndex = 0;
	m_nDobTextIndex = 0;
	m_bAddUnknowns = FALSE;
	m_nFSMGroupNo = 0;
	m_nFSMRevertGroupNo = 0;
	m_strSimsImportFile = "";
	m_bDeleteImportFile = FALSE;
	m_strSimsPhotoFolder = "";
	m_bDeletePhotoFile = FALSE;
	m_nLocatePhotoBy = 0;
	m_nLocateBy = 0;
	m_nImportMethod = 0;
	m_strStartFromUserID = "";
}

//***************************************************************

void CSetupSimsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_BUTTON_VIEW2, m_buttonView2);
	DDX_Control(pDX, IDC_CHECK_ADDUNKNOWNS, m_checkAddUnknowns);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_EDIT_SIMSIMPORT, m_editSIMSImport);
	DDX_Control(pDX, IDC_COMBO_MODIFY, m_comboModify);
	DDX_CBIndex(pDX, IDC_COMBO_MODIFY, m_nLocateBy);
	DDX_Control(pDX, IDC_COMBO_SIMS, m_comboSIMS);
	DDX_CBIndex(pDX, IDC_COMBO_SIMS, m_nAdnoTextIndex);
	DDX_Control(pDX, IDC_COMBO_UPN, m_comboUPN);
	DDX_CBIndex(pDX, IDC_COMBO_UPN, m_nUPNTextIndex);
	DDX_Control(pDX, IDC_COMBO_PID, m_comboPID);
	DDX_CBIndex(pDX, IDC_COMBO_PID, m_nPIDTextIndex);
	DDX_Control(pDX, IDC_COMBO_NEWINTAKE, m_comboNewIntake);
	DDX_CBIndex(pDX, IDC_COMBO_NEWINTAKE, m_nNewIntakeAccount);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_comboYear);
	DDX_CBIndex(pDX, IDC_COMBO_YEAR, m_nYearTextIndex);
	DDX_Control(pDX, IDC_COMBO_REG, m_comboReg);
	DDX_CBIndex(pDX, IDC_COMBO_REG, m_nRegTextIndex);
	DDX_Check(pDX, IDC_CHECK_SAVEEMAIL, m_bSaveEmail);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_CBIndex(pDX, IDC_COMBO_EMAIL, m_nEmailTextIndex);
	DDX_Control(pDX, IDC_COMBO_DOB, m_comboDob);
	DDX_CBIndex(pDX, IDC_COMBO_DOB, m_nDobTextIndex);
	DDX_Check(pDX, IDC_CHECK_ADDUNKNOWNS, m_bAddUnknowns);
	DDX_Text(pDX, IDC_EDIT_GROUPNO, m_nFSMGroupNo);
	DDX_Text(pDX, IDC_EDIT_REVERTGROUPNO, m_nFSMRevertGroupNo);
	DDX_Text(pDX, IDC_EDIT_SIMSIMPORT, m_strSimsImportFile);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteImportFile);
	DDX_Text(pDX, IDC_EDIT_PHOTOFOLDER, m_strSimsPhotoFolder);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE2, m_bDeletePhotoFile);
	DDX_Control(pDX, IDC_COMBO_NEWINTAKE2, m_comboLocatePhotoBy);
	DDX_CBIndex(pDX, IDC_COMBO_NEWINTAKE2, m_nLocatePhotoBy);
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_strStartFromUserID);
}

BEGIN_MESSAGE_MAP(CSetupSimsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, OnButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_VIEW2, OnButtonView2)
	ON_BN_CLICKED(IDC_CHECK_ADDUNKNOWNS, OnToggleAddUnknowns)
	ON_BN_CLICKED(IDC_BUTTON_VIEW3, OnButtonView3)
	ON_EN_KILLFOCUS(IDC_EDIT_SIMSIMPORT, OnKillFocusSimsImport)
	ON_BN_CLICKED(IDC_CHECK_SAVEEMAIL, OnToggleSaveEmail)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWINTAKE, OnSelectNewIntake)
END_MESSAGE_MAP()

//***************************************************************

void CSetupSimsDlg::CSSAutoShutdownPreInitDialog()
{
	m_sims.Read();

	m_strSimsImportFile = m_sims.m_strImportFilename;
	m_strSimsPhotoFolder = m_sims.m_strSimsPhotoFolder;
	m_bDeleteImportFile = m_sims.m_bDeleteImportFile;
	m_bDeletePhotoFile = m_sims.m_bDeletePhotoFile;
	m_nFSMGroupNo = m_sims.m_nFSMGroupNo;
	m_nFSMRevertGroupNo = m_sims.m_nFSMRevertGroupNo;
	m_nUPNTextIndex = m_sims.m_nUPNUserTextNo;
	m_nAdnoTextIndex = m_sims.m_nSIMSUserTextNo;
	m_nPIDTextIndex = m_sims.m_nPIDUserTextNo;
	m_nYearTextIndex = m_sims.m_nYearContactNo;
	m_nRegTextIndex = m_sims.m_nRegContactNo;
	m_nEmailTextIndex = m_sims.m_nEmailContactNo;
	m_bSaveEmail = m_sims.m_bSaveEmail;
	m_nDobTextIndex = m_sims.m_nDobNo;
	m_nNewIntakeAccount = m_sims.m_nNewIntakeAccount;
	m_nLocateBy = m_sims.m_nLocateBy;
	m_bAddUnknowns = m_sims.m_bAddUnknowns;
	m_nLocatePhotoBy = m_sims.m_nLocatePhotoBy;
	m_nImportMethod = m_sims.m_nImportMethod;
	m_strStartFromUserID = m_sims.m_strStartFromUserID;
}

//***************************************************************

BOOL CSetupSimsDlg::CSSAutoShutdownPostInitDialog()
{
// user texts

	m_comboPID.AddString ( szNOTDEFINED );
	m_comboUPN.AddString ( szNOTDEFINED );
	m_comboSIMS.AddString ( szNOTDEFINED );
	m_comboYear.AddString ( szNOTDEFINED );
	m_comboReg.AddString ( szNOTDEFINED );
	m_comboEmail.AddString ( Account::Email.Label );					// 0=Email field, else infor 1-8
	m_comboDob.AddString ( szNOTDEFINED );

	CString strDisplayText;

// add MemberID 

	CSQLRepositoryUserText RepoText;

	strDisplayText = RepoText.GetDisplayMemberText();
	m_comboPID.AddString ( strDisplayText );
	m_comboUPN.AddString ( strDisplayText );
	m_comboSIMS.AddString ( strDisplayText );

// add info texts

	for ( int n = 1 ; n <= nMAX_INFOTEXTS ; n++ )
	{
		strDisplayText = RepoText.GetDisplayInfoText(n);

		m_comboPID.AddString ( strDisplayText );
		m_comboUPN.AddString ( strDisplayText );
		m_comboSIMS.AddString ( strDisplayText );
		m_comboYear.AddString ( strDisplayText );
		m_comboReg.AddString ( strDisplayText );
		m_comboEmail.AddString ( strDisplayText );
	}

// dob no

	m_comboDob.AddString ( Account::DOB.Label );

//
	m_comboPID.SetCurSel ( m_nPIDTextIndex );
	m_comboUPN.SetCurSel ( m_nUPNTextIndex );
	m_comboSIMS.SetCurSel ( m_nAdnoTextIndex );
	m_comboYear.SetCurSel ( m_nYearTextIndex );
	m_comboReg.SetCurSel ( m_nRegTextIndex );
	m_comboEmail.SetCurSel ( m_nEmailTextIndex );
	m_comboDob.SetCurSel ( m_nDobTextIndex );

//

	m_comboModify.AddString ( m_sims.GetLocateByText ( nSIMS_LOCATEBY_ADNO_INDEX ) );	// Adno
	m_comboModify.AddString ( m_sims.GetLocateByText ( nSIMS_LOCATEBY_UPN_INDEX ) );	// UPN
	m_comboModify.AddString ( m_sims.GetLocateByText ( nSIMS_LOCATEBY_PID_INDEX ) );	// Person_id
	m_comboModify.SetCurSel ( m_nLocateBy );

	m_comboNewIntake.AddString ( m_sims.GetNewIntakeAddedByText ( nSIMS_NEWINTAKE_ADNO_INDEX ) );		// Adno
	m_comboNewIntake.AddString ( m_sims.GetNewIntakeAddedByText ( nSIMS_NEWINTAKE_PID_INDEX ) );		// Person_id
	m_comboNewIntake.AddString ( m_sims.GetNewIntakeAddedByText ( nSIMS_NEWINTAKE_NEXTFREE_INDEX ) );	// Next free
	m_comboNewIntake.SetCurSel ( m_nNewIntakeAccount );

	m_comboLocatePhotoBy.AddString ( m_sims.GetLocatePhotoByText( nSIMS_LOCATEPHOTOBY_ADNO_INDEX ) );	// Adno
	m_comboLocatePhotoBy.AddString ( m_sims.GetLocatePhotoByText( nSIMS_LOCATEPHOTOBY_UPN_INDEX ) );	// UPN
	m_comboLocatePhotoBy.AddString ( m_sims.GetLocatePhotoByText( nSIMS_LOCATEPHOTOBY_PID_INDEX ) );	// Person_id
	m_comboLocatePhotoBy.SetCurSel ( m_nLocatePhotoBy );

	SetDisplay();

	return TRUE;
}

//*******************************************************************

void CSetupSimsDlg::OnKillFocusSimsImport()
{ 
	SetDisplay();
}

//*******************************************************************

void CSetupSimsDlg::OnSelectNewIntake()	
{ 
	SetDisplay(); 
}

//*******************************************************************

void CSetupSimsDlg::OnToggleAddUnknowns()
{
	SetDisplay(); 
	m_comboNewIntake.SetFocus();
}

//*******************************************************************

void CSetupSimsDlg::OnToggleSaveEmail()
{
	SetDisplay(); 
	m_comboEmail.SetFocus();
}

//*******************************************************************

void CSetupSimsDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ::FileExists ( m_strSimsImportFile );
		m_buttonView.EnableWindow(bAllowed);
		m_buttonView2.EnableWindow(bAllowed);

		bAllowed = ( ( TRUE == m_bAddUnknowns == TRUE) && (nSIMS_NEWINTAKE_NEXTFREE_INDEX == m_nNewIntakeAccount ) ) ? TRUE : FALSE;
		m_editCardNo.EnableWindow(bAllowed);

		m_comboNewIntake.EnableWindow(m_bAddUnknowns);
		m_comboEmail.EnableWindow(m_bSaveEmail);
	}
}


//*******************************************************************

bool CSetupSimsDlg::ValidateInput()
{
	if ( m_nAdnoTextIndex == 0 && m_nUPNTextIndex == 0 && m_nPIDTextIndex == 0 )
	{
		if ( Prompter.YesNo( "No database fields have been defined for saving SIMS Adno, UPN or Person_id is this correct?", "Verification Required" ) == IDNO )
		{
			m_comboSIMS.SetFocus();
			return FALSE;
		}
	}

	CString strError = "";

	if ( m_nAdnoTextIndex != 0 )
	{
		if ( m_nAdnoTextIndex == m_nUPNTextIndex )
		{
			strError = szERROR1;								// "Conflict between SIMS Adno and UPN Database UserText fields!"
			m_comboSIMS.SetFocus();
		}
	
		else if ( m_nAdnoTextIndex == m_nPIDTextIndex )
		{
			strError = szERROR2;								// "Conflict between SIMS Adno and Person_id Database UserText fields!"
			m_comboSIMS.SetFocus();
		}
	}

	if ( m_nUPNTextIndex != 0 )
	{
		if ( m_nUPNTextIndex == m_nAdnoTextIndex )
		{
			strError = szERROR1;								// "Conflict between SIMS Adno and UPN Database UserText fields!";
			m_comboUPN.SetFocus();
		}
	
		else if ( m_nUPNTextIndex == m_nPIDTextIndex )
		{
			strError = szERROR3;								// "Conflict between UPN and Person_id Database UserText fields!"
			m_comboUPN.SetFocus();
		}
	}

	if ( m_nPIDTextIndex != 0 )
	{
		if ( m_nPIDTextIndex == m_nAdnoTextIndex )
		{
			strError = szERROR2;								// "Conflict between SIMS Adno and Person_id Database UserText fields!"
			m_comboPID.SetFocus();
		}
	
		else if ( m_nPIDTextIndex == m_nUPNTextIndex )
		{
			strError = szERROR3;								// "Conflict between UPN Adno and Perosn_id Database UserText fields!";
			m_comboPID.SetFocus();
		}
	}

	if ( m_nLocateBy == nSIMS_LOCATEBY_ADNO_INDEX && m_nAdnoTextIndex == 0 || m_nLocateBy == nSIMS_LOCATEBY_UPN_INDEX && m_nUPNTextIndex == 0 || m_nLocateBy == nSIMS_LOCATEBY_PID_INDEX && m_nPIDTextIndex == 0 )
	{
		strError = "Cannot locate existing accounts as the specified field is currently set to 'Not defined'!";
		m_comboModify.SetFocus();
	}

	if ( m_nLocatePhotoBy == nSIMS_LOCATEPHOTOBY_ADNO_INDEX && m_nAdnoTextIndex == 0 || m_nLocatePhotoBy == nSIMS_LOCATEPHOTOBY_UPN_INDEX && m_nUPNTextIndex == 0 || m_nLocatePhotoBy == nSIMS_LOCATEPHOTOBY_PID_INDEX && m_nPIDTextIndex == 0 )
	{
		strError = "Cannot locate existing photo files as the specified field is currently set to 'Not defined'!";
		m_comboLocatePhotoBy.SetFocus();
	}

	if ( m_nYearTextIndex != 0 && ( m_nYearTextIndex == m_nRegTextIndex ) )
	{
		strError = "Conflict between Year and Reg fields specified!";
		m_comboYear.SetFocus();
	}

	if ( m_nYearTextIndex != 0 && ( m_nYearTextIndex == m_nEmailTextIndex ) )
	{
		strError = "Conflict between Year and Email fields specified!";
		m_comboYear.SetFocus();
	}

	if ( m_nRegTextIndex != 0 && ( m_nRegTextIndex == m_nEmailTextIndex ) )
	{
		strError = "Conflict between Reg and Email fields specified!";
		m_comboReg.SetFocus();
	}

	if ( m_nImportMethod == nIMPORT_METHOD_EXISTONLY && m_bAddUnknowns == TRUE )
	{
		strError = "Conflict between options 'Import existing only' and 'Create account if not found'!";
		m_checkAddUnknowns.SetFocus();
	}

	if ( m_nImportMethod == nIMPORT_METHOD_NEWONLY && m_bAddUnknowns == FALSE )
	{
		strError = "'Create account if not found' option not specified!";
		m_checkAddUnknowns.SetFocus();
	}

	if ( m_strSimsImportFile == "" )
	{
		strError = "No import filename specified'!";
		m_editSIMSImport.SetFocus();
	}

	if ( m_bAddUnknowns == TRUE && m_nNewIntakeAccount == nSIMS_NEWINTAKE_NEXTFREE_INDEX )
	{
		if ( System.IsValidCardNo ( m_strStartFromUserID ) == FALSE )
		{
			strError = "Invalid 'Start from' UserID specified!";
			m_editCardNo.SetFocus();
		}
		else
		{
			Server.Read ( Filenames.GetServerDataFilename() );

			CString strOutputCardNo;
			if ( Server.ValidateUserID ( m_strStartFromUserID, strOutputCardNo ) == FALSE )
			{
				strError = Server.GetError();
				m_editCardNo.SetFocus();
			}
		}
	}


	if ( strError != "" )
	{
		Prompter.Error( strError, "Entry Error" );
		return FALSE;
	}

	if ( m_bAddUnknowns == TRUE )
	{
		if ( m_nNewIntakeAccount == nSIMS_NEWINTAKE_ADNO_INDEX && m_nAdnoTextIndex != 0 || m_nNewIntakeAccount == nSIMS_NEWINTAKE_PID_INDEX && m_nPIDTextIndex != 0 )
		{
			if ( Prompter.YesNo( "Create account field is also saved in account user text field!\n\nIs this correct (y/n)", "Please confirm", TRUE ) == IDNO )
			{
				m_comboNewIntake.SetFocus();
				return FALSE;
			}
		}
	}

	return TRUE;
}

//*******************************************************************

void CSetupSimsDlg::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CFileDialog dlg ( TRUE, "csv", szFILENAME_IMPORTSIMS, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "Sims import file (*.csv)|*.csv|All Files (*.*)|*.*||", this );
		dlg.m_ofn.lpstrTitle = "Specify SIMS CSV Import file";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if ( DoFileDialog(dlg) == IDOK )
		{
			m_strSimsImportFile = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//*******************************************************************

void CSetupSimsDlg::OnButtonBrowse2()
{
	if ( UpdateData() == TRUE )
	{
		CString strPathname;
		if ( DoBrowseFolder ( strPathname, "Locate SIMS photo folder",  NULL, this ) == TRUE )
		{
			m_strSimsPhotoFolder = strPathname;
			UpdateData(FALSE);
		}
	}
}

//*******************************************************************

void CSetupSimsDlg::OnButtonDefault()
{
	if (UpdateData() == TRUE)
	{
		m_strSimsImportFile = Filenames.GetImportsPath(szFILENAME_IMPORTSIMS);
		m_strSimsPhotoFolder = Filenames.GetImportsPath("SimsPhoto");
		UpdateData(FALSE);
		SetDisplay();
	}
}

//*******************************************************************

void CSetupSimsDlg::OnButtonView()
{ 
	DisplayFile(0);
}

//*******************************************************************

void CSetupSimsDlg::OnButtonView2()
{ 
	DisplayFile(1); 
}

//*******************************************************************

void CSetupSimsDlg::OnButtonView3()	
{ 
	DisplayFile(2); 
}

//*******************************************************************

void CSetupSimsDlg::DisplayFile(int nType)
{
	if (UpdateData() == TRUE)
	{
		if (ValidateInput() == FALSE)
		{
			return;
		}

		if (nType == 0 || nType == 1)
		{
			if (::FileExists(m_strSimsImportFile) == FALSE)
			{
				Prompter.Error(m_strSimsImportFile, "File not found");
				return;
			}
		}

		CSimsData sims;									// make temp sims data for display function
		sims.m_strImportFilename = m_strSimsImportFile;
		sims.m_strSimsPhotoFolder = m_strSimsPhotoFolder;
		sims.m_bDeleteImportFile = (m_bDeleteImportFile != 0);
		sims.m_bDeletePhotoFile = (m_bDeletePhotoFile != 0);
		sims.m_nFSMGroupNo = m_nFSMGroupNo;
		sims.m_nFSMRevertGroupNo = m_nFSMRevertGroupNo;
		sims.m_nUPNUserTextNo = m_nUPNTextIndex;
		sims.m_nSIMSUserTextNo = m_nAdnoTextIndex;
		sims.m_nPIDUserTextNo = m_nPIDTextIndex;
		sims.m_nYearContactNo = m_nYearTextIndex;
		sims.m_nRegContactNo = m_nRegTextIndex;
		sims.m_nEmailContactNo = m_nEmailTextIndex;
		sims.m_bSaveEmail = (m_bSaveEmail != 0);
		sims.m_nDobNo = m_nDobTextIndex;
		sims.m_nNewIntakeAccount = m_nNewIntakeAccount;
		sims.m_nLocateBy = m_nLocateBy;
		sims.m_bAddUnknowns = (m_bAddUnknowns != 0);
		sims.m_nLocatePhotoBy = m_nLocatePhotoBy;
		sims.m_nImportMethod = m_nImportMethod;
		sims.m_strStartFromUserID = m_strStartFromUserID;

		if (nType == 0)
		{
			CCSVFileReport report("R211", m_strSimsImportFile, this);
			report.Show("SIMS Raw Import File");
		}
		else if (nType == 1)
		{
			CImportSimsFile simsFile(&sims, this);
			simsFile.ShowImports(m_strSimsImportFile);
		}
		else
		{
			CImportSimsPhotoFiles simsPhoto(&sims, this);
			simsPhoto.ShowImports();
		}
	}
}

//*******************************************************************

void CSetupSimsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (ValidateInput() == FALSE)
		{
			return;
		}
		m_sims.m_strImportFilename = m_strSimsImportFile;
		m_sims.m_strSimsPhotoFolder = m_strSimsPhotoFolder;
		m_sims.m_bDeleteImportFile = (m_bDeleteImportFile != 0);
		m_sims.m_bDeletePhotoFile = (m_bDeletePhotoFile != 0);
		m_sims.m_nFSMGroupNo = m_nFSMGroupNo;
		m_sims.m_nFSMRevertGroupNo = m_nFSMRevertGroupNo;
		m_sims.m_nUPNUserTextNo = m_nUPNTextIndex;
		m_sims.m_nSIMSUserTextNo = m_nAdnoTextIndex;
		m_sims.m_nPIDUserTextNo = m_nPIDTextIndex;
		m_sims.m_nYearContactNo = m_nYearTextIndex;
		m_sims.m_nRegContactNo = m_nRegTextIndex;
		m_sims.m_bSaveEmail = (m_bSaveEmail != 0);
		m_sims.m_nEmailContactNo = m_nEmailTextIndex;
		m_sims.m_nDobNo = m_nDobTextIndex;
		m_sims.m_nNewIntakeAccount = m_nNewIntakeAccount;
		m_sims.m_nLocateBy = m_nLocateBy;
		m_sims.m_bAddUnknowns = (m_bAddUnknowns != 0);
		m_sims.m_nLocatePhotoBy = m_nLocatePhotoBy;
		m_sims.m_nImportMethod = m_nImportMethod;
		m_sims.m_strStartFromUserID = m_strStartFromUserID;

		m_sims.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************************
