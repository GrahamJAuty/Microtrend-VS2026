/**********************************************************************/
#include "CommentDlg.h"
/**********************************************************************/

CCommentDlg::CCommentDlg(const char* szCaption,CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CCommentDlg::IDD, pParent)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
{
	m_strCaption = szCaption;
	m_strComment = "";
	m_strMessage = "";
}

/**********************************************************************/

CCommentDlg::~CCommentDlg()
{
}

/**********************************************************************/

void CCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_CBString(pDX, IDC_COMBO_COMMENTS, m_strComment);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_strMessage);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCommentDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCommentDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_strCaption );

	m_buttonOK.SetWindowText("Confirm" );
	m_buttonCancel.SetWindowText("No" );

	AddComments();

	return TRUE;
}

/**********************************************************************/

void CCommentDlg::AddComments()
{
	// read previous comments
	for (int i = 0; i < m_Comments.GetSize(); i++)	
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

/**********************************************************************/

void CCommentDlg::OnClickStaticComment()
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

//*******************************************************************

void CCommentDlg::OnOK()
{
	// save comment if required
	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );			
		EndDialog(IDOK);
	}
}

//*******************************************************************

