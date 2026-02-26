//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "SetupAlertTextsDlg.h"
//$$******************************************************************

CSetupAlertTextsDlg::CSetupAlertTextsDlg(CWnd* pParent )
	: CSSDialog(CSetupAlertTextsDlg::IDD, pParent)
{
	m_alertText.Open ( Filenames.GetAlertTextFilename(), DB_READWRITE );
}

//$$******************************************************************

CSetupAlertTextsDlg::~CSetupAlertTextsDlg()
{
}

//$$******************************************************************

void CSetupAlertTextsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT1, m_editText[0]);
	DDX_Control(pDX, IDC_EDIT_TEXT2, m_editText[1]);
	DDX_Control(pDX, IDC_EDIT_TEXT3, m_editText[2]);
	DDX_Control(pDX, IDC_EDIT_TEXT4, m_editText[3]);
	DDX_Control(pDX, IDC_EDIT_TEXT5, m_editText[4]);
	DDX_Control(pDX, IDC_EDIT_TEXT6, m_editText[5]);
	DDX_Control(pDX, IDC_EDIT_TEXT7, m_editText[6]);
	DDX_Control(pDX, IDC_EDIT_TEXT8, m_editText[7]);
	DDX_Control(pDX, IDC_EDIT_TEXT9, m_editText[8]);
	DDX_Control(pDX, IDC_CHECK_EPOS1, m_checkEPOS[0]);
	DDX_Control(pDX, IDC_CHECK_EPOS2, m_checkEPOS[1]);
	DDX_Control(pDX, IDC_CHECK_EPOS3, m_checkEPOS[2]);
	DDX_Control(pDX, IDC_CHECK_EPOS4, m_checkEPOS[3]);
	DDX_Control(pDX, IDC_CHECK_EPOS5, m_checkEPOS[4]);
	DDX_Control(pDX, IDC_CHECK_EPOS6, m_checkEPOS[5]);
	DDX_Control(pDX, IDC_CHECK_EPOS7, m_checkEPOS[6]);
	DDX_Control(pDX, IDC_CHECK_EPOS8, m_checkEPOS[7]);
	DDX_Control(pDX, IDC_CHECK_EPOS9, m_checkEPOS[8]);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupAlertTextsDlg, CSSDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSetupAlertTextsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int n = 1; n <= 9; n++)
	{
		CAlertTextInfo info{};
		m_alertText.GetInfo(n, info);
		m_editText[n-1].SetWindowText(info.m_strText);
		m_checkEPOS[n - 1].SetCheck(info.m_bEPOS);
	}

	return TRUE;
}

//$$******************************************************************

void CSetupAlertTextsDlg::OnBnClickedButtonClear()
{
	for (int n = 0; n < 9; n++)
	{
		m_editText[n].SetWindowText("");
	}
}

//$$******************************************************************

void CSetupAlertTextsDlg::OnOK()
{
	for (int n = 1; n <= 9; n++)
	{
		CAlertTextInfo info{};
		info.m_strText = GetEditBoxText(m_editText[n - 1]);
		info.m_bEPOS = IsTicked(m_checkEPOS[n - 1]);
		m_alertText.SetInfo(n, info);
	}

	EndDialog(IDOK);
}

//$$******************************************************************

