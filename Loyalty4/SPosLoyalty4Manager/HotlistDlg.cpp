//*******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
//*******************************************************************
#include "HotlistDlg.h"
//*******************************************************************

CHotlistDlg::CHotlistDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent )
	: CDialog(CHotlistDlg::IDD, pParent)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
	, m_RowAccount(RowAccount)
{
	m_strComment = "";
	m_strCardName = m_RowAccount.GetFullname();

	CSSDate date{};
	CSSTime time{};
	m_RowAccount.GetLastUsed ( date, time );
	m_strDateLastUsed = date.GetDate();	
	m_strTimeLastUsed = time.GetTime();

	m_strDateLastRefreshed	= m_RowAccount.GetPurse2RefreshedDate();
	
	if (m_strDateLastRefreshed.GetLength() > 0)
	{
		m_strTimeLastRefreshed = m_RowAccount.GetPurse2RefreshedTime();
	}
	else
	{
		m_strTimeLastRefreshed = "";
	}
}

//*******************************************************************

CHotlistDlg::~CHotlistDlg()
{
}

//*******************************************************************

void CHotlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CARDNAME, m_strCardName);
	DDX_Text(pDX, IDC_EDIT_PURSE1DATELASTUSED, m_strDateLastUsed);
	DDX_Text(pDX, IDC_EDIT_PURSE1TIMELASTUSED, m_strTimeLastUsed);
	DDX_Text(pDX, IDC_EDIT_DATELASTREFRESHED, m_strDateLastRefreshed);
	DDX_Text(pDX, IDC_EDIT_TIMELASTREFRESHED, m_strTimeLastRefreshed);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComments);
	DDX_CBString(pDX, IDC_COMBO_COMMENT, m_strComment);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CHotlistDlg, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//*******************************************************************
// CHotlistDlg message handlers

BOOL CHotlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strCaption = "";
	strCaption.Format ( "Hotlist Account %s",
		(const char*) System.FormatCardNo(m_RowAccount.GetUserIDString() ));

	SetWindowText ( strCaption );

	CGroupComboHelpers::FillGroupCombo(m_comboGroup, m_RowAccount.GetGroupNo(), FALSE);
	m_comboGroup.EnableWindow(FALSE);

	AddComments();
	
	return TRUE;
}

//*******************************************************************

void CHotlistDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//*******************************************************************

void CHotlistDlg::OnClickStaticComment()
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

void CHotlistDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );				// save comment if required
		CDialog::OnOK();
	}
}

//*******************************************************************

