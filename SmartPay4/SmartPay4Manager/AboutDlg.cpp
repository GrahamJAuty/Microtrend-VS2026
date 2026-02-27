//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "AboutDlg.h"
//$$******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CAboutDlg::IDD, pParent)
{
	m_nMaxUsers	= Sysset.GetMaxUsers();
}

//$$******************************************************************

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LIBS, m_comboLibs);
	DDX_Control(pDX, IDC_STATIC_SERIALNO, m_staticSerialNo);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
	DDX_Text(pDX, IDC_EDIT_MAXUSERS, m_nMaxUsers);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAboutDlg, CSSAutoShutdownDialog)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_USERS, OnButtonUsers)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAboutDlg::CSSAutoShutdownPostInitDialog()
{
	m_staticVersion.SetWindowText(Sysset.GetVersion(szVERSION_SMARTPAY_ABOUT));

	CString strMsg = "";
	strMsg.Format("Serial No.  %6.6d", Sysset.GetSerialNo());
	m_staticSerialNo.SetWindowText(strMsg);

	m_comboLibs.AddString("Libraries");
	m_comboLibs.AddString(szVERSION_COMMS);
	m_comboLibs.AddString(szVERSION_DATETIMESUBS);
	m_comboLibs.AddString(szVERSION_DBASE);
	m_comboLibs.AddString(szVERSION_EXP);
	m_comboLibs.AddString(szVERSION_REPSUBS);
	m_comboLibs.AddString(szVERSION_SMARTUID);
	m_comboLibs.AddString(szVERSION_SORT);
	m_comboLibs.AddString(szVERSION_SSMAPI32);
	m_comboLibs.AddString(szVERSION_SUBS);
	m_comboLibs.AddString(szVERSION_SYSSET3);
	m_comboLibs.AddString(szVERSION_WINSUBS);
	m_comboLibs.AddString(szVERSION_XML);
	
	m_comboLibs.ShowWindow(SW_HIDE);
	m_comboLibs.SetCurSel(0);

	return TRUE;
}

//$$******************************************************************

void CAboutDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_comboLibs.ShowWindow ( SW_SHOW );

	CSSAutoShutdownDialog::OnRButtonDown(nFlags, point);
}

//$$******************************************************************

void CAboutDlg::OnButtonUsers()
{
	Sysset.DisplayUserList();
}

//$$******************************************************************
