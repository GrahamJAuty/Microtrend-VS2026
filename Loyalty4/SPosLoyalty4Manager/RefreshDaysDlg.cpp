#include "LoyaltyManager.h"
#include "RefreshDaysDlg.h"

// CRefreshDaysDlg dialog

CRefreshDaysDlg::CRefreshDaysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRefreshDaysDlg::IDD, pParent)
	, m_bDay1(FALSE)
	, m_bDay2(FALSE)
	, m_bDay3(FALSE)
	, m_bDay4(FALSE)
	, m_bDay5(FALSE)
	, m_bDay6(FALSE)
	, m_bDay7(FALSE)
{
}

CRefreshDaysDlg::~CRefreshDaysDlg()
{
}

void CRefreshDaysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_DAY1, m_bDay1);
	DDX_Check(pDX, IDC_CHECK_DAY2, m_bDay2);
	DDX_Check(pDX, IDC_CHECK_DAY3, m_bDay3);
	DDX_Check(pDX, IDC_CHECK_DAY4, m_bDay4);
	DDX_Check(pDX, IDC_CHECK_DAY5, m_bDay5);
	DDX_Check(pDX, IDC_CHECK_DAY6, m_bDay6);
	DDX_Check(pDX, IDC_CHECK_DAY7, m_bDay7);
}

BEGIN_MESSAGE_MAP(CRefreshDaysDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_5DAYS, OnBnClickedButton5days)
	ON_BN_CLICKED(IDC_BUTTON_7DAYS, OnBnClickedButton7days)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()

//******************************************************************
// CRefreshDaysDlg message handlers

void CRefreshDaysDlg::OnBnClickedButton5days()
{
	if ( UpdateData() == TRUE )
	{
		m_bDay2 = TRUE;
		m_bDay3 = TRUE;
		m_bDay4 = TRUE;
		m_bDay5 = TRUE;
		m_bDay6 = TRUE;
		UpdateData(FALSE);
	}
}

//******************************************************************

void CRefreshDaysDlg::OnBnClickedButton7days()	{ SetAllDays ( TRUE ); }
void CRefreshDaysDlg::OnBnClickedButtonClear()	{ SetAllDays ( FALSE ); }

void CRefreshDaysDlg::SetAllDays ( bool bReqd )
{
	if ( UpdateData() == TRUE )
	{
		m_bDay1 = bReqd;
		m_bDay2 = bReqd;
		m_bDay3 = bReqd;
		m_bDay4 = bReqd;
		m_bDay5 = bReqd;
		m_bDay6 = bReqd;
		m_bDay7 = bReqd;
		UpdateData(FALSE);
	}
}

//******************************************************************