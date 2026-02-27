// RefreshYearDlg.cpp : implementation file
//

#include "RefreshYearDlg.h"


// CRefreshYearDlg dialog

CRefreshYearDlg::CRefreshYearDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CRefreshYearDlg::IDD, pParent)
	, m_oleDateTime(COleDateTime::GetCurrentTime())
{

}

CRefreshYearDlg::~CRefreshYearDlg()
{
}

void CRefreshYearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_oleDateTime);
}


BEGIN_MESSAGE_MAP(CRefreshYearDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()


// CRefreshYearDlg message handlers
