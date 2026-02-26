/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockReportApparentDlg.h"
/**********************************************************************/

CStockReportApparentDlg::CStockReportApparentDlg( int nType, CWnd* pParent)
	: CSSDialog(CStockReportApparentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportApparentDlg)
	//}}AFX_DATA_INIT
	m_bHideAboveMin = 0;
	m_bPositive = 0;
	m_bZero = 0;
	m_bNegative = 0;
	m_bZeroCost = 0;
	m_nType = nType;
}

/**********************************************************************/

void CStockReportApparentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportApparentDlg)
	DDX_Control(pDX, IDC_CHECK_HIDEABOVEMIN, m_checkHideAboveMin);
	DDX_Control(pDX, IDC_STATIC_POSITIVE, m_staticPositive);
	DDX_Control(pDX, IDC_CHECK_POSITIVE, m_checkPositive);
	DDX_Control(pDX, IDC_STATIC_ZERO, m_staticZero);
	DDX_Control(pDX, IDC_CHECK_ZERO, m_checkZero);
	DDX_Control(pDX, IDC_STATIC_NEGATIVE, m_staticNegative);
	DDX_Control(pDX, IDC_CHECK_NEGATIVE, m_checkNegative);
	DDX_Control(pDX, IDC_CHECK_ZEROCOST, m_checkZeroCost);
	DDX_Check(pDX, IDC_CHECK_HIDEABOVEMIN, m_bHideAboveMin);
	DDX_Check(pDX, IDC_CHECK_POSITIVE, m_bPositive);
	DDX_Check(pDX, IDC_CHECK_ZERO, m_bZero);
	DDX_Check(pDX, IDC_CHECK_NEGATIVE, m_bNegative);
	DDX_Check(pDX, IDC_CHECK_ZEROCOST, m_bZeroCost);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportApparentDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportApparentDlg)
	ON_BN_CLICKED(IDC_CHECK_HIDEABOVEMIN, OnToggleHideAboveMin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportApparentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	bool bNoHideAboveMinFlag = FALSE;
	bool bNoZeroCostFlag = FALSE;
	
	switch( m_nType )
	{
	case STOCK_APPARENT_QTY:
		SetWindowText( "Apparent Stock Report" );
		bNoZeroCostFlag = TRUE;
		break;

	case STOCK_APPARENT_QTY_DU:
		SetWindowText( "Apparent Stock Report by DU" );
		bNoZeroCostFlag = TRUE;
		break;

	case STOCK_APPARENT_SUMMARY:
		SetWindowText( "Apparent Stock Summary" );
		bNoZeroCostFlag = TRUE;
		break;

	case STOCK_APPARENT_SUMMARY_DU:
		SetWindowText( "Apparent Stock Summary by DU" );
		bNoZeroCostFlag = TRUE;
		break;

	case STOCK_APPARENT_VALUE:
		SetWindowText( "Apparent Stock Valuation Report" );
		break;

	case STOCK_OPENING_VALUE:
		SetWindowText( "Opening Stock Valuation Report" );
		bNoHideAboveMinFlag = TRUE;
		break;

	case STOCK_OVERSTOCK_VALUE:
		SetWindowText( "Overstock Valuation Report" );
		bNoHideAboveMinFlag = TRUE;
		break;

	case STOCK_CLOSING_VALUE:
		SetWindowText( "Closing Stock Valuation Report" );
		bNoHideAboveMinFlag = TRUE;
		break;

	case STOCK_TRANSFER_VALUE:
		SetWindowText( "Transfer Stock Valuation Report" );
		bNoHideAboveMinFlag = TRUE;
		break;
	}

	if ( TRUE == bNoHideAboveMinFlag )
	{
		ShowAndEnableWindow( &m_checkHideAboveMin, FALSE );
		MoveControl( &m_checkPositive, 7, 7 );
		MoveControl( &m_checkZero, 7, 23 );
		MoveControl( &m_checkNegative, 7, 39 );
		MoveControl( &m_checkZeroCost, 7, 55 );
	}
	else
	{
		OnToggleHideAboveMin();
	}

	if ( TRUE == bNoZeroCostFlag )
	{
		ShowAndEnableWindow( &m_checkZeroCost, FALSE );
	}

	switch( m_nType )
	{
	case STOCK_APPARENT_QTY:
	case STOCK_APPARENT_QTY_DU:
	case STOCK_APPARENT_SUMMARY:
	case STOCK_APPARENT_SUMMARY_DU:
	case STOCK_APPARENT_VALUE:
	case STOCK_OVERSTOCK_VALUE:
		m_staticPositive.SetWindowText( "Show items with positive apparent stock" );
		m_staticZero.SetWindowText( "Show items with zero apparent stock" );
		m_staticNegative.SetWindowText( "Show items with negative apparent stock" );
		break;

	case STOCK_OPENING_VALUE:
		m_staticPositive.SetWindowText( "Show items with positive opening stock" );
		m_staticZero.SetWindowText( "Show items with zero opening stock" );
		m_staticNegative.SetWindowText( "Show items with negative opening stock" );
		break;

	case STOCK_CLOSING_VALUE:
		m_staticPositive.SetWindowText( "Show items with positive closing stock" );
		m_staticZero.SetWindowText( "Show items with zero closing stock" );
		m_staticNegative.SetWindowText( "Show items with negative closing stock" );
		break;

	case STOCK_TRANSFER_VALUE:
		m_staticPositive.SetWindowText( "Show items with positive net transfers in" );
		m_staticZero.SetWindowText( "Show items with zero net transfers in" );
		m_staticNegative.SetWindowText( "Show items with negative net transfers in" );
		break;
	}

	return TRUE;  
}

/**********************************************************************/

void CStockReportApparentDlg::OnToggleHideAboveMin()
{
	if ( m_checkHideAboveMin.GetCheck() == TRUE )
	{
		m_checkPositive.SetCheck(TRUE);
		m_checkZero.SetCheck( TRUE );
		m_checkNegative.SetCheck(TRUE);
		m_checkPositive.EnableWindow(FALSE);
		m_checkZero.EnableWindow(FALSE);
		m_checkNegative.EnableWindow(FALSE);
	}
	else
	{
		m_checkPositive.EnableWindow(TRUE);
		m_checkZero.EnableWindow(TRUE);
		m_checkNegative.EnableWindow(TRUE);
	}
}

/**********************************************************************/

void CStockReportApparentDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( ( m_bPositive == 0 ) && ( m_bZero == 0 ) && ( m_bNegative == 0 ) )
			Prompter.Error( "You must select at least one type of stock level\nbefore you can create this report." );
		else
			GetParent() -> PostMessage ( WM_APP, m_nType, (long) this );
	}
}

/**********************************************************************/
