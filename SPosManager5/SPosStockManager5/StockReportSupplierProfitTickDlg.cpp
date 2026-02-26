/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "StockReportSupplierProfitTickDlg.h"
/**********************************************************************/

CStockReportSupplierProfitTickDlg::CStockReportSupplierProfitTickDlg( CWnd* pParent)
	: CDialog(CStockReportSupplierProfitTickDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportSupplierProfitTickDlg)
	//}}AFX_DATA_INIT
	m_bAlphaSort = FALSE;
}

/**********************************************************************/

void CStockReportSupplierProfitTickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportSupplierProfitTickDlg)
	DDX_Check(pDX, IDC_CHECK_ALPHASORT, m_bAlphaSort);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportSupplierProfitTickDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportSupplierProfitTickDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportSupplierProfitTickDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

void CStockReportSupplierProfitTickDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		EndDialog ( IDOK );
}

/**********************************************************************/


