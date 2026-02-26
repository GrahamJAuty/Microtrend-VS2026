/**********************************************************************/
#include "StockReportTop100Dlg.h"
/**********************************************************************/

CStockReportTop100Dlg::CStockReportTop100Dlg( int nContext, const char* szText, CSalesHistoryCSVRecordPlu& chartinfo, CWnd* pParent)
	: CSSDialog(CStockReportTop100Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportTop100Dlg)
	//}}AFX_DATA_INIT
	m_nContext = nContext;
	m_strText = szText;
	
	m_chartinfo.SetChartType( chartinfo.GetChartType() );
	m_chartinfo.SetBottom100Flag( chartinfo.IsBottom100() );
	m_chartinfo.SetSortedByValueFlag( chartinfo.IsSortedByValue() );
	m_chartinfo.SetChartSize( chartinfo.GetChartSize() );
	m_chartinfo.SetChartCutOff( chartinfo.GetChartCutOff() );
	
	m_nRadioQtyValue = m_chartinfo.IsSortedByValue() ? 1 : 0;
	m_nRadioTopBottom = m_chartinfo.IsBottom100() ? 1 : 0;
	m_nRadioUnlimited = m_chartinfo.GetChartType();
	m_nChartMaxItems = m_chartinfo.GetChartSize();
	m_dChartCutOff = m_chartinfo.GetChartCutOff();
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

CStockReportTop100Dlg::CStockReportTop100Dlg( int nContext, const char* szText, CSalesHistoryCSVRecordStock& chartinfo, CWnd* pParent)
	: CSSDialog(CStockReportTop100Dlg::IDD, pParent)
{
	m_nContext = nContext;
	m_strText = szText;
	
	m_chartinfo.SetChartType( chartinfo.GetChartType() );
	m_chartinfo.SetBottom100Flag( chartinfo.IsBottom100() );
	m_chartinfo.SetSortedByValueFlag( chartinfo.IsSortedByValue() );
	m_chartinfo.SetChartSize( chartinfo.GetChartSize() );
	m_chartinfo.SetChartCutOff( chartinfo.GetChartCutOff() );
	
	m_nRadioQtyValue = m_chartinfo.IsSortedByValue() ? 1 : 0;
	m_nRadioTopBottom = m_chartinfo.IsBottom100() ? 1 : 0;
	m_nRadioUnlimited = m_chartinfo.GetChartType();
	m_nChartMaxItems = m_chartinfo.GetChartSize();
	m_dChartCutOff = m_chartinfo.GetChartCutOff();
}

/**********************************************************************/
#endif
/**********************************************************************/

void CStockReportTop100Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportTop100Dlg)
	DDX_Radio(pDX, IDC_RADIO_SORTTYPE, m_nRadioQtyValue);
	DDX_Radio(pDX, IDC_RADIO_TOP, m_nRadioTopBottom);
	DDX_Radio(pDX, IDC_RADIO_UNLIMITED, m_nRadioUnlimited);
	DDX_Text(pDX, IDC_EDIT_MAXITEMS, m_nChartMaxItems);
	DDX_Control(pDX, IDC_RADIO_TOP, m_radioTop);
	DDX_Control(pDX, IDC_RADIO_BOTTOM, m_radioBottom);
	DDX_Text(pDX, IDC_EDIT_CUTOFF, m_dChartCutOff);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportTop100Dlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportTop100Dlg)
	ON_BN_CLICKED(IDC_RADIO_UNLIMITED, OnRadioUnlimited)
	ON_BN_CLICKED(IDC_RADIO_CUTOFF, OnRadioCutOff)
	ON_BN_CLICKED(IDC_RADIO_MAXITEMS, OnRadioMaxItems)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockReportTop100Dlg::GetEditCutOff() { return GetEdit( IDC_EDIT_CUTOFF ); }
CEdit* CStockReportTop100Dlg::GetEditMaxItems() { return GetEdit( IDC_EDIT_MAXITEMS ); }
/**********************************************************************/

