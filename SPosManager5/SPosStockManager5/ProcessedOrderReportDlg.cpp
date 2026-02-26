/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderReportDlg.h"
/**********************************************************************/

CProcessedOrderReportDlg::CProcessedOrderReportDlg( CWnd* pParent)
	: CDialog(CProcessedOrderReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessedOrderReportDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CProcessedOrderReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderReportDlg)
	DDX_Check(pDX, IDC_CHECK_CONSOLIDATE, m_bConsolidate);
	DDX_Check(pDX, IDC_CHECK_SORT, m_bSort);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderReportDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderReportDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/
