//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
//$$******************************************************************
#include "DefEditDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "SetupEposPathsDlg.h"
//$$******************************************************************

CSetupEposPathsDlg::CSetupEposPathsDlg(CWnd* pParent )
	: CSSDialog(CSetupEposPathsDlg::IDD, pParent)
{
	CServerDataFolderSetIndex FolderSetIndex;

	for (int nDbNo = 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			FolderSetIndex.m_nSetNo = nSetNo;

			m_strReqPath[nDbNo][nSetNo] = Server.GetEposRequestPath(FolderSetIndex);
			m_strLogPath[nDbNo][nSetNo] = Server.GetEposLogPath(FolderSetIndex);
			m_bExtPath[nDbNo][nSetNo] = Server.GetExternalRequestPathFlag(FolderSetIndex);
		}
	}

	m_nActiveTab = 0;
	m_nCurrentDbNo = 1;
}

//$$******************************************************************

void CSetupEposPathsDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_TAB_FOLDERS, m_tabFolders);
	DDX_Control(pDX, IDC_EDIT_LOYREQPATH, m_editLoyReqPath);
	DDX_Control(pDX, IDC_EDIT_LOYLOGPATH, m_editLoyLogPath);
	DDX_Control(pDX, IDC_CHECK_EXTERNAL, m_checkExternal);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CSetupEposPathsDlg, CSSDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FOLDERS, OnTcnSelchangeTabs)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_REQ, OnButtonBrowseReq)
	ON_BN_CLICKED(IDC_BUTTON_TEST_REQ, OnButtonTestReq)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOG, OnButtonBrowseLog)
	ON_BN_CLICKED(IDC_BUTTON_TEST_LOG, OnButtonTestLog)
	ON_BN_CLICKED(IDC_BUTTON_LABEL, OnButtonLabel)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CSetupEposPathsDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();

	CStandardCombos::FillDatabaseCombo(m_comboDatabase, FALSE, 1);

	m_nCurrentDbNo = 1;

	CServerDataFolderSetIndex FolderSetIndex;
	FolderSetIndex.m_nDbNo = 1;

	for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		FolderSetIndex.m_nSetNo = nSetNo;

		CString strLabel = Server.GetEposPathLabel(FolderSetIndex);
		TrimSpacesFromString(strLabel);

		if (strLabel == "")
		{
			strLabel = Server.GetDefaultEposPathLabel(nSetNo);
		}

		m_tabFolders.InsertItem(nSetNo - 1, strLabel);
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

