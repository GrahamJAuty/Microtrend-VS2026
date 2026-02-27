//***************************************************************
#include "GlobalFunctions.h"
#include "GTData.h"
#include "PasswordData.h"
#include "ServiceTimesDlg.h"
#include "SetupReaderDlg.h"
//***************************************************************
#include "SetupDlg.h"
//***************************************************************
extern CSysset Sysset;
//*******************************************************************
IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)
//*******************************************************************

CSetupDlg::CSetupDlg( CSystemData* pSystemData, CWnd* pParent)
	: CSSDialog(CSetupDlg::IDD, pParent)
	, m_pSystemData(pSystemData)
{
	CPasswordData password;
	m_strSetupPassword = password.GetPassword();
}

//*******************************************************************

CSetupDlg::~CSetupDlg()
{
}

//*******************************************************************

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PASSWORDREQD, m_checkEnablePassword);
	DDX_Control(pDX, IDC_CHECK_SHOWPOINTS, m_checkShowPoints);
	DDX_Control(pDX, IDC_CHECK_SHOWGROUP, m_checkShowGroup);
	DDX_Control(pDX, IDC_CHECK_SHOWPHOTOID, m_checkShowPhotoID);
	DDX_Control(pDX, IDC_CHECK_KEYPADENTRY, m_checkEnableKeypadEntry);
	DDX_Control(pDX, IDC_CHECK_HIDEACCOUNTID, m_checkHideAccountID);
	DDX_Control(pDX, IDC_CHECK_ENROLMENT, m_checkEnableEnrolment);
	DDX_Control(pDX, IDC_CHECK_SHOWAVAILABLESPEND, m_checkShowAvailableSpend);
	DDX_Control(pDX, IDC_CHECK_DISABLEALERTS, m_checkDisableAlerts);
	DDX_Control(pDX, IDC_CHECK_SHOWDATETIME, m_checkShowDateTime);
	DDX_Control(pDX, IDC_CHECK_NOSMALLCOINWARN, m_checkNoSmallCoinWarn);
	DDX_Control(pDX, IDC_CHECK_PURSE1TOPUP, m_checkTopupPurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE3TOPUP, m_checkTopupPurse3);
	DDX_Control(pDX, IDC_CHECK_SHOWUSERIDCANCEL, m_checkShowUserIDCancel);
	DDX_Control(pDX, IDC_COMBO_PINPAD, m_comboPINPad);
	/*****/
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strSetupPassword);
	DDX_Text(pDX, IDC_EDIT_TERMINALNO, m_nTerminalNo);
	DDV_MinMaxInt(pDX, m_nTerminalNo, 0, 99);
	DDX_Text(pDX, IDC_EDIT_CCNO, m_nCCNo);
	DDX_Text(pDX, IDC_EDIT_LOYREQFOLDER, m_strLoyReqFolder);
	DDX_Text(pDX, IDC_EDIT_LOYLOGFOLDER, m_strLoyLogFolder);
	DDX_Text(pDX, IDC_EDIT_NOTIFYPATH, m_strNotifyPath);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILE, m_strImportFile);
	DDX_Text(pDX, IDC_EDIT_CID, m_dCID);
	DDX_Text(pDX, IDC_EDIT_GT, m_dGT);
	DDX_Text(pDX, IDC_EDIT_REFRESHINTERVAL, m_nRefreshInterval);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nEnquiryTimeout);
	DDX_Text(pDX, IDC_EDIT_SERIALNO, m_lPaylinkSerialNo);
	DDX_CBIndex(pDX, IDC_COMBO_INTERFACE, m_nInterfaceType);
	DDX_Text(pDX, IDC_EDIT_ENROLPROMPT, m_strEnrolmentPrompt);
	DDX_Text(pDX, IDC_EDIT_CARDNOTEXT, m_strCardNoText);
	DDX_Text(pDX, IDC_EDIT_PURSE1TEXT, m_strPurse1Text);
	DDX_Text(pDX, IDC_EDIT_PURSE3TEXT, m_strPurse3Text);
	/*****/
	DDX_Control(pDX, IDC_EDIT_IMPORTFILE, m_editImportFile);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_CCNO, m_editCCNo);
	DDX_Control(pDX, IDC_EDIT_SERIALNO, m_editSerialNo);
	DDX_Control(pDX, IDC_EDIT_ENROLPROMPT, m_editEnrolPrompt);
	DDX_Control(pDX, IDC_EDIT_PURSE1TEXT, m_editPurse1Text);
	DDX_Control(pDX, IDC_EDIT_PURSE3TEXT, m_editPurse3Text);
	DDX_Control(pDX, IDC_EDIT_CARDNOTEXT, m_editCardNoText);
	DDX_Control(pDX, IDC_EDIT_REFRESHINTERVAL, m_editRefreshInterval);
	DDX_Control(pDX, IDC_EDIT_ENROLPROMPT, m_editEnrolPrompt);
	DDX_Control(pDX, IDC_EDIT_NOTIFYPATH, m_editNotifyPath);
	DDX_Control(pDX, IDC_EDIT_LOYREQFOLDER, m_editLoyREQFolder);
	DDX_Control(pDX, IDC_EDIT_LOYLOGFOLDER, m_editLoyLOGFolder);
	DDX_Control(pDX, IDC_EDIT_TERMINALNO, m_editTerminalNo);
	DDX_Control(pDX, IDC_BUTTON_TERMINAL, m_buttonTerminal);
	DDX_Control(pDX, IDC_BUTTON_CHECKBIOMETRIC, m_buttonTestBiometric);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_PASSWORDREQD, OnTogglePasswordReqd)
	ON_STN_CLICKED(IDC_STATIC_CCNO, OnClickStaticCCNo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE1, OnButtonBrowse1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, OnButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE3, OnButtonBrowse3)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE, OnButtonService)
	ON_BN_CLICKED(IDC_BUTTON_CHECKBIOMETRIC, OnButtonTestBiometric)
	ON_STN_CLICKED(IDC_STATIC_IMPORTFILE, OnClickStaticImportFile)
	ON_STN_CLICKED(IDC_STATIC_GT, OnClickStaticGT)
	ON_STN_CLICKED(IDC_STATIC_SERIALNO, OnClickStaticSerialNo)
	ON_BN_CLICKED(IDC_BUTTON_TERMINAL, OnButtonTerminal)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnSelectInterface)
	ON_BN_CLICKED(IDC_CHECK_ENROLMENT, OnToggleEnrolment)
	ON_BN_CLICKED(IDC_BUTTON_TEST1, OnButtonTest1)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, OnButtonTest2)
	ON_BN_CLICKED(IDC_BUTTON_TEST3, OnButtonTest3)
	ON_BN_CLICKED(IDC_CHECK_PURSE1TOPUP, OnTogglePurse1Topup)
	ON_BN_CLICKED(IDC_CHECK_PURSE3TOPUP, OnTogglePurse3Topup)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSetupDlg::OnInitDialog()
{
	m_nInterfaceType = m_pSystemData->m_nInterfaceType;
	m_nTerminalNo = m_pSystemData->m_nTerminalNo;
	m_lPaylinkSerialNo = m_pSystemData->GetPaylinkSerialNo();
	m_strLoyReqFolder = m_pSystemData->m_strLoyReqFolder;
	m_strLoyLogFolder = m_pSystemData->m_strLoyLogFolder;
	m_strNotifyPath = m_pSystemData->m_strNotifyPath;
	m_strImportFile = m_pSystemData->m_strImportFile;
	m_nRefreshInterval = m_pSystemData->m_nRefreshInterval;
	m_nEnquiryTimeout = m_pSystemData->m_nEnquiryTimeout;
	m_strEnrolmentPrompt = m_pSystemData->m_strEnrolmentPrompt;
	m_strCardNoText = m_pSystemData->m_strCardNoText;
	m_strPurse1Text = m_pSystemData->m_strPurse1Text;
	m_strPurse3Text = m_pSystemData->m_strPurse3Text;

	CGTData gt;
	m_nCCNo = m_nOldCCNo = gt.GetCCNo();
	m_dGT = m_dOldGT = (double)(gt.GetGT()) / 100;
	m_dCID = (double)(gt.GetCID()) / 100;

	CDialog::OnInitDialog();

	m_checkEnablePassword.SetCheck(m_pSystemData->GetSystemPasswordReqdFlag());
	m_checkShowPhotoID.SetCheck(m_pSystemData->GetShowPhotoIDFlag());
	m_checkShowPoints.SetCheck(m_pSystemData->GetShowPointsFlag());
	m_checkShowGroup.SetCheck(m_pSystemData->GetShowGroupFlag());
	m_checkShowAvailableSpend.SetCheck(m_pSystemData->GetShowAvailableSpendFlag());
	m_checkDisableAlerts.SetCheck(m_pSystemData->GetDisableAlertsFlag());
	m_checkHideAccountID.SetCheck(m_pSystemData->GetHideAccountIDFlag());
	m_checkEnableKeypadEntry.SetCheck(m_pSystemData->GetEnableKeypadEntryFlag());
	m_checkEnableEnrolment.SetCheck(m_pSystemData->GetEnrolmentAllowedFlag());
	m_checkShowDateTime.SetCheck(m_pSystemData->GetShowDateTimeFlag());
	m_checkNoSmallCoinWarn.SetCheck(m_pSystemData->GetNoSmallCoinWarningFlag());
	m_checkTopupPurse1.SetCheck(m_pSystemData->GetTopupPurse1Flag());
	m_checkTopupPurse3.SetCheck(m_pSystemData->GetTopupPurse3Flag());
	m_checkShowUserIDCancel.SetCheck(m_pSystemData->GetShowUserIDCancelFlag());

	SubclassEdit(IDC_EDIT_GT, SS_NUM_SDP, 11, "%.*f", System.m_nDPValue);
	SubclassEdit(IDC_EDIT_CID, SS_NUM_SDP, 11, "%.*f", System.m_nDPValue);

	m_editImportFile.EnableWindow(FALSE);

	m_comboPINPad.SetCurSel(m_pSystemData -> GetActualPINModeFlag() ? 1 : 0);

	SetDisplay();

	return TRUE;
}

