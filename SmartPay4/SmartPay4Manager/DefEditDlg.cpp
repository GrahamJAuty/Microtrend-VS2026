#include "DefEditDlg.h"
/**********************************************************************/

CDefEditDlg::CDefEditDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CDefEditDlg::IDD, pParent)
{
	m_strPrompt = "";
}

/**********************************************************************/

CDefEditDlg::CDefEditDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CDefEditDlg::IDD, pParent)
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

BEGIN_MESSAGE_MAP(CDefEditDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CDefEditDlg)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditDlg::CSSAutoShutdownPostInitDialog()
{
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
/**********************************************************************/
/**********************************************************************/

CDefEditASDlg::CDefEditASDlg( CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CDefEditASDlg::IDD, pParent)
{
	m_strPrompt = "";
}

/**********************************************************************/

CDefEditASDlg::CDefEditASDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt, CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CDefEditASDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditASDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
	m_strName = szName;
	m_nMaxLen = nMaxLen;
	m_strPrompt = szPrompt;
}

/**********************************************************************/

void CDefEditASDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditASDlg)
	DDX_Control(pDX, IDC_STATIC_REF, m_staticRef);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditASDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CDefEditASDlg)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditASDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strTitle);

	if (m_strPrompt != "")
		m_staticRef.SetWindowText(m_strPrompt);

	m_editName.LimitText(m_nMaxLen);
	m_editName.SetWindowText(m_strName);

	SetDialogSize();

	return TRUE;
}

/**********************************************************************/

void CDefEditASDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		m_editName.GetWindowText(m_strName);
		::TrimSpacesFromString(m_strName, FALSE);
		EndDialog(IDOK);
	}
}

/**********************************************************************/

