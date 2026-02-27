//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "GroupComboHelpers.h"
#include "ServerPropertySheet.h"
#include "SetupEposPathsDlg.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ServerPropertyPage1.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CServerPropertyPage1::CServerPropertyPage1()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CServerPropertyPage1::IDD)
{
	m_bHadServer = FALSE;
}

//$$******************************************************************

CServerPropertyPage1::~CServerPropertyPage1()
{
}

//$$******************************************************************

void CServerPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LOGACTION2, m_checkLogAction2);
	/*****/
	DDX_Control(pDX, IDC_EDIT_LOYREQPATH, m_editEposRequestPath);
	DDX_Control(pDX, IDC_CHECK_OVERDRAFT, m_checkAllowOverdraft);
	DDX_Control(pDX, IDC_EDIT_OFFLINESPENDID, m_editOfflineSpendID);
	DDX_Control(pDX, IDC_CHECK_ADDUNKNOWNS, m_checkAddUnknowns);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_COMBO_FORENAME, m_comboForename);
	/*****/
	DDX_Control(pDX, IDC_EDIT_LOYLOGPATH, m_editEposLogPath);
	DDX_Control(pDX, IDC_CHECK_NEWBALANCE, m_checkNewBalance);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage1, CSSAutoShutdownPropertyPage)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_LOYREQ_FOLDERS, OnButtonFolders)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOYREQ, OnButtonBrowseLoyReq)
	ON_BN_CLICKED(IDC_BUTTON_TEST_LOYREQ, OnButtonTestLoyReq)
	ON_BN_CLICKED(IDC_CHECK_ADDUNKNOWNS, OnToggleAddUnknowns)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_LOYLOG_FOLDERS, OnButtonFolders)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOYLOG, OnButtonBrowseLoyLog)
	ON_BN_CLICKED(IDC_BUTTON_TEST_LOYLOG, OnButtonTestLoyLog)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	m_checkLogAction2.SetCheck( Server.GetLogAction2Flag() );

	/*****/

	SubclassEdit ( IDC_EDIT_OFFLINELIMIT, SS_NUM_SDP, 11, "%.2f" ); 

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(70);
	m_comboForename.SetTabStops(arrayStops);

	{
		CSQLRepositoryUserText RepoText;

		CString strInfo3 = "";
		strInfo3.Format("Info3,(%s)", 
			(const char*) RepoText.GetInfo3Text());

		CString strInfo4 = "";
		strInfo4.Format("Info4,(%s)", 
			(const char*) RepoText.GetInfo4Text());

		m_comboForename.AddItem("Forename");
		m_comboForename.AddItem(strInfo3);
		m_comboForename.AddItem(strInfo4);

		switch (Server.GetForenameSearchFieldType())
		{
		case 2:
			m_comboForename.SetCurSel(1);
			break;

		case 3:
			m_comboForename.SetCurSel(2);
			break;

		case 0:
		case 1:
		default:
			m_comboForename.SetCurSel(0);
			break;
		}
	}

	m_editEposRequestPath.SetWindowText( Server.GetEposRequestPath(1) );
	m_checkAllowOverdraft.SetCheck( Server.GetAllowOverdraftFlag() );
	m_checkAddUnknowns.SetCheck( Server.GetAddUnknownsFlag() );
	m_editOfflineSpendID.SetWindowText(Server.GetOfflineSpendID());

	CString strLimit;
	strLimit.Format( "%.2f", Server.GetOfflineSpendLimit() );
	GetEditOfflineSpendLimit() -> SetWindowText( strLimit );

	OnToggleAddUnknowns();

	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, Server.GetUnknownGroup(), FALSE, 70);
	
	if ( m_tabcomboGroup.GetCount() > 1 )	
	{
		m_tabcomboGroup.EnableWindow(m_checkAddUnknowns.GetCheck() != 0);
	}

	/*****/

	m_editEposLogPath.SetWindowText( Server.GetEposLogPath(1));
	m_checkNewBalance.SetCheck( Server.GetNewBalanceOnReceiptFlag() );

	return TRUE;
}

//$$******************************************************************
CEdit* CServerPropertyPage1::GetEditOfflineSpendLimit(){ return GetEdit( IDC_EDIT_OFFLINELIMIT ); }
//$$******************************************************************