//*******************************************************************

void CSetupDlg::OnTogglePasswordReqd()
{
	SetDisplay();
	m_editPassword.SetFocus();
}

//*******************************************************************

void CSetupDlg::OnClickStaticCCNo()
{
	m_editCCNo.EnableWindow(TRUE);
	m_editCCNo.SetFocus();
}

//*******************************************************************

void CSetupDlg::OnClickStaticGT()
{
	GetEditGT()->EnableWindow(TRUE);
	GetEditGT()->SetFocus();
}

//*******************************************************************

void CSetupDlg::OnClickStaticSerialNo()
{
	m_editSerialNo.EnableWindow(TRUE);
	m_editSerialNo.SetFocus();
}

//*******************************************************************

void CSetupDlg::OnButtonBrowse1()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the EPOS Request folder ( LOYREQ )", NULL, this) == TRUE)
	{
		m_strLoyReqFolder = strPathname;
		UpdateData(FALSE);
	}
}

//*******************************************************************

void CSetupDlg::OnButtonBrowse2()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the EPOS Transaction folder ( LOYLOG )", NULL, this) == TRUE)
	{
		m_strLoyLogFolder = strPathname;
		UpdateData(FALSE);
	}
}

//*******************************************************************

void CSetupDlg::OnButtonBrowse3()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the Biommetric Notify folder", NULL, this) == TRUE)
	{
		m_strNotifyPath = strPathname;
		UpdateData(FALSE);
	}
}

