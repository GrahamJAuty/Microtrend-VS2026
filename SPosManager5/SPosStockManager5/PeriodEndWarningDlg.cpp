/**********************************************************************/
#include "..\CommonStockTray\StockLevelsCSVArray.h"
#include "StocktakeCSVArray.h"
#include "PeriodEndWarningDlg.h"
/**********************************************************************/

CPeriodEndWarningDlg::CPeriodEndWarningDlg( int nStockItems, int nZeroClosing, int nStocktakeType, CWnd* pParent)
	: CDialog(CPeriodEndWarningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeriodEndWarningDlg)
	//}}AFX_DATA_INIT
	m_nStockItems = nStockItems;
	m_nZeroClosing = nZeroClosing;
	m_nStocktakeType = nStocktakeType;
}

/**********************************************************************/

void CPeriodEndWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeriodEndWarningDlg)
	DDX_Control(pDX, IDC_STATIC_MESSAGE4, m_staticMessage4);
	DDX_Control(pDX, IDC_STATIC_MESSAGE3, m_staticMessage3);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_STATIC_MESSAGE2, m_staticMessage2);
	DDX_Control(pDX, IDC_STATIC_MESSAGE1, m_staticMessage1);
	DDX_Control(pDX, IDC_STATIC_ZEROCLOSING, m_staticZeroClosing);
	DDX_Control(pDX, IDC_STATIC_STOCKITEMS, m_staticStockItems);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPeriodEndWarningDlg, CDialog)
	//{{AFX_MSG_MAP(CPeriodEndWarningDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPeriodEndWarningDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strNum;
	strNum.Format ( "%d", m_nStockItems );
	m_staticStockItems.SetWindowText ( strNum );
	strNum.Format ( "%d", m_nZeroClosing );
	m_staticZeroClosing.SetWindowText ( strNum );

	switch ( m_nStocktakeType )
	{
	case STOCKTAKE_TYPE_ADJUST:
		m_staticInfo1.SetWindowText( "Stock items selected for adjustment" );
		m_staticMessage1.SetWindowText ( "Are you sure you wish to adjust apparent stock levels to match" );
		m_staticMessage2.SetWindowText ( "the closing stock levels that you have specified ?" );
		m_staticMessage3.SetWindowText ( "Some of the stock items that are selected for adjustment have a" );
		m_staticMessage4.SetWindowText ( "closing stock level of zero." );
		break;
	
	case STOCKTAKE_TYPE_YTD:
		m_staticInfo1.SetWindowText( "Stock items selected for YTD end" );
		m_staticMessage1.SetWindowText ( "Are you sure you wish to use closing stock levels to set opening" );
		m_staticMessage2.SetWindowText ( "stock levels for the new year ?" );
		m_staticMessage3.SetWindowText ( "Some of the stock items that are selected for YTD end have a" );
		m_staticMessage4.SetWindowText ( "closing stock level of zero." );
		break;

	case STOCKTAKE_TYPE_PERIOD:
	default:
		m_staticInfo1.SetWindowText( "Stock items selected for period end" );
		m_staticMessage1.SetWindowText ( "Are you sure you wish to use closing stock levels to set opening" );
		m_staticMessage2.SetWindowText ( "stock levels for the new period ?" );
		m_staticMessage3.SetWindowText ( "Some of the stock items that are selected for period end have a" );
		m_staticMessage4.SetWindowText ( "closing stock level of zero." );
		break;
	}

	SetDefID ( IDCANCEL );
	return TRUE;  
}

/**********************************************************************/
