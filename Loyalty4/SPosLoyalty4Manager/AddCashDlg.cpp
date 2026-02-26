//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "AddCashDlg.h"
//$$******************************************************************

CAddCashDlg::CAddCashDlg(bool bSpend, bool bPurse2, CWnd* pParent) : CSSDialog(CAddCashDlg::IDD, pParent), m_Comments(Filenames.GetAuditCommentsFilename())
{
	m_strComment = "";
	m_dValue = 0.0;
	m_dPurseBalance = 0.0;
	m_bSpend = bSpend;
	m_bPurse2 = bPurse2;
}

//$$******************************************************************

void CAddCashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PURSE, m_dValue);
	DDX_Text(pDX, IDC_EDIT_BALANCE, m_dPurseBalance);
	DDX_Control(pDX, IDC_STATIC_ACTION, m_staticAction);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComments);
	DDX_CBString(pDX, IDC_COMBO_COMMENT, m_strComment);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAddCashDlg, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickedStaticComment)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAddCashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (TRUE == m_bSpend)
	{
		SetWindowText("Purse1 Manual Spend");
		m_staticAction.SetWindowText("Enter spend value");
	}
	else
	{
		m_staticAction.SetWindowText("Enter topup value");

		if (TRUE == m_bPurse2)
		{
			SetWindowText("Topup Purse2");
		}
		else
		{
			SetWindowText("Topup Purse1");
		}
	}

	SubclassEdit(IDC_EDIT_PURSE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_BALANCE, SS_NUM_SDP, 11, "%.2f");

	AddComments();

	return TRUE;
}

//$$******************************************************************

void CAddCashDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//$$******************************************************************

void CAddCashDlg::OnClickedStaticComment()
{
	if (UpdateData() == TRUE)
	{
		if (m_strComment != "")
		{
			m_Comments.Delete(m_strComment);			// delete comment
			m_comboComments.ResetContent();				// new list
			AddComments();

			m_strComment = "";
			UpdateData(FALSE);
		}
	}
}

//$$******************************************************************

void CAddCashDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );				// save comment if required
		CDialog::OnOK();
	}
}

//$$******************************************************************