BOOL CStockReportTop100Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_MAXITEMS, SS_NUM, 4, "%d" );
	SubclassEdit ( IDC_EDIT_CUTOFF, SS_NUM_SDP, 10, "%.5f" );

	GetEditCutOff() -> EnableWindow( m_nRadioUnlimited == CHART_TYPE_CUTOFF );
	GetEditMaxItems() -> EnableWindow( m_nRadioUnlimited == CHART_TYPE_MAXITEMS );
	
	m_radioTop.SetWindowText( "Top Sellers" );
	m_radioBottom.SetWindowText( "Bottom Sellers" );

	if ( m_strText != "" )
		SetWindowText( m_strText );
	else
	{
		switch ( m_nContext )
		{
		case TOP100_STOCKLEVEL_SALES_PERIOD:
			SetWindowText ( "Top/Bottom Period Sellers" );
			break;

		case TOP100_STOCKLEVEL_SALES_YTD:
			SetWindowText ( "Top/Bottom YTD Sellers" );
			break;

		case TOP100_STOCKLEVEL_SALES_YTD_PLUS_PERIOD:
			SetWindowText ( "Top/Bottom YTD + Period Sellers" );
			break;

		case TOP100_STOCKLEVEL_APPARENT:
			SetWindowText ( "Top/Bottom Apparent Stock" );
			m_radioTop.SetWindowText( "Highest first" );
			m_radioBottom.SetWindowText( "Lowest first" );
			break;

		case TOP100_SALESHISTORY_STOCK:
			SetWindowText ( "Top/Bottom Stock Items" );
			break;

		case TOP100_SALESHISTORY_DEPT:
			SetWindowText ( "Top/Bottom Departments" );
			break;

		case TOP100_EMAILTASK:
			SetWindowText ( "Chart Settings for PosTray Task" );
			break;

		case TOP100_SALESHISTORY_PLU:
		default:
			SetWindowText ( "Top/Bottom Plu Items" );
			break;
		}
	}

	return TRUE;  
}

/**********************************************************************/

void CStockReportTop100Dlg::OnRadioUnlimited() 
{
	GetEditCutOff() -> EnableWindow( FALSE );
	GetEditMaxItems() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CStockReportTop100Dlg::OnRadioMaxItems() 
{
	GetEditCutOff() -> EnableWindow( FALSE );
	GetEditMaxItems() -> EnableWindow( TRUE );
}

/**********************************************************************/

void CStockReportTop100Dlg::OnRadioCutOff() 
{
	GetEditCutOff() -> EnableWindow( TRUE );
	GetEditMaxItems() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CStockReportTop100Dlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_chartinfo.SetChartType( m_nRadioUnlimited );
		m_chartinfo.SetBottom100Flag( m_nRadioTopBottom == 1 );
		m_chartinfo.SetSortedByValueFlag( m_nRadioQtyValue == 1 );
		m_chartinfo.SetChartSize( m_nChartMaxItems );
		m_chartinfo.SetChartCutOff( m_dChartCutOff );

		if ( TOP100_EMAILTASK != m_nContext )
			GetParent() -> PostMessage ( WM_APP, TOP100_MESSAGE, (long) this );
		else
			EndDialog( IDOK );
	}
}

/**********************************************************************/

void CStockReportTop100Dlg::GetChartInfo( CSalesHistoryCSVRecordPlu& chartinfo )
{
	chartinfo.SetChartType( m_chartinfo.GetChartType() );
	chartinfo.SetBottom100Flag( m_chartinfo.IsBottom100() );
	chartinfo.SetSortedByValueFlag( m_chartinfo.IsSortedByValue() );
	chartinfo.SetChartSize( m_chartinfo.GetChartSize() );
	chartinfo.SetChartCutOff( m_chartinfo.GetChartCutOff() );
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CStockReportTop100Dlg::GetChartInfo( CSalesHistoryCSVRecordStock& chartinfo )
{
	chartinfo.SetChartType( m_chartinfo.GetChartType() );
	chartinfo.SetBottom100Flag( m_chartinfo.IsBottom100() );
	chartinfo.SetSortedByValueFlag( m_chartinfo.IsSortedByValue() );
	chartinfo.SetChartSize( m_chartinfo.GetChartSize() );
	chartinfo.SetChartCutOff( m_chartinfo.GetChartCutOff() );
}

/**********************************************************************/
#endif
/**********************************************************************/
