//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "DefEditDlg.h"
//$$******************************************************************
#include "ServiceLogonDlg.h"
//$$******************************************************************

CServiceLogonDlg::CServiceLogonDlg( bool bBackground, CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CServiceLogonDlg::IDD, pParent)
{
	m_bBackground = bBackground;
	m_strLastUsername = "";
	m_strLastPassword = "<No password set>";
	
	if ( TRUE == m_bBackground )
	{
		m_pServiceOptions = &BgndServiceOptions;
	}
	else
	{
		m_pServiceOptions = &ServerServiceOptions;
	}
}

//$$******************************************************************

void CServiceLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CHECK_LOGON, m_checkLogon );
	DDX_Control( pDX, IDC_EDIT_USERNAME, m_editUsername );
	DDX_Control( pDX, IDC_EDIT_PASSWORD, m_editPassword );
	DDX_Control( pDX, IDC_BUTTON_PASSWORD, m_buttonPassword );
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CServiceLogonDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED( IDC_CHECK_LOGON, OnToggleLogon )
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword )
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CServiceLogonDlg::CSSAutoShutdownPostInitDialog()
{
	if ( TRUE == m_bBackground )
	{
		SetWindowText( "Background Processor Service" );
	}
	else
	{
		SetWindowText( "SmartPay Server Service" );
	}

	m_bLogonState = m_pServiceOptions -> GetWindowsLogonFlag();
	m_checkLogon.SetCheck( m_bLogonState );
	m_editUsername.EnableWindow( m_bLogonState );
	m_buttonPassword.EnableWindow( m_bLogonState );

	if ( TRUE == m_bLogonState )
	{
		m_editUsername.SetWindowText( m_pServiceOptions -> GetWindowsUsername() );

		CString strPassword = m_pServiceOptions -> GetWindowsPassword();
		
		if ( strPassword != "" )
		{
			m_editPassword.SetWindowText( "<The password has been set>" );
			m_bGotOldPassword = TRUE;
		}
		else
		{
			m_editPassword.SetWindowText( "<No password set>" );
			m_bGotOldPassword = FALSE;
		}
	}
	else
	{
		m_editUsername.SetWindowText( "" );
		m_editPassword.SetWindowText( "" );
		m_bGotOldPassword = FALSE;
	}

	return TRUE;
}

//$$******************************************************************

void CServiceLogonDlg::OnButtonPassword()
{
	CAutoShutdownDlgStacker Stacker(this);

	CDefEditASDlg dlg( "Set Windows Password for Service", "", MAX_LENGTH_WINDOWS_PASSWORD, "Password", this );
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_strName != "" )
		{
			m_strNewPassword = dlg.m_strName;
			
			if ( TRUE == m_bGotOldPassword )
				m_editPassword.SetWindowText ( "<The password has been changed>" );
			else
				m_editPassword.SetWindowText ( "<The password has been set>" );
		}
	}
}

/**********************************************************************/

void CServiceLogonDlg::OnToggleLogon()
{
	bool bNewState = IsTicked( m_checkLogon );

	if ( bNewState != m_bLogonState )
	{
		m_bLogonState = bNewState;

		if ( TRUE == m_bLogonState )
		{
			m_buttonPassword.EnableWindow( TRUE );
			m_editUsername.EnableWindow( TRUE );
			m_editUsername.SetWindowText( m_strLastUsername );
			m_editPassword.SetWindowText( m_strLastPassword );
		}
		else
		{
			m_buttonPassword.EnableWindow( FALSE );
			m_editUsername.EnableWindow( FALSE );
			m_strLastUsername = GetEditBoxText( m_editUsername );
			m_strLastPassword = GetEditBoxText( m_editPassword );
			m_editUsername.SetWindowText( "" );
			m_editPassword.SetWindowText( "" );
		}
	}
}

/**********************************************************************/

void CServiceLogonDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

//$$******************************************************************

void CServiceLogonDlg::OnOK()
{
	UpdateServiceOptions();
	EndDialog( IDOK );
}

//$$******************************************************************

void CServiceLogonDlg::UpdateServiceOptions()
{
	if ( IsTicked( m_checkLogon ) )
	{
		m_pServiceOptions -> SetWindowsLogonFlag( TRUE );
		m_pServiceOptions -> SetWindowsUsername( GetEditBoxText( m_editUsername ) );

		if ( m_strNewPassword != "" )
		{
			m_pServiceOptions -> SetWindowsPassword( m_strNewPassword );
		}
	}
	else
	{
		m_pServiceOptions -> SetWindowsLogonFlag( FALSE );
		m_pServiceOptions -> SetWindowsUsername( "" );
		m_pServiceOptions -> SetWindowsPassword( "" );
	}
}

//$$******************************************************************

