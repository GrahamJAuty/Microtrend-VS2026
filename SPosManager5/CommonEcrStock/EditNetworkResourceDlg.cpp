/**********************************************************************/
 
/**********************************************************************/
#include "NetworkResources.h"
/**********************************************************************/
#include "EditNetworkResourceDlg.h"
/**********************************************************************/

CEditNetworkResourceDlg::CEditNetworkResourceDlg( int nResourceNo, CWnd* pParent)
	: CSSDialog(CEditNetworkResourceDlg::IDD, pParent)
{
	m_nResourceNo = nResourceNo;

	//{{AFX_DATA_INIT(CEditNetworkResourceDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CEditNetworkResourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditNetworkResourceDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_RESOURCE, m_editResource );
	DDX_Control( pDX, IDC_EDIT_USER, m_editUser );
	DDX_Control( pDX, IDC_EDIT_PASSWORD, m_editPassword );
	DDX_Control( pDX, IDC_STATIC_PASSWORD, m_staticPassword );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEditNetworkResourceDlg, CDialog)
	//{{AFX_MSG_MAP(CEditNetworkResourceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEditNetworkResourceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format( "Configure Access to Network Resource #%d", m_nResourceNo + 1 );
	SetWindowText( strTitle );

	m_editResource.LimitText( MAX_LENGTH_NETWORKRESOURCE_NAME );
	m_editUser.LimitText( MAX_LENGTH_NETWORKRESOURCE_USER );
	m_editPassword.LimitText( MAX_LENGTH_NETWORKRESOURCE_PASSWORD );

	CString strResource = NetworkResources.GetResourceName( m_nResourceNo );
	CString strUser = NetworkResources.GetUserName( m_nResourceNo );
	CString strPassword = NetworkResources.GetPassword( m_nResourceNo );

	m_editResource.SetWindowText( strResource );
	m_editUser.SetWindowText( strUser );
	m_editPassword.SetWindowText( "" );

	if ( strPassword == "" )
		m_staticPassword.ShowWindow( SW_HIDE );

	return TRUE;  
}

/**********************************************************************/

void CEditNetworkResourceDlg::OnOK() 
{
	CString strText;
	
	m_editResource.GetWindowText( strText );
	NetworkResources.SetResourceName( m_nResourceNo, strText );

	m_editUser.GetWindowText( strText );
	NetworkResources.SetUserName( m_nResourceNo, strText );

	m_editPassword.GetWindowText( strText );
	if ( strText != "" )
		NetworkResources.SetPassword( m_nResourceNo, strText );

	EndDialog ( IDOK );
}

/**********************************************************************/
