/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockReportVarianceDlg.h"
/**********************************************************************/

CStockReportVarianceDlg::CStockReportVarianceDlg(CWnd* pParent)
	: CDialog(CStockReportVarianceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportVarianceDlg)
	m_bZeroSkip = FALSE;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CStockReportVarianceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportVarianceDlg)
	DDX_Check(pDX, IDC_CHECK_ZEROSKIP, m_bZeroSkip);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportVarianceDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportVarianceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportVarianceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

void CStockReportVarianceDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		GetParent() -> PostMessage ( WM_APP, STOCK_VARIANCE, (long) this );
}

/**********************************************************************/
