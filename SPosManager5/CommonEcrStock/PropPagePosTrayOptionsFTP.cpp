/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "MaxLengths.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsFTP.h"
/**********************************************************************/

CPropPagePosTrayOptionsFTP::CPropPagePosTrayOptionsFTP() : CSSPropertyPage(CPropPagePosTrayOptionsFTP::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsFTP)
	//}}AFX_DATA_INIT
	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
}

/**********************************************************************/

CPropPagePosTrayOptionsFTP::~CPropPagePosTrayOptionsFTP()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsFTP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsFTP)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_SERVER, m_editServer );
	DDX_Control( pDX, IDC_EDIT_USERNAME, m_editUserName );
	DDX_Control( pDX, IDC_EDIT_PASSWORD, m_editPassword );
	DDX_Control( pDX, IDC_BUTTON_PASSWORD, m_buttonPassword );
	DDX_Control( pDX, IDC_CHECK_UNIXMODE, m_checkUnixMode );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsFTP, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsFTP)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsFTP::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_editServer.LimitText( MAX_LENGTH_FTP_SERVER );
	m_editUserName.LimitText( MAX_LENGTH_FTP_USERNAME );

	m_editServer.SetWindowText( PosTrayOptions.GetFTPServer() );
	m_editUserName.SetWindowText( PosTrayOptions.GetFTPUserName() );

	CString strPassword = PosTrayOptions.GetFTPPassword();

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

	m_checkUnixMode.SetCheck( PosTrayOptions.GetFTPUnixModeFlag() );

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsFTP::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsFTP::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayOptionsFTP::OnButtonPassword()
{
	CDefEditDlg dlg( "Set FTP Password", "", MAX_LENGTH_FTP_PASSWORD, "Password", this );
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

void CPropPagePosTrayOptionsFTP::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsFTP::SaveRecord()
{
	CString str;
	m_editServer.GetWindowText( str );
	PosTrayOptions.SetFTPServer( str );

	m_editUserName.GetWindowText( str );
	PosTrayOptions.SetFTPUserName( str );

	if ( m_strNewPassword != "" )
		PosTrayOptions.SetFTPPassword( m_strNewPassword );

	PosTrayOptions.SetFTPUnixModeFlag( m_checkUnixMode.GetCheck() != 0 );
}

/**********************************************************************/
