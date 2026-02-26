// RefreshYearlyDlg.cpp : implementation file
//

#include "LoyaltyManager.h"
#include "RefreshYearlyDlg.h"


// CRefreshYearlyDlg dialog

CRefreshYearlyDlg::CRefreshYearlyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRefreshYearlyDlg::IDD, pParent)
	, m_oleDateTime(COleDateTime::GetCurrentTime())
{

}

CRefreshYearlyDlg::~CRefreshYearlyDlg()
{
}

void CRefreshYearlyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_oleDateTime);
}


BEGIN_MESSAGE_MAP(CRefreshYearlyDlg, CDialog)
END_MESSAGE_MAP()


// CRefreshYearlyDlg message handlers
