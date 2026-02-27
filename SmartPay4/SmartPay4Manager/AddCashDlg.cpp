//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "AddCashDlg.h"
//$$******************************************************************

CAddCashDlg::CAddCashDlg( int nPurse, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CAddCashDlg::IDD, pParent)
	, m_Comments(Filenames.GetAuditCommentsFilename())
{
	m_nPurse = nPurse;
	m_dValue = 0.0;
	m_dPurseBalance = 0.0;
	m_bPebbleAdjust = FALSE;
	m_dPendingPebbleAdjust = 0.0;
}

//$$******************************************************************

CAddCashDlg::~CAddCashDlg()
{
}

//$$******************************************************************

void CAddCashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PURSE1, m_dValue);
	DDX_Text(pDX, IDC_EDIT_BALANCE, m_dPurseBalance);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_CBString(pDX, IDC_COMBO_COMMENTS, m_strComment);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAddCashDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAddCashDlg::CSSAutoShutdownPostInitDialog()
{
	CString strTitle = "";

	if (FALSE == m_bPebbleAdjust)
	{
		strTitle.Format("Topup Purse %d", m_nPurse);
	}
	else
	{
		strTitle.Format("Pebble Adjust");

		if (::CompareDoubles(m_dPendingPebbleAdjust, 0.0, 2) != 0)
		{
			strTitle.AppendFormat(" ( %.2f Pending )", m_dPendingPebbleAdjust);
		}

		m_nPurse = 1;
	}

	SetWindowText(strTitle);

	SubclassEdit(IDC_EDIT_PURSE1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_BALANCE, SS_NUM_SDP, 11, "%.2f");

	AddComments();

	return TRUE;
}

//$$******************************************************************

void CAddCashDlg::AddComments()
{
	if (TRUE == m_bPebbleAdjust)
	{
		m_comboComments.AddString("Pebble Adjust");
		m_comboComments.SetCurSel(0);
		m_comboComments.EnableWindow(FALSE);
	}

	for (int i = 0; i < m_Comments.GetSize(); i++)
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//$$******************************************************************

void CAddCashDlg::OnClickStaticComment()
{
	if (UpdateData() == TRUE)
	{
		if (m_strComment != "")
		{
			m_Comments.Delete(m_strComment);
			m_comboComments.ResetContent();
			AddComments();

			m_strComment = "";
			UpdateData(FALSE);
		}
	}
}

//$$******************************************************************

void CAddCashDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_Comments.Save(m_strComment);
		EndDialog(IDOK);
	}
}

//$$******************************************************************
