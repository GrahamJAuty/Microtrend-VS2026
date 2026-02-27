// AddPointsDlg.cpp : implementation file
//

#include "SmartPay4Manager.h"
//#include "FilenameHandler.h"
#include "AddPointsDlg.h"

// CAddPointsDlg dialog

CAddPointsDlg::CAddPointsDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CAddPointsDlg::IDD, pParent)
	, m_Comments(Filenames.GetAuditCommentsFilename())
	, m_strComment(_T(""))
	, m_nValue(0)
	, m_nPointsBalance(0)
{

}

CAddPointsDlg::~CAddPointsDlg()
{
}

void CAddPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINTS, m_nValue);
	DDX_Text(pDX, IDC_EDIT_POINTSBALANCE, m_nPointsBalance);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_CBString(pDX, IDC_COMBO_COMMENTS, m_strComment);
}


BEGIN_MESSAGE_MAP(CAddPointsDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnStnClickedStaticComment)
END_MESSAGE_MAP()


// CAddPointsDlg message handlers

BOOL CAddPointsDlg::CSSAutoShutdownPostInitDialog()
{
	SubclassEdit ( IDC_EDIT_POINTS,			SS_NUM_S,	 8, "%ld" );
	SubclassEdit ( IDC_EDIT_POINTSBALANCE,	SS_NUM_S,	 8, "%ld" );

	AddComments();

	return TRUE;
}

void CAddPointsDlg::AddComments()
{
	for ( int i = 0 ; i < m_Comments.GetSize() ; i++ )		// read previous comments
		m_comboComments.AddString ( m_Comments.GetAt(i) );
}

//*******************************************************************

void CAddPointsDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );				// save comment if required
		EndDialog(IDOK);
	}
}

//*******************************************************************

void CAddPointsDlg::OnStnClickedStaticComment()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_strComment != "" )
		{
			m_Comments.Delete ( m_strComment );			// delete comment
			m_comboComments.ResetContent();				// new list
			AddComments();

			m_strComment = "";
			UpdateData(FALSE);
		}
	}
}

//*******************************************************************
