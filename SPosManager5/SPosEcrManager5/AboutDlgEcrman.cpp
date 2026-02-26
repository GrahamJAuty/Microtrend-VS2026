/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "AboutDlgEcrman.h"
/**********************************************************************/
extern const char* szVERSION_ABOUT;
/**********************************************************************/

CAboutDlgEcrman::CAboutDlgEcrman() : CDialog("IDD_ABOUTBOX_ECRMAN")
{
	//{{AFX_DATA_INIT(CAboutDlgEcrman)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CAboutDlgEcrman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlgEcrman)
	DDX_Control(pDX, IDC2_EDIT_USERS, m_editUsers);
	DDX_Control(pDX, IDC2_STATIC_ICON, m_staticIcon);
	DDX_Control(pDX, IDC2_STATIC_VERSION, m_staticVersion);
	DDX_Control(pDX, IDC2_STATIC_SERIAL, m_staticSerial);
	DDX_Control(pDX, IDC2_STATIC_LITETYPE, m_staticLiteType);
	DDX_Control(pDX, IDC2_COMBO_LIBS, m_comboLibs);
	DDX_Control(pDX, IDC2_BUTTON_USERS, m_buttonUsers);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAboutDlgEcrman, CDialog)
	//{{AFX_MSG_MAP(CAboutDlgEcrman)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC2_BUTTON_USERS, OnButtonUsers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAboutDlgEcrman::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_staticIcon.SetIcon(hIcon);

	CString strTitle = "About ";

	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		strTitle += AfxGetAppName();
		m_staticLiteType.ShowWindow(SW_HIDE);
	}
	else
	{
		strTitle += "Ecr Manager Lite";
		m_staticLiteType.SetWindowText(SysInfo.GetEcrLiteVersionDetail());
	}

	SetWindowText(strTitle);
	m_staticVersion.SetWindowText(Sysset.GetVersion(szVERSION_ABOUT));

	m_comboLibs.AddString("Libraries");
	m_comboLibs.AddString(szVERSION_SUBS);
	m_comboLibs.AddString(szVERSION_WINSUBS);
	m_comboLibs.AddString(szVERSION_REPSUBS);
	m_comboLibs.AddString(szVERSION_DATETIMESUBS);
	m_comboLibs.AddString(szVERSION_SSMAPI32);
	m_comboLibs.AddString(szVERSION_SORT);
	m_comboLibs.AddString(szVERSION_SYSSET3);
	m_comboLibs.AddString(szVERSION_EXP);
	m_comboLibs.AddString(szVERSION_XML);
	m_comboLibs.AddString(szVERSION_SMARTUID);
	m_comboLibs.AddString(szVERSION_COMMS);
	
	m_comboLibs.ShowWindow(SW_HIDE);
	m_comboLibs.SetCurSel(0);

	CString str;
	str.Format("%d", Sysset.GetMaxUsers());
	m_editUsers.SetWindowText(str);
	m_editUsers.EnableWindow(FALSE);

	CString strSerial;
	strSerial.Format("Serial No. %6.6d", Sysset.GetSerialNo());
	m_staticSerial.SetWindowText(strSerial);

	return TRUE;
}

/**********************************************************************/

void CAboutDlgEcrman::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_comboLibs.ShowWindow(SW_SHOW);
	CDialog::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CAboutDlgEcrman::OnButtonUsers()
{
	Sysset.DisplayUserList();
}

/**********************************************************************/


