/**********************************************************************/
#include "SmartPay4Manager.h"
/**********************************************************************/
#include "PasswordDlg.h"
/**********************************************************************/

CPasswordDlg::CPasswordDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CPasswordDlg::IDD, pParent)
	, m_strPassword(_T(""))
{
}

/**********************************************************************/

CPasswordDlg::~CPasswordDlg()
{
}

/**********************************************************************/

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPasswordDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
/**********************************************************************/
