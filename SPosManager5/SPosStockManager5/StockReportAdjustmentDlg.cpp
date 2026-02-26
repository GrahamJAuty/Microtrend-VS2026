/**********************************************************************/
 
/**********************************************************************/
 
#include "StockOptionsIni.h"
#include "StockReportNames.h"
#include "StockTreeReportNew.h"
/**********************************************************************/
#include "StockReportAdjustmentDlg.h"
/**********************************************************************/

CStockReportAdjustmentDlg::CStockReportAdjustmentDlg( int nTimeType, CWnd* pParent)
	: CDialog(CStockReportAdjustmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportAdjustmentDlg)
	//}}AFX_DATA_INIT
	m_bBreakdown = TRUE;
	m_bAdjust = TRUE;
	m_bSales = TRUE;
	m_bWasteAuto = TRUE;
	m_bWasteEcr = TRUE;
	m_nTimeType = nTimeType;
}

/**********************************************************************/

void CStockReportAdjustmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportAdjustmentDlg)
	DDX_Control(pDX, IDC_CHECK_WASTE_AUTO, m_checkWasteAuto);
	DDX_Check(pDX, IDC_CHECK_ADJUST, m_bAdjust);
	DDX_Check(pDX, IDC_CHECK_SALES, m_bSales);
	DDX_Check(pDX, IDC_CHECK_WASTE_AUTO, m_bWasteAuto);
	DDX_Check(pDX, IDC_CHECK_WASTE_ECR, m_bWasteEcr);
	DDX_Check(pDX, IDC_CHECK_BREAKDOWN, m_bBreakdown);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportAdjustmentDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportAdjustmentDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportAdjustmentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch ( m_nTimeType )
	{
	case REPORT_YTD:
		SetWindowText ( "YTD Adjustment Summary" );
		break;

	case REPORT_YTD_PLUS_PERIOD:
		SetWindowText ( "YTD + Period Adjustment Summary" );
		break;

	case REPORT_PERIOD:
	default:
		SetWindowText ( "Period Adjustment Summary" );
		break;
	}

	if ( StockOptions.GetStockSubUnitsFlag() == FALSE )
		m_checkWasteAuto.ShowWindow( SW_HIDE );

	return TRUE;  
}

/**********************************************************************/

void CStockReportAdjustmentDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( StockOptions.GetStockSubUnitsFlag() == FALSE )
			m_bWasteAuto = FALSE;

		if ( ( m_bWasteAuto == FALSE ) && ( m_bWasteEcr == FALSE ) && ( m_bAdjust == FALSE ) )
		{
			Prompter.Error ( "You must select at least one type of adjustment or wastage\nbefore you can create this report." );
			return;
		}

		GetParent() -> PostMessage ( WM_APP, STOCK_ADJUST, (long) this );
	}
}

/**********************************************************************/
