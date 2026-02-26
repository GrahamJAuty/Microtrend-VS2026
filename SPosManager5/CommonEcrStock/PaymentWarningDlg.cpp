/**********************************************************************/
#include "PaymentWarningDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPaymentWarningDlg, CDialog)
/**********************************************************************/

CPaymentWarningDlg::CPaymentWarningDlg(CWnd* pParent)
	: CDialog(CPaymentWarningDlg::IDD, pParent)
{
}

/**********************************************************************/

CPaymentWarningDlg::~CPaymentWarningDlg()
{
}

/**********************************************************************/

void CPaymentWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPaymentWarningDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/
