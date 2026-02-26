/**********************************************************************/
#include "LocationCSVArray.h"
#include "MaxLengths.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LocationMasterDlg.h"
/**********************************************************************/

CLocationMasterDlg::CLocationMasterDlg( int nLocIdx, int nTNo, bool bShowLocName, CWnd* pParent)
	: CSSDialog(CLocationMasterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocationMasterDlg)
	//}}AFX_DATA_INIT
	m_nLocIdx = nLocIdx;
	m_nTNo = nTNo;
	m_bShowLocName = bShowLocName;
	m_nTabPage = 0;
	m_strReportName = _T("");
	m_strMasterName = _T("");
	m_strPathCommsIn = _T("");
	m_strPathCommsInNow = _T("");
	m_strPathCashRSP = _T("");
	
	m_bInitialEnableMaster = FALSE;
	m_bInitialEnableCashRSP = FALSE;
	if ( LocationMasterMap.GetMasterInfo( nLocIdx, nTNo, m_infoMaster ) == TRUE )
	{
		m_bInitialEnableMaster = m_infoMaster.GetMasterFlag();
		m_bInitialEnableCashRSP = m_infoMaster.GetCashRSPFlag();
	}
}

/**********************************************************************/

void CLocationMasterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocationMasterDlg)
	DDX_Control(pDX, IDC_TAB_SETTINGS, m_TabSettings );
	/*****/
	DDX_Control(pDX, IDC_STATIC_CASHRSP1, m_staticCashRSP1);
	DDX_Control(pDX, IDC_EDIT_REPORT_NAME, m_editReportName);
	DDX_Control(pDX, IDC_CHECK_ENABLE_CASHRSP, m_checkEnableCashRSP);
	DDX_Control(pDX, IDC_EDIT_PATH_CASHRSP, m_editPathCashRSP);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_CASHRSP, m_buttonBrowseCashRSP);
	DDX_Text(pDX, IDC_EDIT_PATH_CASHRSP, m_strPathCashRSP);
	DDX_Text(pDX, IDC_EDIT_REPORT_NAME, m_strReportName);
	DDV_MaxChars(pDX, m_strReportName, MAX_LENGTH_TERMINAL_NAME );
	/*****/
	DDX_Control(pDX, IDC_STATIC_MASTER1, m_staticMaster1);
	DDX_Control(pDX, IDC_STATIC_MASTER2, m_staticMaster2);
	DDX_Control(pDX, IDC_STATIC_MASTER3, m_staticMaster3);
	DDX_Control(pDX, IDC_CHECK_ENABLE_MASTER, m_checkEnableMaster);
	DDX_Control(pDX, IDC_EDIT_MASTER_NAME, m_editMasterName);
	DDX_Control(pDX, IDC_EDIT_PATH_COMMSIN, m_editPathCommsIn);
	DDX_Control(pDX, IDC_EDIT_PATH_COMMSIN_NOW, m_editPathCommsInNow);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_COMMSIN, m_buttonBrowseCommsIn);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_COMMSIN_NOW, m_buttonBrowseCommsInNow);
	DDX_Text(pDX, IDC_EDIT_PATH_COMMSIN, m_strPathCommsIn);
	DDX_Text(pDX, IDC_EDIT_PATH_COMMSIN_NOW, m_strPathCommsInNow);
	DDX_Text(pDX, IDC_EDIT_MASTER_NAME, m_strMasterName);
	DDV_MaxChars(pDX, m_strMasterName, MAX_LENGTH_GENERAL_NAME );
	/*****/
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CLocationMasterDlg, CDialog)
	//{{AFX_MSG_MAP(CLocationMasterDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTINGS, OnTcnSelchangeTabs)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_ENABLE_CASHRSP, OnToggleEnableCashRSP)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_CASHRSP, OnButtonBrowseCashRSP)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MASTER, OnToggleEnableMaster)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_COMMSIN, OnButtonBrowseCommsIn)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_COMMSIN_NOW, OnButtonBrowseCommsInNow)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CLocationMasterDlg::OnInitDialog()
{
	m_strReportName = LocationMasterMap.GetTerminalName(m_nLocIdx, m_nTNo);
	m_strMasterName = m_infoMaster.GetMasterName();
	m_strPathCommsIn = m_infoMaster.GetDeferredFolder();
	m_strPathCommsInNow = m_infoMaster.GetImmediateFolder();
	m_strPathCashRSP = m_infoMaster.GetCashRSPFolder();

	CDialog::OnInitDialog();

	m_TabSettings.InsertItem(0, "Sales && Reports");
	m_TabSettings.InsertItem(1, "Download Master");

	CString strTitle = "";
	if (TRUE == m_bShowLocName)
	{
		strTitle.Format("Configure Terminal Settings (%s, TNo %d)", dbLocation.GetName(m_nLocIdx), m_nTNo);
	}
	else
	{
		strTitle.Format("Configure Terminal Settings (TNo %d)", m_nTNo);
	}

	SetWindowText(strTitle);

	m_checkEnableMaster.SetCheck(m_bInitialEnableMaster);
	m_checkEnableCashRSP.SetCheck(m_bInitialEnableCashRSP);

	MoveControl(&m_staticCashRSP1, 17, 37);
	MoveControl(&m_editReportName, 17, 50);
	MoveControl(&m_checkEnableCashRSP, 17, 80);
	MoveControl(&m_editPathCashRSP, 17, 95);
	MoveControl(&m_buttonBrowseCashRSP, 299, 95);
	MoveControl(&m_buttonOK, 327, 198);
	MoveControl(&m_buttonCancel, 327, 214);
	ResizeDialog(384, 235);

	SelectTab(0);

	return TRUE;
}

