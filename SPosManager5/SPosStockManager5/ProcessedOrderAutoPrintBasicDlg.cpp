/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderAutoPrintBasicDlg.h"
/**********************************************************************/

CProcessedOrderAutoPrintBasicDlg::CProcessedOrderAutoPrintBasicDlg( CWnd* pParent )
	: CDialog(CProcessedOrderAutoPrintBasicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessedOrderAutoPrintBasicDlg)
	m_bPurchase = TRUE;
	m_bReports = TRUE;
	m_bEmail = FALSE;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CProcessedOrderAutoPrintBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderAutoPrintBasicDlg)
	DDX_Check(pDX, IDC_CHECK_PURCHASE, m_bPurchase);
	DDX_Check(pDX, IDC_CHECK_REPORTS, m_bReports);
	DDX_Check(pDX, IDC_CHECK_EMAIL, m_bEmail);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderAutoPrintBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderAutoPrintBasicDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderAutoPrintBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/
