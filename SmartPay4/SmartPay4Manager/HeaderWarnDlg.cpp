//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ListDataHeaderChangeDlg.h"
//$$******************************************************************
#include "HeaderWarnDlg.h"
//$$******************************************************************

CHeaderWarnDlg::CHeaderWarnDlg( CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CHeaderWarnDlg::IDD, pParent)
{
	m_strTitle = "";
	m_strInfo1Text = "";
	m_strInfo2Text = "";
	m_strHeader1Description = "";
	m_strHeader2Description = "";
	m_strHeader1 = "";
	m_strHeader2 = "";
	bool m_bAllowCancel = TRUE;
}

//$$******************************************************************

CHeaderWarnDlg::~CHeaderWarnDlg()
{
}

//$$******************************************************************

void CHeaderWarnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CHeaderWarnDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW,OnButtonView)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CHeaderWarnDlg::CSSAutoShutdownPostInitDialog()
{
	if (m_strTitle != "")
	{
		SetWindowText(m_strTitle);
	}

	if (m_strInfo1Text != "")
	{
		m_staticInfo1.SetWindowText(m_strInfo1Text);
	}

	if (m_strInfo2Text != "")
	{
		m_staticInfo2.SetWindowText(m_strInfo2Text);
	}

	if (FALSE == m_bAllowCancel)
	{
		ShowAndEnableWindow(&m_buttonCancel, FALSE);
		MoveControl(&m_buttonOK, 256, 119);
	}

	return TRUE;
}

//$$******************************************************************

void CHeaderWarnDlg::OnButtonView()
{
	CListDataHeaderChangeDlg dlg( "Compare CSV Headers", m_strHeader1, m_strHeader2);
	dlg.SetHeader1Description(m_strHeader1Description);
	dlg.SetHeader2Description(m_strHeader2Description);
	dlg.DoModal();
}

//$$******************************************************************


