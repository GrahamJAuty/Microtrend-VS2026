/**********************************************************************/
 
/**********************************************************************/
#include "StockReportExceptionsDlg.h"
/**********************************************************************/

CStockReportExceptionsDlg::CStockReportExceptionsDlg( CStockAuditExceptionTypes& Types, CWnd* pParent)
	: CDialog(CStockReportExceptionsDlg::IDD, pParent), m_Types ( Types )
{
	//{{AFX_DATA_INIT(CStockReportExceptionsDlg)
	//}}AFX_DATA_INIT
	m_bHeaders = m_Types.m_bShowHeaders;
	m_bOpening = m_Types.m_bOpening;
	m_bDelivery = m_Types.m_bDelivery;
	m_bReturns = m_Types.m_bReturns;
	m_bSales = m_Types.m_bSales;
	m_bAdjustments = m_Types.m_bAdjustment;
	m_bTransfers = m_Types.m_bTransfer;
	m_bStocktaking = m_Types.m_bStocktaking;
}

/**********************************************************************/

void CStockReportExceptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportExceptionsDlg)
	DDX_Check(pDX, IDC_CHECK_ADJUSTMENTS, m_bAdjustments);
	DDX_Check(pDX, IDC_CHECK_DELIVERY, m_bDelivery);
	DDX_Check(pDX, IDC_CHECK_OPENING, m_bOpening);
	DDX_Check(pDX, IDC_CHECK_RETURNS, m_bReturns);
	DDX_Check(pDX, IDC_CHECK_STOCKTAKING, m_bStocktaking);
	DDX_Check(pDX, IDC_CHECK_TRANSFERS, m_bTransfers);
	DDX_Check(pDX, IDC_CHECK_SALES, m_bSales);
	DDX_Check(pDX, IDC_CHECK_HEADERS, m_bHeaders);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportExceptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportExceptionsDlg)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CStockReportExceptionsDlg::OnButtonAll() 
{
	UpdateData( TRUE );
	m_bOpening = TRUE;
	m_bDelivery = TRUE;
	m_bReturns = TRUE;
	m_bSales = TRUE;
	m_bAdjustments = TRUE;
	m_bTransfers = TRUE;
	m_bStocktaking = TRUE;
	UpdateData( FALSE );
}

/**********************************************************************/

void CStockReportExceptionsDlg::OnButtonNone() 
{
	UpdateData( TRUE );
	m_bOpening = FALSE;
	m_bDelivery = FALSE;
	m_bReturns = FALSE;
	m_bSales = FALSE;
	m_bAdjustments = FALSE;
	m_bTransfers = FALSE;
	m_bStocktaking = FALSE;
	UpdateData( FALSE );
}

/**********************************************************************/

void CStockReportExceptionsDlg::OnOK() 
{
	UpdateData ( TRUE );
	m_Types.m_bShowHeaders = ( m_bHeaders != 0 );
	m_Types.m_bOpening = ( m_bOpening != 0 );
	m_Types.m_bDelivery = ( m_bDelivery != 0 );
	m_Types.m_bReturns = ( m_bReturns != 0 );
	m_Types.m_bSales = ( m_bSales != 0 );
	m_Types.m_bAdjustment = ( m_bAdjustments != 0 );
	m_Types.m_bTransfer = ( m_bTransfers != 0 );
	m_Types.m_bStocktaking = ( m_bStocktaking != 0 );
	EndDialog ( IDOK );
}

/**********************************************************************/

