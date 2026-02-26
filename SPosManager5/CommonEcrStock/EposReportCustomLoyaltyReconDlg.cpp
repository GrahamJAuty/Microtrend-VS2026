/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "GlobalFunctions.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCustomLoyaltyReconDlg.h"
/**********************************************************************/

CEposReportCustomLoyaltyReconDlg::CEposReportCustomLoyaltyReconDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CSSDialog(CEposReportCustomLoyaltyReconDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomLoyaltyReconDlg::~CEposReportCustomLoyaltyReconDlg()
{
}

/**********************************************************************/

void CEposReportCustomLoyaltyReconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_STATIC_FOLDER, m_staticFolder);
	DDX_Control(pDX, IDC_STATIC_SELECT, m_staticSelect);
	DDX_Control(pDX, IDC_EDIT_REQFOLDER, m_editREQFolder);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_buttonTest);
	DDX_Control(pDX, IDC_STATIC_OPTIONS, m_staticOptions);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDC_CHECK_UNRECON, m_checkUnrecon);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomLoyaltyReconDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
	ON_BN_CLICKED( IDC_BUTTON_BROWSE,OnButtonBrowse)
	ON_BN_CLICKED( IDC_BUTTON_TEST, OnButtonTest)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomLoyaltyReconDlg::OnInitDialog()
{
	CEposReportCustomSettingsLoyaltyRecon Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine(strParams);

	CDialog::OnInitDialog();

	m_editName.LimitText(EPOS_CUSTOM_MAXLEN_NAME);
	m_editName.SetWindowText(m_infoReport.GetCustomReportName());

	m_editREQFolder.LimitText(MAX_PATH);
	m_editREQFolder.SetWindowText(Settings.GetLoyaltyRequestPath());

	m_checkAdhoc.SetCheck(Settings.GetAllowAdhocFlag());
	m_checkUnrecon.SetCheck(Settings.GetUnreconOnlyFlag());

	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenu);
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenuTexts);

	CString strType = "";

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V2:
	case LOYALTY_TYPE_SMP_V4:
		strType = "SmartPay";
		break;

	default:
		strType = "Loyalty";
		break;
	}
	strType += " Reconcilation";

	if (TRUE == m_bAdhoc)
	{
		m_editName.EnableWindow(FALSE);

		ShowAndEnableWindow(&m_staticFolder, FALSE);
		ShowAndEnableWindow(&m_staticSelect, FALSE);
		ShowAndEnableWindow(&m_editREQFolder, FALSE);
		ShowAndEnableWindow(&m_buttonBrowse, FALSE);
		ShowAndEnableWindow(&m_buttonTest, FALSE);
		ShowAndEnableWindow(&m_checkAdhoc, FALSE);

		MoveControl(&m_staticOptions, 7, 55);
		MoveControl(&m_checkUnrecon, 28, 79);
		MoveControl(&m_buttonSave, 26, 100);
		MoveControl(&m_buttonOK, 282, 137);
		MoveControl(&m_buttonCancel, 338, 137);
		ResizeDialog(395,159);

		CString strTitle;
		strTitle.Format("Adhoc %s Report Settings",
			(const char*)strType);

		SetWindowText(strTitle);
	}
	else
	{
		CString strTitle;
		strTitle.Format("Setup Custom %s Report",
			(const char*)strType);

		SetWindowText(strTitle);
	}

	if ((FALSE == m_bAdhoc) || (FALSE == bEnableSave))
	{
		ShowAndEnableWindow(&m_buttonSave, FALSE);
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomLoyaltyReconDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsLoyaltyRecon Settings;
	Settings.SetAllowAdhocFlag(IsTicked(m_checkAdhoc));
	Settings.SetUnreconOnlyFlag(IsTicked(m_checkUnrecon));
	Settings.SetLoyaltyRequestPath(GetEditBoxText(m_editREQFolder));
	
	infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_LOYALTYRECON );
	infoReport.SetCustomReportName( GetEditBoxText(m_editName));

	CString strLine;
	Settings.GetSettingsCSVLine( strLine );
	infoReport.SetCustomReportParams( strLine );
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCustomLoyaltyReconDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomLoyaltyReconDlg::OnButtonBrowse()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the SPos Loyalty Request folder ( LOYREQ )", NULL, this) == TRUE)
	{
		m_editREQFolder.SetWindowText(strPathname);
	}
}

/**********************************************************************/

void CEposReportCustomLoyaltyReconDlg::OnButtonTest()
{
	CString strEposRequestPath = GetEditBoxText(m_editREQFolder);

	CString strMsg;

	if (::ExistSubdirectory(strEposRequestPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyReq folder ' %s '!", 
			(const char*) strEposRequestPath);
	}
	else
	{
		if (ValidateFolderAccess(strEposRequestPath) == FALSE)
		{
			strMsg = "Unable to write to Epos LoyReq folder!";
		}
		else
		{
			CString strMask = strEposRequestPath + "\\*.REQ";
			int nFileCount = CountFiles(strMask);

			strMsg.Format("Path to Epos LoyReq folder is valid.\n\nNumber of outstanding requests = %d.", nFileCount);
		}
	}

	Prompter.Info(strMsg);
}

/**********************************************************************/

void CEposReportCustomLoyaltyReconDlg::OnOK()
{
	if (GetUpdatedSettings(m_infoReport) == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/
