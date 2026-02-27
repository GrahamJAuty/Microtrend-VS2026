//$$******************************************************************
#include "DefEditDlg.h"
//$$******************************************************************
#include "SetupEposPathsDlg.h"
//$$******************************************************************

CSetupEposPathsDlg::CSetupEposPathsDlg(CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupEposPathsDlg::IDD, pParent)
{
	for ( int n = 1; n <= EPOSREQPATH_COUNT; n++ )
	{
		m_strReqPath[n] = Server.GetEposRequestPath(n);
		m_strLogPath[n] = Server.GetEposLogPath(n);
		m_bExtPath[n] = Server.GetExternalRequestPathFlag(n);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_FOLDERS, m_tabFolders);
	DDX_Control(pDX, IDC_EDIT_LOYREQPATH, m_editLoyReqPath);
	DDX_Control(pDX, IDC_EDIT_LOYLOGPATH, m_editLoyLogPath);
	DDX_Control(pDX, IDC_CHECK_EXTERNAL, m_checkExternal);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CSetupEposPathsDlg, CSSAutoShutdownDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FOLDERS, OnTcnSelchangeTabs)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_REQ, OnButtonBrowseReq)
	ON_BN_CLICKED(IDC_BUTTON_TEST_REQ, OnButtonTestReq)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOG, OnButtonBrowseLog)
	ON_BN_CLICKED(IDC_BUTTON_TEST_LOG, OnButtonTestLog)
	ON_BN_CLICKED(IDC_BUTTON_LABEL, OnButtonLabel)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CSetupEposPathsDlg::CSSAutoShutdownPostInitDialog()
{
	for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
	{
		CString strLabel = Server.GetEposPathLabel(n);
		TrimSpacesFromString(strLabel);

		if (strLabel == "")
		{
			strLabel = Server.GetDefaultEposPathLabel(n);
		}

		m_tabFolders.InsertItem(n - 1, strLabel);
	}

	m_tabFolders.SetCurSel(0);

	m_nActiveTab = 0;
	SetEditPathTexts();

	if (System.GetExternalAccountFlag() == FALSE)
	{
		ShowAndEnableWindow(&m_checkExternal, FALSE);
	}

	return TRUE;
}

//$$******************************************************************

void CSetupEposPathsDlg::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	GetEditPathTexts();

	m_nActiveTab = m_tabFolders.GetCurSel();
	if ((m_nActiveTab < 0) || (m_nActiveTab >= EPOSREQPATH_COUNT))
	{
		m_nActiveTab = 0;
	}

	SetEditPathTexts();
}

//$$******************************************************************

void CSetupEposPathsDlg::SetEditPathTexts()
{
	if ((m_nActiveTab >= 0) && (m_nActiveTab < EPOSREQPATH_COUNT))
	{
		m_editLoyReqPath.SetWindowText(m_strReqPath[m_nActiveTab + 1]);
		m_editLoyLogPath.SetWindowText(m_strLogPath[m_nActiveTab + 1]);
		m_checkExternal.SetCheck(m_bExtPath[m_nActiveTab + 1]);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::GetEditPathTexts()
{
	if ((m_nActiveTab >= 0) && (m_nActiveTab < EPOSREQPATH_COUNT))
	{
		m_editLoyReqPath.GetWindowText(m_strReqPath[m_nActiveTab + 1]);
		m_editLoyLogPath.GetWindowText(m_strLogPath[m_nActiveTab + 1]);
		m_bExtPath[m_nActiveTab + 1] = IsTicked(m_checkExternal);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonBrowseReq()
{
	CString strPathname;
	if (BrowseFolder(strPathname, "Locate the SPos SmartPay Request folder ( LOYREQ )", NULL, this) == TRUE)
	{
		m_editLoyReqPath.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonTestReq()
{
	CString strReqPath;
	m_editLoyReqPath.GetWindowText(strReqPath);

	CString strMsg = "";

	if (::ExistSubdirectory(strReqPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyReq folder ' %s '!", strReqPath);
	}
	else
	{
		if (::ValidateFolderAccess(strReqPath) == FALSE)
		{
			strMsg = "Unable to write to Epos LoyReq folder!";
		}
		else
		{
			CString strMask = strReqPath + "\\*.REQ";
			int nFileCount = ::CountFiles(strMask);
			strMsg.Format("Path to Epos LoyReq folder is valid.\n\nNumber of outstanding requests = %d.", nFileCount);
		}
	}

	Prompter.Info(strMsg, "Validation Check");
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonBrowseLog()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the SPos SmartPay Transaction folder ( LOYLOG )", NULL, this) == TRUE)
	{
		m_editLoyLogPath.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonTestLog()
{
	CString strLogPath = "";
	m_editLoyLogPath.GetWindowText(strLogPath);

	CString strMsg = "";

	if (::ExistSubdirectory(strLogPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyLog folder ' %s '!", strLogPath);
	}
	else
	{
		if (::ValidateFolderAccess(strLogPath) == FALSE)
		{
			strMsg = "Unable to write to Epos LoyLog folder!";
		}
		else
		{
			CString strMask = strLogPath + "\\*.LOG";
			int nFileCount = ::CountFiles(strMask);

			strMask = strLogPath + "\\*.$$1";
			nFileCount += ::CountFiles(strMask);

			strMsg.Format("Path to Epos LoyLog folder is valid.\n\nNumber of outstanding log files = %d.", nFileCount);
		}
	}

	Prompter.Info(strMsg, "Validation Check");
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonLabel()
{
	int nSet = m_nActiveTab + 1;

	CString strTitle = "";
	strTitle.Format("Change Folder Set Label (Set %d)", nSet);

	CDefEditDlg dlg(strTitle, Server.GetEposPathLabel(nSet), 20, "Label", this);

	if (dlg.DoModal() == IDOK)
	{
		CString strLabel = dlg.m_strName;

		TrimSpacesFromString(strLabel);

		if (strLabel == "")
		{
			strLabel = Server.GetDefaultEposPathLabel(nSet);
		}

		Server.SetEposPathLabel(nSet, strLabel);

		TCITEM itemState;
		itemState.mask = TCIF_TEXT;
		itemState.pszText = strLabel.LockBuffer();
		m_tabFolders.SetItem(nSet - 1, &itemState);
		strLabel.UnlockBuffer();
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnOK()
{
	GetEditPathTexts();

	for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
	{
		Server.SetEposRequestPath(n, m_strReqPath[n]);
		Server.SetEposLogPath(n, m_strLogPath[n]);
		Server.SetExternalRequestPathFlag(n, m_bExtPath[n]);
	}

	EndDialog(IDOK);
}

//$$******************************************************************
