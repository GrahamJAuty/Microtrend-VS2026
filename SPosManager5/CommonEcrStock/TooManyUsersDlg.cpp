/**********************************************************************/
#include "TooManyUsersDlg.h"
/**********************************************************************/

CTooManyUsersDlg::CTooManyUsersDlg(CWnd* pParent) : CDialog(CTooManyUsersDlg::IDD, pParent)
{
}

/**********************************************************************/

CTooManyUsersDlg::~CTooManyUsersDlg()
{
}

/**********************************************************************/

void CTooManyUsersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINE1,m_staticLine1);
	DDX_Control(pDX, IDC_STATIC_LINE2,m_staticLine2);
	DDX_Control(pDX, IDC_STATIC_LINE3,m_staticLine3);
	DDX_Control(pDX, IDC_STATIC_LINE4,m_staticLine4);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CTooManyUsersDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INFO,OnButtonInfo)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CTooManyUsersDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nMaxUsers = Sysset.GetMaxUsers();
		
	{
		CString strUsers;
		if ( 1 == nMaxUsers )
			strUsers = "1 user";
		else 
			strUsers.Format( "%d users", nMaxUsers );

		CString strLine1;
		strLine1.Format( "%s is licensed for use by %s at any one time.",
			AfxGetAppName(),
			(const char*) strUsers  );

		m_staticLine1.SetWindowText( strLine1 );
	}

	{
		CString strUsers;
		if ( 1 == nMaxUsers )
			strUsers = "a user is";
		else 
			strUsers.Format( "%d users are", nMaxUsers );

		CString strLine2;
		strLine2.Format( "As %s already logged on, you cannot start another session now.",
			(const char*) strUsers  );

		m_staticLine2.SetWindowText( strLine2 );
	}

	{
		CString strLine3 = "Click <More Info> for further information about the currently active session";
		strLine3 += ( 1 == nMaxUsers ) ? "." : "s.";
		m_staticLine3.SetWindowText( strLine3 );
	}

	{
		CString strLine4;
		strLine4.Format( "Click <OK> to close this message and exit %s.",
			AfxGetAppName() );
		m_staticLine4.SetWindowText( strLine4 );
	}

	return TRUE;
}

/**********************************************************************/

void CTooManyUsersDlg::OnButtonInfo()
{
	Sysset.DisplayUserList();
}

/**********************************************************************/
