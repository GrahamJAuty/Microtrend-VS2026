/**********************************************************************/
#include "LoyaltyManager.h"
/**********************************************************************/
#include "GroupNoDlg.h"
/**********************************************************************/

CGroupNoDlg::CGroupNoDlg( bool bScheme, int nGroupNo, CWnd* pParent)
	: CDialog(CGroupNoDlg::IDD, pParent)
{
	m_bScheme = bScheme;
	m_nGroupNo = nGroupNo;
}

/**********************************************************************/

void CGroupNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_staticPrompt);	
	DDX_Text(pDX, IDC_EDIT_GROUPNO, m_nGroupNo);
	DDV_MinMaxInt(pDX, m_nGroupNo, 1, 99);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CGroupNoDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CGroupNoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if (FALSE == m_bScheme)
	{
		SetWindowText("Add Group");
		m_staticPrompt.SetWindowText("Enter Group Number");
	}
	else
	{
		SetWindowText("Add Scheme Name");
		m_staticPrompt.SetWindowText("Enter Scheme Number");
	}

	return TRUE;
}

/**********************************************************************/
