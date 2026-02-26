#include "DefEditDlg.h"
/**********************************************************************/

CDefEditDlg::CDefEditDlg(CWnd* pParent)
	: CSSDialog(CDefEditDlg::IDD, pParent)
{
	m_strPrompt = "";
	m_nMaxLen = 0;
	m_strTitle = "";
	m_strName = "";
}

/**********************************************************************/

CDefEditDlg::CDefEditDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt, CWnd* pParent)
	: CSSDialog(CDefEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
	m_strName = szName;
	m_nMaxLen = nMaxLen;
	m_strPrompt = szPrompt;
}

/**********************************************************************/

void CDefEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditDlg)
	DDX_Control(pDX, IDC_STATIC_REF, m_staticRef);
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control( pDX, IDOK, m_buttonOK);
	DDX_Control( pDX, IDCANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditDlg)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );

	if ( m_strPrompt != "" )
		m_staticRef.SetWindowText ( m_strPrompt );

	m_editName.LimitText( m_nMaxLen );
	m_editName.SetWindowText( m_strName );

	SetDialogSize();

	return TRUE;  
}

/**********************************************************************/

void CDefEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_editName.GetWindowText( m_strName );
		::TrimSpacesFromString( m_strName, FALSE );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
