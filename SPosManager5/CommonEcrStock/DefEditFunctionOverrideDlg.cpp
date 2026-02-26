/**********************************************************************/
#include "DefEditFunctionOverrideDlg.h"
/**********************************************************************/

CDefEditFunctionOverrideDlg::CDefEditFunctionOverrideDlg( int nIndex, int nConLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditFunctionOverrideDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_nIndex = nIndex;
	m_nConLevel = ( NODE_DATABASE == nConLevel ) ? NODE_DATABASE : NODE_LOCATION; 
	m_pListDataDlg = NULL;
	//{{AFX_DATA_INIT(CDefEditFunctionOverrideDlg)
	//}}AFX_DATA_INIT
	
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditFunctionOverrideDlg)
	DDX_Control(pDX, IDC_STATIC_SETTINGS, m_staticSettings);
	DDX_Control(pDX, IDC_CHECK_OVERRIDE, m_checkOverride);
	DDX_Control(pDX, IDC_CHECK_PASSWORD, m_checkPassword);
	DDX_Control(pDX, IDC_CHECK_AUDIT, m_checkAudit);
	DDX_Control(pDX, IDC_CHECK_LOG, m_checkLog);
	DDX_Control(pDX, IDC_STATIC_ORIGIN, m_staticOrigin);
	DDX_Control(pDX, IDC_EDIT_LEVEL_DEFAULT, m_editLevelDefault);
	DDX_Control(pDX, IDC_CHECK_PASSWORD_DEFAULT, m_checkPasswordDefault);
	DDX_Control(pDX, IDC_CHECK_AUDIT_DEFAULT, m_checkAuditDefault);
	DDX_Control(pDX, IDC_CHECK_LOG_DEFAULT, m_checkLogDefault);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditFunctionOverrideDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditFunctionOverrideDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_CHECK_OVERRIDE, OnToggleOverride)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditFunctionOverrideDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( NODE_DATABASE == m_nConLevel )
	{
		m_staticSettings.SetWindowText( "Database Settings" );
		m_staticOrigin.ShowWindow( SW_HIDE );
	}
	else
	{
		m_staticSettings.SetWindowText( "Location Settings" );
	}

	SubclassEdit( IDC_EDIT_LEVEL, SS_NUM, 1, "%d" );
	GetData();
	return FALSE;  
}

/**********************************************************************/
CEdit* CDefEditFunctionOverrideDlg::GetEditLevel(){ return GetEdit( IDC_EDIT_LEVEL ); }
/**********************************************************************/

void CDefEditFunctionOverrideDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex <= 0 )
			return;

		SaveData();
		m_nIndex--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine( m_nIndex );
	}
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.FunctionSystem.GetFunctionCount() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::GetData()
{
	CString strTitle;

	CFunctionAuthMiniInfo infoMini;

	if ( NODE_DATABASE == m_nConLevel )
	{
		DataManagerNonDb.FunctionDatabase.GetFunction( m_nIndex, infoMini );
		DataManagerNonDb.FunctionSystem.GetFunction( m_nIndex, m_infoDefault );
	}
	else
	{
		DataManagerNonDb.FunctionLocation.GetFunction( m_nIndex, infoMini );
		DataManagerNonDb.FunctionDatabase.GetFunction( m_nIndex, m_infoDefault );
		
		if ( m_infoDefault.m_nAuthLevel < 0 )
		{
			m_staticOrigin.SetWindowText( "(From system)" );
			DataManagerNonDb.FunctionSystem.GetFunction( m_nIndex, m_infoDefault );
		}
		else
		{
			m_staticOrigin.SetWindowText( "(From database)" );
		}
	}
	
	{
		strTitle.Format ( "F%4.4d %s", infoMini.m_nFuncNo, (const char*) infoMini.m_strName );
		SetWindowText ( strTitle );
	}

	{
		CString strAuth;
		strAuth.Format( "%d", m_infoDefault.m_nAuthLevel );
		m_editLevelDefault.SetWindowText( strAuth );
	}

	m_checkPasswordDefault.SetCheck( m_infoDefault.m_bPassword );
	m_checkAuditDefault.SetCheck( m_infoDefault.m_bAudit );
	m_checkLogDefault.SetCheck( m_infoDefault.m_bLog );

	if ( infoMini.m_nAuthLevel >= 0 )
	{
		m_checkOverride.SetCheck( TRUE );

		GetEditLevel() -> EnableWindow( TRUE );
		m_checkPassword.EnableWindow( TRUE );
		m_checkAudit.EnableWindow( TRUE );
		m_checkLog.EnableWindow( TRUE );
	
		CString strLevel;
		strLevel.Format( "%d", infoMini.m_nAuthLevel );
		GetEditLevel() -> SetWindowText( strLevel );
	
		m_checkPassword.SetCheck( infoMini.m_bPassword );
		m_checkAudit.SetCheck( infoMini.m_bAudit );
		m_checkLog.SetCheck( infoMini.m_bLog );

		GetEditLevel() -> SetSel ( 0, -1 );
		GetEditLevel() -> SetFocus();
	}
	else
	{
		m_checkOverride.SetCheck( FALSE );
		CopyDefaultSettings();
		m_checkOverride.SetFocus();
	}
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::CopyDefaultSettings()
{
	GetEditLevel() -> EnableWindow( FALSE );
	m_checkPassword.EnableWindow( FALSE );
	m_checkAudit.EnableWindow( FALSE );
	m_checkLog.EnableWindow( FALSE );

	CString strLevel;
	strLevel.Format( "%d", m_infoDefault.m_nAuthLevel );
	GetEditLevel() -> SetWindowText( strLevel );
	
	m_checkPassword.SetCheck( m_infoDefault.m_bPassword );
	m_checkAudit.SetCheck( m_infoDefault.m_bAudit );
	m_checkLog.SetCheck( m_infoDefault.m_bLog );
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::SaveData()
{
	CFunctionAuthMiniInfo infoMini;

	if ( NODE_DATABASE == m_nConLevel )
		DataManagerNonDb.FunctionDatabase.GetFunction( m_nIndex, infoMini );
	else
		DataManagerNonDb.FunctionLocation.GetFunction( m_nIndex, infoMini );

	if ( m_checkOverride.GetCheck() != 0 )
	{
		CString strLevel;
		GetEditLevel() -> GetWindowText( strLevel );

		infoMini.m_nAuthLevel = atoi( strLevel );
		infoMini.m_bPassword = ( m_checkPassword.GetCheck() != 0 );
		infoMini.m_bAudit = ( m_checkAudit.GetCheck() != 0 );
		infoMini.m_bLog = ( m_checkLog.GetCheck() != 0 );
	}
	else
	{
		infoMini.m_nAuthLevel = -1;
		infoMini.m_bPassword = FALSE;
		infoMini.m_bAudit = FALSE;
		infoMini.m_bLog = FALSE;
	}
	
	if ( NODE_DATABASE == m_nConLevel )
		DataManagerNonDb.FunctionDatabase.UpdateFunction( m_nIndex, infoMini );
	else
		DataManagerNonDb.FunctionLocation.UpdateFunction( m_nIndex, infoMini );

	m_WarnCancelAfterChange.SetFlag();
 }

/**********************************************************************/

void CDefEditFunctionOverrideDlg::OnToggleOverride() 
{
	if ( m_checkOverride.GetCheck() != 0 )
	{
		GetEditLevel() -> EnableWindow( TRUE );
		m_checkPassword.EnableWindow( TRUE );
		m_checkAudit.EnableWindow( TRUE );
		m_checkLog.EnableWindow( TRUE );
	}
	else
	{
		CopyDefaultSettings();
	}
}

/**********************************************************************/

void CDefEditFunctionOverrideDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
