/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "AboutDlgPosTray.h"
/**********************************************************************/

CAboutDlgPosTray::CAboutDlgPosTray(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlgPosTray::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlgPosTray)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CAboutDlgPosTray::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlgPosTray)
	DDX_Control(pDX, IDC_EDIT_USERS, m_editUsers);
	DDX_Control(pDX, IDC_STATIC_ICON, m_staticIcon);
	DDX_Control(pDX, IDC_COMBO_LIBS, m_comboLibs);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAboutDlgPosTray, CDialog)
	//{{AFX_MSG_MAP(CAboutDlgPosTray)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
extern const char* szVERSION_ABOUT;
/**********************************************************************/

BOOL CAboutDlgPosTray::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp() -> LoadIcon( IDR_MAINFRAME );
	m_staticIcon.SetIcon( hIcon );
	
	CString strTitle = "About ";
	strTitle += AfxGetAppName();
	SetWindowText ( strTitle );
	
	m_staticVersion.SetWindowText ( Sysset.GetVersion( szVERSION_ABOUT ) );

	m_comboLibs.AddString ( "Libraries" );
	m_comboLibs.AddString ( szVERSION_SUBS );
	m_comboLibs.AddString ( szVERSION_WINSUBS );
	m_comboLibs.AddString ( szVERSION_REPSUBS );
	m_comboLibs.AddString ( szVERSION_DATETIMESUBS );
	m_comboLibs.AddString ( szVERSION_SSMAPI32 );
	m_comboLibs.AddString ( szVERSION_SORT );
	m_comboLibs.AddString ( szVERSION_SYSSET3 );
	m_comboLibs.AddString ( szVERSION_EXP );
	
	m_comboLibs.ShowWindow ( SW_HIDE );	
	m_comboLibs.SetCurSel ( 0 );

	CString str;
	str.Format( "%d", Sysset.GetMaxUsers() );
	m_editUsers.SetWindowText( str );
	
	m_editUsers.EnableWindow( FALSE );

	return TRUE;  
}

/**********************************************************************/

void CAboutDlgPosTray::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_comboLibs.ShowWindow ( SW_SHOW );
	CDialog::OnRButtonDown(nFlags, point);
}

/**********************************************************************/
