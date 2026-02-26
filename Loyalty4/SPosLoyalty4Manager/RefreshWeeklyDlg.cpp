// RefreshWeeklyDlg.cpp : implementation file
//

#include "LoyaltyManager.h"
#include "RefreshWeeklyDlg.h"


// CRefreshWeeklyDlg dialog

CRefreshWeeklyDlg::CRefreshWeeklyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRefreshWeeklyDlg::IDD, pParent)
	, m_nRefreshDay(0)
{

}

CRefreshWeeklyDlg::~CRefreshWeeklyDlg()
{
}

void CRefreshWeeklyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DAY0, m_nRefreshDay);
}


BEGIN_MESSAGE_MAP(CRefreshWeeklyDlg, CDialog)
END_MESSAGE_MAP()


// CRefreshWeeklyDlg message handlers