//*******************************************************************

void CSetupDlg::OnButtonService()
{
	CServiceTimesDlg dlg(this);
	dlg.DoModal();
}

//*******************************************************************

void CSetupDlg::OnButtonTestBiometric()
{
	if (UpdateData() == TRUE)
	{
		CWaitCursor wait;

		CString strMsg = "";

		if (m_pSystemData->RefreshBiometrics(m_strNotifyPath) == TRUE)
		{
			strMsg = "Biometric reader is responding to the current Notify path setting";
		}
		else
		{
			strMsg.Format("No response from the biometric reader using Notify path ' %s '",
				(const char*)m_strNotifyPath);
		}

		MessageBox(strMsg, "Verify Notify Path");
	}
}

//*******************************************************************

void CSetupDlg::OnClickStaticImportFile()
{
	m_editImportFile.EnableWindow(TRUE);
	m_editImportFile.SetFocus();
}

//*******************************************************************

void CSetupDlg::OnButtonTerminal()
{
	CSetupReaderDlg dlg(this);
	dlg.DoModal();
}

//*******************************************************************

void CSetupDlg::OnSelectInterface()
{
	SetDisplay();

	if (nINTERFACE_PINPAD == m_nInterfaceType)
	{
		m_checkEnableKeypadEntry.SetCheck(TRUE);
		m_checkEnablePassword.SetCheck(TRUE);
		m_editPassword.EnableWindow(TRUE);
		m_checkEnableKeypadEntry.EnableWindow(FALSE);
	}
	else
	{
		m_checkEnableKeypadEntry.EnableWindow(TRUE);
	}
}

