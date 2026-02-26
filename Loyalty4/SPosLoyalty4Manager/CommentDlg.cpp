//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "CommentDlg.h"
//$$******************************************************************

CCommentDlg::CCommentDlg(const char* szCaption, CWnd* pParent )
	: CSSDialog(CCommentDlg::IDD, pParent)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
{
	m_strComment = "";
	m_strMessage = "";
	m_strCaption = szCaption;
}

//$$******************************************************************

void CCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComments);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CCommentDlg, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CCommentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_staticMessage.SetWindowText(m_strMessage);

	SetWindowText( m_strCaption );
	m_buttonOK.SetWindowText( "Confirm" );
	m_buttonCancel.SetWindowText( "No" );

	AddComments();

	return TRUE;
}

//$$******************************************************************

void CCommentDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//$$******************************************************************

void CCommentDlg::OnClickStaticComment()
{
	m_strComment = GetComboBoxText(m_comboComments);

	if (m_strComment != "")
	{
		m_Comments.Delete(m_strComment);			// delete comment
		m_comboComments.ResetContent();				// new list
		AddComments();

		m_comboComments.SetWindowText("");
	}
}

//$$******************************************************************

void CCommentDlg::OnOK()
{
	m_strComment = GetComboBoxText(m_comboComments);
	m_Comments.Save(m_strComment);				// save comment if required
	EndDialog(IDOK);
}

//$$******************************************************************