void CSetupEposPathsDlg::OnSelectDatabase()
{
	GetEditPathTexts();

	int nDbNo = m_comboDatabase.GetCurSel() + 1;
	if ((nDbNo < 1) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = 1;
		m_comboDatabase.SetCurSel(0);
	}

	m_nCurrentDbNo = nDbNo;
	SetEditPathTexts();

	CServerDataFolderSetIndex FolderSetIndex;
	FolderSetIndex.m_nDbNo = nDbNo;

	for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		FolderSetIndex.m_nSetNo = nSetNo;

		CString strLabel = Server.GetEposPathLabel(FolderSetIndex);

		TrimSpacesFromString(strLabel);
		if (strLabel == "")
		{
			strLabel = Server.GetDefaultEposPathLabel(nSetNo);
		}

		TCITEM itemState;
		itemState.mask = TCIF_TEXT;
		itemState.pszText = strLabel.LockBuffer();
		m_tabFolders.SetItem(nSetNo - 1, &itemState);
		strLabel.UnlockBuffer();
	}
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
	if ((m_nActiveTab >= 0) && (m_nActiveTab < EPOSREQPATH_COUNT) && (m_nCurrentDbNo >= 1) && (m_nCurrentDbNo <= PLU_DBNO_COUNT))
	{
		m_editLoyReqPath.SetWindowText(m_strReqPath[m_nCurrentDbNo][m_nActiveTab + 1]);
		m_editLoyLogPath.SetWindowText(m_strLogPath[m_nCurrentDbNo][m_nActiveTab + 1]);
		m_checkExternal.SetCheck(m_bExtPath[m_nCurrentDbNo][m_nActiveTab + 1]);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::GetEditPathTexts()
{
	if ((m_nActiveTab >= 0) && (m_nActiveTab < EPOSREQPATH_COUNT) && (m_nCurrentDbNo >= 1) && (m_nCurrentDbNo <= PLU_DBNO_COUNT))
	{
		m_editLoyReqPath.GetWindowText(m_strReqPath[m_nCurrentDbNo][m_nActiveTab + 1]);
		m_editLoyLogPath.GetWindowText(m_strLogPath[m_nCurrentDbNo][m_nActiveTab + 1]);
		m_bExtPath[m_nCurrentDbNo][m_nActiveTab + 1] = IsTicked(m_checkExternal);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonBrowseReq()
{
	CString strPathname;
	if (BrowseFolder(strPathname, "Locate the SPos Loyalty Request folder ( LOYREQ )", NULL, this) == TRUE)
	{
		m_editLoyReqPath.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonTestReq()
{
	CString strReqPath;
	m_editLoyReqPath.GetWindowText( strReqPath );
		
	CString strMsg;

	if (::ExistSubdirectory(strReqPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyReq folder ' %s '!", 
			(const char*) strReqPath);
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
			int nFileCount = ::CountFiles ( strMask );
			strMsg.Format ( "Path to Epos LoyReq folder is valid.\n\nNumber of outstanding requests = %d.", 
				nFileCount );
		}
	}

	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonBrowseLog()
{
	CString strPathname;
	if (BrowseFolder(strPathname, "Locate the SPos Loyalty Transaction folder ( LOYLOG )", NULL, this) == TRUE)
	{
		m_editLoyLogPath.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonTestLog()
{
	CString strLogPath;
	m_editLoyLogPath.GetWindowText( strLogPath );
	
	CString strMsg;

	if (::ExistSubdirectory(strLogPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyLog folder ' %s '!",
			(const char*) strLogPath);
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
			int nFileCount = ::CountFiles ( strMask );

			strMask = strLogPath + "\\*.$$1";
			nFileCount += ::CountFiles ( strMask );

			strMsg.Format ( "Path to Epos LoyLog folder is valid.\n\nNumber of outstanding log files = %d.", nFileCount );
		}
	}

	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************

void CSetupEposPathsDlg::OnButtonLabel()
{
	CServerDataFolderSetIndex FolderSetIndex(m_comboDatabase.GetCurSel() + 1, m_nActiveTab + 1);

	CString strTitle = "";
	strTitle.Format("Change Folder Set Label (Db %d, Set %d)", FolderSetIndex.m_nDbNo, FolderSetIndex.m_nSetNo);

	CDefEditDlg dlg(strTitle, Server.GetEposPathLabel(FolderSetIndex), 20, "Label", this);

	if (dlg.DoModal() == IDOK)
	{
		CString strLabel = dlg.m_strName;

		TrimSpacesFromString(strLabel);

		if (strLabel == "")
		{
			strLabel = Server.GetDefaultEposPathLabel(FolderSetIndex.m_nSetNo);
		}

		Server.SetEposPathLabel(FolderSetIndex, strLabel);
		
		TCITEM itemState;
		itemState.mask = TCIF_TEXT;
		itemState.pszText = strLabel.LockBuffer();
		m_tabFolders.SetItem(FolderSetIndex.m_nSetNo - 1, &itemState);
		strLabel.UnlockBuffer();
	}
}

//$$******************************************************************

void CSetupEposPathsDlg::OnOK()
{
	GetEditPathTexts();

	CServerDataFolderSetIndex FolderSetIndex;

	for (int nDbNo = 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			FolderSetIndex.m_nSetNo = nSetNo;

			Server.SetEposRequestPath(FolderSetIndex, m_strReqPath[nDbNo][nSetNo]);
			Server.SetEposLogPath(FolderSetIndex, m_strLogPath[nDbNo][nSetNo]);
			Server.SetExternalRequestPathFlag(FolderSetIndex, m_bExtPath[nDbNo][nSetNo]);
		}
	}

	EndDialog(IDOK);
}

//$$******************************************************************
