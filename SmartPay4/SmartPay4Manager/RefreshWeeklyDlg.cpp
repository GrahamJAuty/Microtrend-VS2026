//**********************************************************************
#include "RefreshWeeklyDlg.h"
//**********************************************************************

CRefreshWeeklyDlg::CRefreshWeeklyDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CRefreshWeeklyDlg::IDD, pParent)
{
	m_nRefreshDay = 0;
}

//**********************************************************************

CRefreshWeeklyDlg::~CRefreshWeeklyDlg()
{
}

//**********************************************************************

void CRefreshWeeklyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DAY0, m_nRefreshDay);
}

//**********************************************************************
BEGIN_MESSAGE_MAP(CRefreshWeeklyDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
//**********************************************************************
