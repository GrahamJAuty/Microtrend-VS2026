//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "RefreshDaysDlg.h"
//$$******************************************************************

CRefreshDaysDlg::CRefreshDaysDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CRefreshDaysDlg::IDD, pParent)
{
	m_bGroupsAllowed = FALSE;

	for (int n = 0; n < 7; n++)
	{
		m_bDayFlag[n] = FALSE;
		m_nGroupDayNumber[n] = 0;
	}
}

//$$******************************************************************

CRefreshDaysDlg::~CRefreshDaysDlg()
{
}

//$$******************************************************************

void CRefreshDaysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT_GROUPDAY1, m_editGroupDayNumber[0]);
	DDX_Control(pDX, IDC_EDIT_GROUPDAY2, m_editGroupDayNumber[1]);
	DDX_Control(pDX, IDC_EDIT_GROUPDAY3, m_editGroupDayNumber[2]);
	DDX_Control(pDX, IDC_EDIT_GROUPDAY4, m_editGroupDayNumber[3]);
	DDX_Control(pDX, IDC_EDIT_GROUPDAY5, m_editGroupDayNumber[4]);
	DDX_Control(pDX, IDC_EDIT_GROUPDAY6, m_editGroupDayNumber[5]);
	DDX_Control(pDX, IDC_EDIT_GROUPDAY7, m_editGroupDayNumber[6]);
	DDX_Check(pDX, IDC_CHECK_DAY1, m_bDayFlag[0]);
	DDX_Check(pDX, IDC_CHECK_DAY2, m_bDayFlag[1]);
	DDX_Check(pDX, IDC_CHECK_DAY3, m_bDayFlag[2]);
	DDX_Check(pDX, IDC_CHECK_DAY4, m_bDayFlag[3]);
	DDX_Check(pDX, IDC_CHECK_DAY5, m_bDayFlag[4]);
	DDX_Check(pDX, IDC_CHECK_DAY6, m_bDayFlag[5]);
	DDX_Check(pDX, IDC_CHECK_DAY7, m_bDayFlag[6]);
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

BEGIN_MESSAGE_MAP(CRefreshDaysDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_5DAYS, OnButton5days)
	ON_BN_CLICKED(IDC_BUTTON_7DAYS, OnButton7days)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CRefreshDaysDlg::CSSAutoShutdownPostInitDialog()
{
	if ( FALSE == m_bGroupsAllowed )
	{
		for (int n = 0; n < 7; n++)
		{
			m_editGroupDayNumber[n].ShowWindow(SW_HIDE);
		}

		SetWindowText ( "Refresh Days" );
	}

	return TRUE;
}

//$$******************************************************************

void CRefreshDaysDlg::OnButton5days()
{
	for (int n = 1; n <= 5; n++)
	{
		m_bDayFlag[n] = TRUE;
	}

	UpdateData(FALSE);
}

//$$******************************************************************

void CRefreshDaysDlg::OnButton7days()
{
	for (int n = 0; n <= 6; n++)
	{
		m_bDayFlag[n] = TRUE;
	}

	UpdateData(FALSE);
}

//$$******************************************************************

void CRefreshDaysDlg::OnButtonClear()
{
	for (int n = 0; n <= 6; n++)
	{
		m_bDayFlag[n] = FALSE;
	}

	UpdateData(FALSE);
}

//$$******************************************************************

bool CRefreshDaysDlg::HaveDaysSet()
{
	for (int n = 0; n <= 6; n++)
	{
		if (m_bDayFlag[n] == TRUE)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//$$******************************************************************