//*******************************************************************

void CSetupDlg::OnToggleEnrolment()
{
	SetDisplay();
	m_editEnrolPrompt.SetFocus();
}

//*******************************************************************

void CSetupDlg::OnTogglePurse1Topup()
{
	SetDisplay();
	m_editPurse1Text.SetFocus();
}

//*******************************************************************

void CSetupDlg::OnTogglePurse3Topup()
{
	SetDisplay();
	m_editPurse3Text.SetFocus();
}

//*******************************************************************

void CSetupDlg::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		bool bAllowTerminal = TRUE;
		bool bAllowBiometric = FALSE;
		bool bAllowEnrol = FALSE;
		bool bCanDisableKeypad = TRUE;

		switch (m_nInterfaceType)
		{
		case nINTERFACE_BIOMETRIC:
			bAllowTerminal = FALSE;
			bAllowBiometric = TRUE;
			break;

		case nINTERFACE_PINPAD:
			bAllowTerminal = FALSE;
			bCanDisableKeypad = FALSE;
			break;

		case nINTERFACE_DUAL:
			bAllowBiometric = TRUE;
			break;

		case nINTERFACE_MIFAREv2:
			bAllowEnrol = TRUE;
			break;
		}

		m_buttonTerminal.EnableWindow(bAllowTerminal);
		m_editCardNoText.EnableWindow(bAllowTerminal);

		m_editRefreshInterval.EnableWindow(bAllowBiometric);
		m_buttonTestBiometric.EnableWindow(bAllowBiometric);

		m_checkEnableEnrolment.EnableWindow(bAllowEnrol);
		m_editEnrolPrompt.EnableWindow(bAllowEnrol && IsTicked(m_checkEnableEnrolment));

		m_editPassword.EnableWindow(IsTicked(m_checkEnablePassword));
		m_editPurse1Text.EnableWindow(IsTicked(m_checkTopupPurse1));
		m_editPurse3Text.EnableWindow(IsTicked(m_checkTopupPurse3));

		m_checkEnableKeypadEntry.EnableWindow(bCanDisableKeypad);
	}
}

//*****************************************************************

void CSetupDlg::OnButtonTest1()
{
	if (UpdateData() == TRUE)
	{
		if (TestPath("Notify", m_strNotifyPath) == FALSE)
		{
			m_editNotifyPath.SetFocus();
		}
	}
}

//*****************************************************************

void CSetupDlg::OnButtonTest2()
{
	if (UpdateData() == TRUE)
	{
		if (TestPath("LoyReq", m_strLoyReqFolder) == FALSE)
		{
			m_editLoyREQFolder.SetFocus();
		}
	}
}

//*****************************************************************

void CSetupDlg::OnButtonTest3()
{
	if (UpdateData() == TRUE)
	{
		if (TestPath("LoyLog", m_strLoyLogFolder) == FALSE)
		{
			m_editLoyLOGFolder.SetFocus();
		}
	}
}

//*****************************************************************

bool CSetupDlg::TestPath(const char* szTitle, const char* szPath)
{
	bool bReply = FALSE;
	CString strMsg = "";

	if ((bReply = ::ExistSubdirectory(szPath)) == FALSE)
	{
		strMsg.Format("Unable to access %s folder ' %s '!", szTitle, szPath);
	}
	else
	{
		if ((bReply = ::ValidateFolderAccess(szPath)) == FALSE)
		{
			strMsg.Format("Unable to write to %s folder ' %s '!", szTitle, szPath);
		}
		else
		{
			strMsg.Format("Path to %s folder is valid.", szTitle);
		}
	}

	MessageBox(strMsg, "Validation Check");
	return bReply;
}

//***************************************************************

void CSetupDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (m_strImportFile.IsEmpty())
		{
			MessageBox("No Biometric Import file name has been set!", "Validation Error", MB_ICONEXCLAMATION);
			m_editImportFile.SetFocus();
			return;
		}

		if (0 == m_nTerminalNo)
		{
			MessageBox("No valid Terminal number has been set!", "Validation Error", MB_ICONEXCLAMATION);
			m_editTerminalNo.SetFocus();
			return;
		}

		if (nINTERFACE_PINPAD == m_nInterfaceType)
		{
			if ((FALSE == IsTicked(m_checkEnablePassword)) || (m_strSetupPassword.IsEmpty()))
			{
				MessageBox("An Administration access PIN must be specified when using the Pin Pad interface!", "Validation Error", MB_ICONEXCLAMATION);
				m_editPassword.SetFocus();
				return;
			}
		}

		if (IsTicked(m_checkTopupPurse1) && IsTicked(m_checkTopupPurse3))
		{
			if (m_strPurse1Text == "")
			{
				MessageBox("Please specify Purse1 descriptive text", "Input Required", MB_ICONEXCLAMATION);
				m_editPurse1Text.SetFocus();
				return;
			}

			if (m_strPurse3Text == "")
			{
				MessageBox("Please specify Purse3 descriptive text", "Input Required", MB_ICONEXCLAMATION);
				m_editPurse3Text.SetFocus();
				return;
			}
		}

		m_pSystemData->m_nInterfaceType = m_nInterfaceType;

		m_pSystemData->SetSystemPasswordReqdFlag(IsTicked(m_checkEnablePassword));
		m_pSystemData->SetShowPhotoIDFlag(IsTicked(m_checkShowPhotoID));
		m_pSystemData->SetShowPointsFlag(IsTicked(m_checkShowPoints));
		m_pSystemData->SetShowGroupFlag(IsTicked(m_checkShowGroup));
		m_pSystemData->SetHideAccountIDFlag(IsTicked(m_checkHideAccountID));
		m_pSystemData->SetShowAvailableSpendFlag(IsTicked(m_checkShowAvailableSpend));
		m_pSystemData->SetDisableAlertsFlag(IsTicked(m_checkDisableAlerts));
		m_pSystemData->SetEnableKeypadEntryFlag(IsTicked(m_checkEnableKeypadEntry));
		m_pSystemData->SetEnrolmentAllowedFlag(IsTicked(m_checkEnableEnrolment));
		m_pSystemData->SetShowDateTimeFlag(IsTicked(m_checkShowDateTime));
		m_pSystemData->SetNoSmallCoinWarningFlag(IsTicked(m_checkNoSmallCoinWarn));
		m_pSystemData->SetTopupPurse1Flag(IsTicked(m_checkTopupPurse1));
		m_pSystemData->SetTopupPurse3Flag(IsTicked(m_checkTopupPurse3));
		m_pSystemData->SetShowUserIDCancelFlag(IsTicked(m_checkShowUserIDCancel));
		m_pSystemData->SetActualPINModeFlag(m_comboPINPad.GetCurSel() == 1);

		m_pSystemData->m_nTerminalNo = m_nTerminalNo;
		m_pSystemData->SetPaylinkSerialNo(m_lPaylinkSerialNo);
		m_pSystemData->m_strLoyReqFolder = m_strLoyReqFolder;
		m_pSystemData->m_strLoyLogFolder = m_strLoyLogFolder;
		m_pSystemData->m_strNotifyPath = m_strNotifyPath;
		m_pSystemData->m_strImportFile = m_strImportFile;
		m_pSystemData->m_nRefreshInterval = m_nRefreshInterval;
		m_pSystemData->m_nEnquiryTimeout = m_nEnquiryTimeout;
		m_pSystemData->m_strEnrolmentPrompt = m_strEnrolmentPrompt;
		m_pSystemData->m_strCardNoText = m_strCardNoText;
		m_pSystemData->m_strPurse1Text = m_strPurse1Text;
		m_pSystemData->m_strPurse3Text = m_strPurse3Text;

		CGTData gt;
		if (m_nCCNo != m_nOldCCNo)
		{
			gt.SetCCNo(m_nCCNo);
		}

		if (m_dGT != m_dOldGT)
		{
			gt.SetGT(m_dGT);
		}

		if (TRUE == IsTicked(m_checkEnablePassword))
		{
			CPasswordData password;
			password.SetPassword(m_strSetupPassword);
		}

		EndDialog(IDOK);
	}
}

//*******************************************************************
