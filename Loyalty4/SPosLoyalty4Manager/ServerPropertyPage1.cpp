//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "SetupEposPathsDlg.h"
//$$******************************************************************
#include "ServerPropertyPage1.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CServerPropertyPage1::CServerPropertyPage1()
	: CSSPropertyPage(CServerPropertyPage1::IDD)
{
}

//$$******************************************************************

CServerPropertyPage1::~CServerPropertyPage1()
{
}

//$$******************************************************************

void CServerPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LOGACTION, m_checkLogAction);
	DDX_Control(pDX, IDC_EDIT_LOYREQ_PATH, m_editLoyReqPath);
	DDX_Control(pDX, IDC_CHECK_OVERSPEND, m_checkAllowOverSpend);
	DDX_Control(pDX, IDC_CHECK_REALTIMEPLUS, m_checkRealTimePlus);
	/*****/
	DDX_Control(pDX, IDC_EDIT_LOYLOG_PATH, m_editLoyLogPath);
	DDX_Control(pDX, IDC_CHECK_LOYLOG_NEWBALANCE, m_checkLoyLogNewBalance);
	DDX_Control(pDX, IDC_CHECK_LOGSAVE, m_checkLogSave);
	DDX_Control(pDX, IDC_CHECK_LOGSAVE2, m_checkLogSave2);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage1, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_LOYREQ_FOLDERS, OnButtonFolders)
	ON_BN_CLICKED(IDC_BUTTON_LOYREQ_BROWSE, OnButtonLoyReqBrowse)
	ON_BN_CLICKED(IDC_BUTTON_LOYREQ_TEST, OnButtonLoyReqTest)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_LOYLOG_FOLDERS, OnButtonFolders)
	ON_BN_CLICKED(IDC_BUTTON_LOYLOG_BROWSE, OnButtonLoyLogBrowse)
	ON_BN_CLICKED(IDC_BUTTON_LOYLOG_TEST, OnButtonLoyLogTest)
	ON_BN_CLICKED(IDC_CHECK_LOGSAVE, OnToggleLogSave)
	ON_STN_CLICKED(IDC_STATIC_LOGSAVE2, OnClickStaticLogSave2)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	/*****/

	CServerDataFolderSetIndex FolderSetIndex(1, 1);

	m_checkLogAction.SetCheck(Server.GetLogActionFlag());
	m_editLoyReqPath.SetWindowText(Server.GetEposRequestPath(FolderSetIndex));
	m_checkAllowOverSpend.SetCheck(Server.GetAllowOverSpendFlag());
	m_checkRealTimePlus.SetCheck(Server.GetRealTimePlusFlag());

	switch (Server.GetLoyaltyLogSaveType())
	{
	case 1:
		m_checkLogSave.SetCheck(TRUE);
		m_checkLogSave2.EnableWindow(TRUE);
		m_checkLogSave2.SetCheck(FALSE);
		break;

	case 2:
		m_checkLogSave.SetCheck(TRUE);
		m_checkLogSave2.EnableWindow(TRUE);
		m_checkLogSave2.SetCheck(TRUE);
		break;

	default:
		m_checkLogSave.SetCheck(FALSE);
		m_checkLogSave2.EnableWindow(FALSE);
		m_checkLogSave2.SetCheck(FALSE);
		break;
	}

	/*****/

	m_checkLoyLogNewBalance.SetCheck(Server.GetNewBalanceOnReceiptFlag());
	m_editLoyLogPath.SetWindowText(Server.GetEposLogPath(FolderSetIndex));

	return TRUE;
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonFolders()
{
	CServerDataFolderSetIndex FolderSetIndex(1, 1);
	Server.SetEposRequestPath(FolderSetIndex, GetEditBoxText(m_editLoyReqPath));
	Server.SetEposLogPath(FolderSetIndex, GetEditBoxText(m_editLoyLogPath));

	CSetupEposPathsDlg dlg(this);

	if (dlg.DoModal() == IDOK)
	{
		CServerDataFolderSetIndex FolderSetIndex(1, 1);
		m_editLoyReqPath.SetWindowText(Server.GetEposRequestPath(FolderSetIndex));
		m_editLoyLogPath.SetWindowText(Server.GetEposLogPath(FolderSetIndex));
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonLoyReqBrowse()
{
	CString strPathname;
	if ( BrowseFolder ( strPathname, "Locate the SPos Loyalty Request folder ( LOYREQ )",  NULL, this ) == TRUE )
	{
		m_editLoyReqPath.SetWindowText( strPathname );
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonLoyReqTest()
{
	CString strEposRequestPath = GetEditBoxText( m_editLoyReqPath );

	CString strMsg;

	if (::ExistSubdirectory(strEposRequestPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyReq folder ' %s '!", 
			(const char*) strEposRequestPath);
	}
	else
	{
		if (::ValidateFolderAccess(strEposRequestPath) == FALSE)
		{
			strMsg = "Unable to write to Epos LoyReq folder!";
		}
		else
		{
			CString strMask = strEposRequestPath + "\\*.REQ";
			int nFileCount = ::CountFiles ( strMask );

			strMsg.Format ( "Path to Epos LoyReq folder is valid.\n\nNumber of outstanding requests = %d.", nFileCount );
		}
	}

	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CServerPropertyPage1::OnButtonLoyLogBrowse()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the SPos Loyalty Transaction folder ( LOYLOG )", NULL, this) == TRUE)
	{
		m_editLoyLogPath.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonLoyLogTest()
{
	CString strPath = GetEditBoxText( m_editLoyLogPath );

	CString strMsg = "";

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		strMsg.Format("Unable to access LoyLog folder ' %s '!", 
			(const char*) strPath);
	}
	else
	{
		if (::ValidateFolderAccess(strPath) == FALSE)
		{
			strMsg = "Unable to write to Epos LoyLog folder!";
		}
		else
		{
			CString strMask = strPath + "\\*.LOG";
			int nFileCount = ::CountFiles ( strMask );

			strMask = strPath + "\\*.$$1";
			nFileCount += ::CountFiles ( strMask );

			strMsg.Format ( "Path to Epos LoyLog folder is valid.\n\nNumber of outstanding log files = %d.", nFileCount );
		}
	}

	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************

void CServerPropertyPage1::OnToggleLogSave()
{
	if (IsTicked(m_checkLogSave))
	{
		m_checkLogSave2.EnableWindow(TRUE);
	}
	else
	{
		m_checkLogSave2.SetCheck(FALSE);
		m_checkLogSave2.EnableWindow(FALSE);
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnClickStaticLogSave2()
{
	if (m_checkLogSave2.IsWindowEnabled())
	{
		m_checkLogSave2.SetCheck(IsTicked(m_checkLogSave2) == FALSE);
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

BOOL CServerPropertyPage1::OnKillActive()
{
	CServerDataFolderSetIndex FolderSetIndex(1, 1);

	Server.SetLogActionFlag(IsTicked(m_checkLogAction));
	Server.SetEposRequestPath(FolderSetIndex, GetEditBoxText(m_editLoyReqPath));
	Server.SetRealTimePlusFlag(IsTicked(m_checkRealTimePlus));
	Server.SetAllowOverSpendFlag(IsTicked(m_checkAllowOverSpend));

	/*****/

	Server.SetEposLogPath(FolderSetIndex, GetEditBoxText(m_editLoyLogPath));
	Server.SetNewBalanceOnReceiptFlag(IsTicked(m_checkLoyLogNewBalance));

	if (IsTicked(m_checkLogSave) == FALSE)
	{
		Server.SetLoyaltyLogSaveType(0);
	}
	else if (IsTicked(m_checkLogSave2) == FALSE)
	{
		Server.SetLoyaltyLogSaveType(1);
	}
	else
	{
		Server.SetLoyaltyLogSaveType(2);
	}

	/*****/

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