/**********************************************************************/

void CLocationMasterDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabPage = m_TabSettings.GetCurSel();
	if ( ( m_nTabPage < 0 ) || ( m_nTabPage >= m_TabSettings.GetItemCount() ) )
		m_nTabPage = 0;

	SelectTab( m_nTabPage );
}

/**********************************************************************/

void CLocationMasterDlg::SelectTab( int nPage )
{
	bool bCashRSP = ( m_checkEnableCashRSP.GetCheck() != 0 );
	bool bMaster = ( m_checkEnableMaster.GetCheck() != 0 );

	ShowAndEnableWindow( &m_staticCashRSP1, ( 0 == nPage ) );
	ShowAndEnableWindow( &m_editReportName, ( 0 == nPage ) );
	ShowAndEnableWindow( &m_checkEnableCashRSP, ( 0 == nPage ) );
	ShowAndEnableWindow( &m_editPathCashRSP, ( 0 == nPage ), ( TRUE == bCashRSP ) );
	ShowAndEnableWindow( &m_buttonBrowseCashRSP, ( 0 == nPage ), ( TRUE == bCashRSP ) );
	/*****/
	ShowAndEnableWindow( &m_checkEnableMaster, ( 1 == nPage ) );
	ShowAndEnableWindow( &m_staticMaster1, ( 1 == nPage ) );
	ShowAndEnableWindow( &m_editMasterName, ( 1 == nPage ), ( TRUE == bMaster ) );
	ShowAndEnableWindow( &m_staticMaster2, ( 1 == nPage ) );
	ShowAndEnableWindow( &m_editPathCommsIn, ( 1 == nPage ), ( TRUE == bMaster ) );
	ShowAndEnableWindow( &m_buttonBrowseCommsIn, ( 1 == nPage ), ( TRUE == bMaster ) );
	ShowAndEnableWindow( &m_staticMaster3, ( 1 == nPage ) );
	ShowAndEnableWindow( &m_editPathCommsInNow, ( 1 == nPage ), ( TRUE == bMaster ) );
	ShowAndEnableWindow( &m_buttonBrowseCommsInNow, ( 1 == nPage ), ( TRUE == bMaster ) );
}

/**********************************************************************/

void CLocationMasterDlg::OnToggleEnableMaster()
{
	bool bEnableMaster = ( m_checkEnableMaster.GetCheck() != 0 );
	m_editMasterName.EnableWindow( bEnableMaster );
	m_editPathCommsIn.EnableWindow( bEnableMaster );
	m_editPathCommsInNow.EnableWindow( bEnableMaster );
	m_buttonBrowseCommsIn.EnableWindow( bEnableMaster );
	m_buttonBrowseCommsInNow.EnableWindow( bEnableMaster );
}

/**********************************************************************/

void CLocationMasterDlg::OnToggleEnableCashRSP()
{
	bool bEnableCashRSP = ( m_checkEnableCashRSP.GetCheck() != 0 );
	m_editPathCashRSP.EnableWindow( bEnableCashRSP );
	m_buttonBrowseCashRSP.EnableWindow( bEnableCashRSP );
}

/**********************************************************************/

void CLocationMasterDlg::OnButtonBrowseCommsIn() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Deferred CommsIn Path", NULL, this ) == TRUE )
		m_editPathCommsIn.SetWindowText ( strPathname );
}

/**********************************************************************/

void CLocationMasterDlg::OnButtonBrowseCommsInNow() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Immediate CommsIn Path", NULL, this ) == TRUE )
		m_editPathCommsInNow.SetWindowText ( strPathname );
}

/**********************************************************************/

void CLocationMasterDlg::OnButtonBrowseCashRSP() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Sales Collection Path", NULL, this ) == TRUE )
		m_editPathCashRSP.SetWindowText ( strPathname );
}

/**********************************************************************/

void CLocationMasterDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		LocationMasterMap.RemoveMaster( m_infoMaster );

		m_infoMaster.SetTerminalName( m_strReportName );

		if ( m_checkEnableMaster.GetCheck() != 0 )
		{
			m_infoMaster.SetMasterFlag( TRUE );
			m_infoMaster.SetMasterName( m_strMasterName );
			m_infoMaster.SetDeferredFolder( m_strPathCommsIn );
			m_infoMaster.SetImmediateFolder( m_strPathCommsInNow );
		}
		else
		{
			m_infoMaster.SetMasterFlag( FALSE );
			m_infoMaster.SetMasterName( "" );
			m_infoMaster.SetDeferredFolder( "" );
			m_infoMaster.SetImmediateFolder( "" );
		}

		if ( m_checkEnableCashRSP.GetCheck() != 0 )
		{
			m_infoMaster.SetCashRSPFlag( TRUE );
			m_infoMaster.SetCashRSPFolder( m_strPathCashRSP );
		}
		else
		{
			m_infoMaster.SetCashRSPFlag( FALSE );
			m_infoMaster.SetCashRSPFolder( "" );
		}

		LocationMasterMap.AddMaster( m_nLocIdx, m_infoMaster, FALSE );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