void CServerPropertyPage1::OnToggleAddUnknowns()
{
	m_tabcomboGroup.EnableWindow ( IsTicked(m_checkAddUnknowns) && ( m_tabcomboGroup.GetCount() > 1 ) );
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonFolders()
{
	Server.SetEposRequestPath(1, GetEditBoxText(m_editEposRequestPath));
	Server.SetEposLogPath(1, GetEditBoxText(m_editEposLogPath));

	CSetupEposPathsDlg dlg(this);

	if (dlg.DoModal() == IDOK)
	{
		m_editEposRequestPath.SetWindowText(Server.GetEposRequestPath(1));
		m_editEposLogPath.SetWindowText(Server.GetEposLogPath(1));
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonBrowseLoyReq()
{
	CString strPathname;
	if ( DoBrowseFolder ( strPathname, "Locate the LOYREQ Request folder",  NULL, this ) == TRUE )
	{
		m_editEposRequestPath.SetWindowText( strPathname );
		UpdateData(FALSE);
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonTestLoyReq()
{
	CString strMsg;
	CString strReqPath = GetEditBoxText( m_editEposRequestPath );

	if ( ::ExistSubdirectory ( strReqPath ) == FALSE )
	{
		strMsg.Format ( "Unable to access LoyReq folder ' %s '!", 
			(const char*) strReqPath );
	}
	else
	{
		if ( ::ValidateFolderAccess ( strReqPath ) == FALSE )
		{
			strMsg = "Unable to write to Epos LoyReq folder!";
		}
		else
		{
			CString strMask = strReqPath + "\\*.REQ";
			int nFileCount = ::CountFiles ( strMask );

			strMsg.Format ( "Path to Epos LoyReq folder is valid.\n\nNumber of outstanding requests = %d.", nFileCount );
		}
	}

	CAutoShutdownPropSheetStacker Stacker((CServerPropertySheet*)GetParent());
	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************

int CServerPropertyPage1::ExtractGroupNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup,0);
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CServerPropertyPage1::OnButtonBrowseLoyLog()
{
	CString strPathname;
	if ( DoBrowseFolder ( strPathname, "Locate the LOYLOG Transaction folder",  NULL, this ) == TRUE )
	{
		m_editEposLogPath.SetWindowText( strPathname );
	}
}

//$$******************************************************************

void CServerPropertyPage1::OnButtonTestLoyLog()
{
	CString strMsg;
	CString strLogPath = GetEditBoxText( m_editEposLogPath );

	if ( ::ExistSubdirectory (strLogPath ) == FALSE )
	{
		strMsg.Format ( "Unable to access LoyLog folder ' %s '!",
			(const char*) strLogPath );
	}
	else
	{
		if ( ::ValidateFolderAccess ( strLogPath ) == FALSE )
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

	CAutoShutdownPropSheetStacker Stacker((CServerPropertySheet*)GetParent());
	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************

BOOL CServerPropertyPage1::OnKillActive()
{
	Server.SetLogAction2Flag(IsTicked(m_checkLogAction2));

	/*****/

	Server.SetEposRequestPath(1, GetEditBoxText(m_editEposRequestPath));
	Server.SetAllowOverdraftFlag(IsTicked(m_checkAllowOverdraft));
	Server.SetAddUnknownsFlag(IsTicked(m_checkAddUnknowns));
	Server.SetForenameSearchFieldType(m_comboForename.GetCurSel() + 1);
	Server.SetOfflineSpendLimit(atof(GetEditBoxText(*GetEditOfflineSpendLimit())));
	Server.SetOfflineSpendID(GetEditBoxText(m_editOfflineSpendID));
	Server.SetUnknownGroup(ExtractGroupNo());

	/*****/

	Server.SetEposLogPath(1, GetEditBoxText(m_editEposLogPath));
	Server.SetNewBalanceOnReceiptFlag(IsTicked(m_checkNewBalance));

	return CPropertyPage::OnKillActive();
}

//**********************************************************************

void CServerPropertyPage1::OnOK()
{
	CDbExportHandler exportHandler{};

	if (exportHandler.Read("DEL2") == FALSE)						// see if epos deletion file exists
	{
		exportHandler.Save("DEL2");									// no create it  "C:\VPro\Data\BioDeletions.txt";
	}

	CSSPropertyPage::OnOK();
}

//$$******************************************************************

