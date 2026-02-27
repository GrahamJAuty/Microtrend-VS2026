/*********************************************************************/
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
/*********************************************************************/
#include "HotlistDlg.h"
/*********************************************************************/

CHotlistDlg::CHotlistDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CHotlistDlg::IDD, pParent)
	, m_RowAccount(RowAccount)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
{
	m_strComment = "";
	m_nReasonIndex = 0;
}

/*********************************************************************/

void CHotlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REASON, m_comboReason);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_Control(pDX, IDC_STATIC_MEMBERID, m_staticMemberID);
	DDX_Control(pDX, IDC_EDIT_MEMBERID, m_editMemberID);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_EDIT_CARDNAME, m_editSurname);
	DDX_Control(pDX, IDC_EDIT_FORENAME, m_editForename);
	DDX_Control(pDX, IDC_EDIT_GROUPNO, m_editGroupName);
	DDX_Control(pDX, IDC_EDIT_PURSE1DATELASTUSED, m_editPurse1Date);
	DDX_Control(pDX, IDC_EDIT_PURSE2DATELASTUSED, m_editPurse2Date);
	DDX_Control(pDX, IDC_EDIT_PURSE3DATELASTUSED, m_editPurse3Date);
	DDX_CBIndex(pDX, IDC_COMBO_REASON, m_nReasonIndex);
	DDX_CBString(pDX, IDC_COMBO_COMMENTS, m_strComment);
}

/*********************************************************************/

BEGIN_MESSAGE_MAP(CHotlistDlg, CSSAutoShutdownDialog)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

/*********************************************************************/

BOOL CHotlistDlg::CSSAutoShutdownPostInitDialog()
{

	m_editCardNo.SetWindowText(System.FormatCardNo(m_RowAccount.GetUserIDString()));
	m_editSurname.SetWindowText(m_RowAccount.GetSurname());
	m_editForename.SetWindowText(m_RowAccount.GetForename());
	m_editPurse1Date.SetWindowText(m_RowAccount.GetPurse1LastSpendDate());
	m_editPurse2Date.SetWindowText(m_RowAccount.GetPurse2LastSpendDate());
	m_editPurse3Date.SetWindowText(m_RowAccount.GetPurse3LastSpendDate());

	CAlertText alert;
	alert.Open ( Filenames.GetAlertFilename() );	// readonly

	for (int nIndex = 0; nIndex < ALERT_MAX; ++nIndex)
	{
		m_comboReason.AddString(alert.GetHotlistText(nIndex + 1));
	}

	m_comboReason.SetCurSel(0);

	CSQLRepositoryUserText RepoText;
	CString strText = RepoText.GetMemberIDText();
	if ( strText == "" )
	{
		m_staticMemberID.ShowWindow(SW_HIDE);
		m_editMemberID.ShowWindow(SW_HIDE);
	}
	else
	{
		m_staticMemberID.SetWindowText(strText);
		m_editMemberID.SetWindowText(m_RowAccount.GetMemberID());
	}

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(m_RowAccount.GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_editGroupName.SetWindowText(RowGroup.GetListboxText());
	}
	else
	{
		CString strGroupName = "";
		strGroupName.Format("%d  :  Unknown", m_RowAccount.GetGroupNo());
		m_editGroupName.SetWindowText(strGroupName);
	}

	AddComments();

	return TRUE;
}

/*********************************************************************/

void CHotlistDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

/*********************************************************************/

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
		EndDialog(IDOK);
	}
}

/*********************************************************************/


