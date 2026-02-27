//$$******************************************************************
#include "SetDailyAlternateGroupDlg.h"
//$$******************************************************************

CSetDailyAlternateGroupDlg::CSetDailyAlternateGroupDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetDailyAlternateGroupDlg::IDD, pParent)
{
	for (int n = 1; n <= 7; n++)
	{
		m_nGroupDayNumber[n - 1] = 0;
	}
}

//$$******************************************************************

CSetDailyAlternateGroupDlg::~CSetDailyAlternateGroupDlg()
{
}

//$$******************************************************************

void CSetDailyAlternateGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY1, m_nGroupDayNumber[0]);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY2, m_nGroupDayNumber[1]);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY3, m_nGroupDayNumber[2]);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY4, m_nGroupDayNumber[3]);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY5, m_nGroupDayNumber[4]);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY6, m_nGroupDayNumber[5]);
	DDX_Text(pDX, IDC_EDIT_GROUPDAY7, m_nGroupDayNumber[6]);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[0], 0, 99);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[1], 0, 99);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[2], 0, 99);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[3], 0, 99);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[4], 0, 99);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[5], 0, 99);
	DDV_MinMaxInt(pDX, m_nGroupDayNumber[6], 0, 99);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetDailyAlternateGroupDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

//$$******************************************************************
