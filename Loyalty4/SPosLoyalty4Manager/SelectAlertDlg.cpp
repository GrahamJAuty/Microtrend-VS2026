//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "SelectAlertDlg.h"
//$$******************************************************************

CSelectAlertDlg::CSelectAlertDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CSelectAlertDlg::IDD, pParent)
{
	m_alertText.Open(Filenames.GetAlertTextFilename());
	m_strAlertCodes = "";
}

//$$******************************************************************

CSelectAlertDlg::~CSelectAlertDlg()
{
}

//$$******************************************************************

void CSelectAlertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ALERT1, m_checkAlert[0]);
	DDX_Control(pDX, IDC_CHECK_ALERT2, m_checkAlert[1]);
	DDX_Control(pDX, IDC_CHECK_ALERT3, m_checkAlert[2]);
	DDX_Control(pDX, IDC_CHECK_ALERT4, m_checkAlert[3]);
	DDX_Control(pDX, IDC_CHECK_ALERT5, m_checkAlert[4]);
	DDX_Control(pDX, IDC_CHECK_ALERT6, m_checkAlert[5]);
	DDX_Control(pDX, IDC_CHECK_ALERT7, m_checkAlert[6]);
	DDX_Control(pDX, IDC_CHECK_ALERT8, m_checkAlert[7]);
	DDX_Control(pDX, IDC_CHECK_ALERT9, m_checkAlert[8]);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CSelectAlertDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NONE,OnButtonNone)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CSelectAlertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int n = 0; n < 9; n++)
	{
		CAlertTextInfo info;
		m_alertText.GetInfo(n + 1, info);

		if (info.m_strText == "")
		{
			m_checkAlert[n].EnableWindow(FALSE);
		}
		else
		{
			m_checkAlert[n].SetWindowText(info.m_strText);
		}

		m_checkAlert[n].SetCheck(FALSE);
	}

	CCSV csv(m_strAlertCodes, ':');

	for (int n = 0; (n < csv.GetSize()) && (n < 9); n++)
	{
		int a = csv.GetInt(n);

		if ((a >= 1) && (a <= 9) && (m_checkAlert[a - 1].IsWindowEnabled()))
		{
			m_checkAlert[a - 1].SetCheck(TRUE);
		}
	}

	return TRUE;
}

//$$******************************************************************

void CSelectAlertDlg::OnButtonNone()
{
	for (int n = 0; n < 9; n++)
	{
		m_checkAlert[n].SetCheck(FALSE);
	}
}

//$$******************************************************************

void CSelectAlertDlg::OnOK()
{
	CCSV csv(':');

	for (int n = 0; n < 9; n++)
	{
		if (IsTicked(m_checkAlert[n]))
		{
			csv.Add(n + 1);
		}
	}

	m_strAlertCodes = csv.GetLine();
	EndDialog(IDOK);
}

//$$******************************************************************
