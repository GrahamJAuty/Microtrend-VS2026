/******************************************************************************/
#include "SmartPay4Manager.h"
/******************************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/******************************************************************************/
#include "GroupNoDlg.h"
/******************************************************************************/

CGroupNoDlg::CGroupNoDlg(int nGroupNo, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CGroupNoDlg::IDD, pParent)
{
	m_nGroupNo = nGroupNo;
}

/******************************************************************************/

CGroupNoDlg::~CGroupNoDlg()
{
}

/******************************************************************************/

void CGroupNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/******************************************************************************/

BEGIN_MESSAGE_MAP(CGroupNoDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

/******************************************************************************/

BOOL CGroupNoDlg::CSSAutoShutdownPostInitDialog()
{
	SubclassEdit(IDC_EDIT_GROUPNO, SS_NUM, 2, "%d");
	SetEditBoxInt(*GetEditGroupNo(), m_nGroupNo);
	return TRUE;
}

/******************************************************************************/

void CGroupNoDlg::OnOK()
{
	m_nGroupNo = GetEditBoxInt(*GetEditGroupNo());
	if ((m_nGroupNo < 1) || (m_nGroupNo > 99))
	{
		Prompter.Error("Please enter a group number between 1 and 99");
		GetEditGroupNo()->SetSel(0, -1);
		GetEditGroupNo()->SetFocus();
		return;
	}

	EndDialog(IDOK);
}

/******************************************************************************/
