/**********************************************************************/
 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "StockReportSalesAuditDlg.h"
/**********************************************************************/

CStockReportSalesAuditDlg::CStockReportSalesAuditDlg(bool bExceptions, CWnd* pParent )
	: CDialog(CStockReportSalesAuditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportSalesAuditDlg)
	//}}AFX_DATA_INIT
	m_bE1Sales = TRUE;
	m_bImportSales = TRUE;
	m_bManualSales = ( ( FALSE == bExceptions ) || ( SysInfo.IsPMSSystem() == TRUE ) );
	m_bExceptions = bExceptions;
}

/**********************************************************************/

void CStockReportSalesAuditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportSalesAuditDlg)
	DDX_Control(pDX, IDC_CHECK_MANUALSALES, m_checkManualSales);
	DDX_Check(pDX, IDC_CHECK_E1SALES, m_bE1Sales);
	DDX_Check(pDX, IDC_CHECK_IMPORTSALES, m_bImportSales);
	DDX_Check(pDX, IDC_CHECK_MANUALSALES, m_bManualSales);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportSalesAuditDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportSalesAuditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportSalesAuditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( m_bExceptions == TRUE )
	{
		if ( SysInfo.IsPMSSystem() == FALSE )
			m_checkManualSales.ShowWindow ( SW_HIDE );
		else
			m_checkManualSales.SetWindowText( "PMS manual sales" );

		SetWindowText ( "Audit Sales Exceptions" );
	}
	else
		SetWindowText ( "Audit Sales" );

	return TRUE; 
}

/**********************************************************************/

void CStockReportSalesAuditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_bImportSales == TRUE || m_bManualSales == TRUE || m_bE1Sales == TRUE )
			EndDialog ( IDOK );
		else
			EndDialog ( IDCANCEL );
	}
}

/**********************************************************************/
