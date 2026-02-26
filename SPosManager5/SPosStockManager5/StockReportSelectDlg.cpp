/**********************************************************************/
#include "StockOptionsIni.h"
#include "StockReportNames.h"
#include "StockReportSelect.h"
/**********************************************************************/
#include "StockReportSelectDlg.h"
/**********************************************************************/

CStockReportSelectDlg::CStockReportSelectDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog(CStockReportSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportSelectDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CStockReportSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportSelectDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportSelectDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_RESETORDER, OnButtonResetOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listReports.SubclassDlgItem ( IDC_LIST, this );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listReports.InsertColumn ( 0, "", LVCFMT_LEFT, 250 );
	m_listReports.SetLockItemZeroFlag( FALSE );

	BuildReportList();

	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CStockReportSelect StockReportSelect;
	StockReportSelect.LoadReportSelection();
		
	int nNextTickPos = 0;

	//TICK THE OPTIONS THAT WERE FOUND IN THE FILE
	for ( int nPos = 0; nPos < StockReportSelect.GetReportCount(); nPos++ )
	{
		int nType = StockReportSelect.GetReportType(nPos);
		
		for ( int nIndex = nNextTickPos; nIndex < m_listReports.GetItemCount(); nIndex++ )
		{
			CSSListTaggedSelectItem item;
			m_listReports.GetListItem( nIndex, item );

			if ( item.m_nData == nType )
			{			
				if ( nIndex != nNextTickPos )
				{
					m_listReports.SwapItems( nIndex, nNextTickPos );
				}
				
				m_listReports.SetItemTagState( nNextTickPos++, 1 );

				break;
			}
		}
	}		
	
	return TRUE;  
}

/**********************************************************************/

void CStockReportSelectDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listReports.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStockReportSelectDlg::BuildReportList()
{
	m_listReports.ClearList();
	AddReportToList( STOCK_SALES );
	AddReportToList( STOCK_TOP_100_SALES );
	AddReportToList( STOCK_ESTIMATED_PROFIT );
	AddReportToList( STOCK_REORDER );
	AddReportToList( ORDER_ONORDER_SUPPLIER );
	AddReportToList( ORDER_ONORDER_CATEGORY );
	AddReportToList( STOCK_APPARENT_QTY );
	AddReportToList( STOCK_APPARENT_QTY_DU );
	AddReportToList( STOCK_APPARENT_SUMMARY );
	AddReportToList( STOCK_APPARENT_SUMMARY_DU );
	AddReportToList( STOCK_TOP_100_APPARENT );
	AddReportToList( STOCK_RETAIL_SUMMARY );
	AddReportToList( STOCK_COST_SUMMARY );
	AddReportToList( STOCK_APPARENT_VALUE );
	AddReportToList( STOCK_OPENING_VALUE );
	AddReportToList( STOCK_OVERSTOCK_VALUE );
	
	if ( Sysset.GetMaxStockPoints() >= 2 )
	{
		AddReportToList( STOCK_TRANSFER_VALUE );
		AddReportToList( STOCK_TRANSFER_PERIOD );
	}

	if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
		AddReportToList( STOCK_SUPPLIER );

	AddReportToList( STOCK_DELIVERY );
	AddReportToList( STOCK_RETURNS );
	AddReportToList( STOCK_ADJUST );
	AddReportToList( STOCK_SHEET );
	AddReportToList( STOCK_PRICE_LIST );
}

/**********************************************************************/

void CStockReportSelectDlg::AddReportToList( int nReportType )
{
	CStockReportNames ReportNames;

	CSSListTaggedSelectItem item;
	item.m_strText = ReportNames.GetComboName( nReportType );
	item.m_nData = nReportType;
	item.SetTagState(0);
	m_listReports.AddItemToList( item );
}

/**********************************************************************/

void CStockReportSelectDlg::OnButtonUp() 
{
	int nSel = m_listReports.GetCurSel();
	if ( nSel > 0 && nSel < m_listReports.GetItemCount() )
	{
		m_listReports.SwapItems( nSel, nSel - 1 );
	}
}

/**********************************************************************/

void CStockReportSelectDlg::OnButtonDown() 
{
	int nSel = m_listReports.GetCurSel();
	if ( nSel >= 0 && nSel < m_listReports.GetItemCount() - 1 )
	{
		m_listReports.SwapItems( nSel, nSel + 1 );
	}
}

/**********************************************************************/

void CStockReportSelectDlg::OnButtonResetOrder() 
{
	int nSelectedField = -1;

	int nSel = m_listReports.GetCurSel();
	if ( nSel >= 0 && nSel < m_listReports.GetItemCount() )
	{
		CSSListTaggedSelectItem item;
		m_listReports.GetListItem( nSel, item );
		nSelectedField = item.m_nData;
	}

	CReportConsolidationArray<CSortedIntItem> arrayTicked;

	for ( int n = 0; n < m_listReports.GetItemCount(); n++ )
	{
		CSSListTaggedSelectItem item;
		m_listReports.GetListItem( n, item );

		if ( item.GetTagState() == 1 )
		{
			CSortedIntItem tickedItem;
			tickedItem.m_nItem = item.m_nData;
			arrayTicked.Consolidate(tickedItem);
		}
	}

	BuildReportList();

	for ( int c = 0; c < m_listReports.GetItemCount(); c++ )
	{
		CSSListTaggedSelectItem item;
		m_listReports.GetListItem( c, item );

		CSortedIntItem tickedItem;
		tickedItem.m_nItem = item.m_nData;

		int nPos;
		if ( arrayTicked.Find( tickedItem, nPos ) == TRUE )
		{
			m_listReports.SetItemTagState( c, 1 );
		}
	}

	if ( nSelectedField != -1 )
	{
		int nField = nSelectedField;

		for ( int c = 0; c < m_listReports.GetItemCount(); c++ )
		{
			CSSListTaggedSelectItem item;
			m_listReports.GetListItem( nSel, item );
		
			if ( nField == item.m_nData )
			{
				m_listReports.SetCurSel(c);
				break;
			}
		}
	}
}

/**********************************************************************/

void CStockReportSelectDlg::OnButtonAll() 
{
	m_listReports.SelectAll();
}

/**********************************************************************/

void CStockReportSelectDlg::OnButtonNone() 
{
	m_listReports.SelectNone();	
}

/**********************************************************************/

void CStockReportSelectDlg::AppendFieldTicks( CCSV& csv )
{
	for ( int n = 0; n < m_listReports.GetItemCount(); n++ )
	{
		CSSListTaggedSelectItem item;
		m_listReports.GetListItem( n, item );

		if ( item.GetTagState() == 1 )
		{
			csv.Add( item.m_nData );
		}
	}
}

/**********************************************************************/

void CStockReportSelectDlg::OnOK() 
{
	CCSV csv;
	AppendFieldTicks( csv );

	//PREVENT OPTIONS THAT ARE CURRENTLY SWITCHED OFF FROM BLOCKING
	//REPORTS IN FUTURE IF THESE OPTIONS ARE SWITCHED ON.
	if ( Sysset.GetMaxStockPoints() < 2 )
	{
		csv.Add( STOCK_TRANSFER_VALUE );
		csv.Add( STOCK_TRANSFER_PERIOD );
	}

	if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
		csv.Add( STOCK_SUPPLIER );

	CStockReportSelect StockReportSelect;

	for ( int n = 0; n < csv.GetSize(); n++ )
		StockReportSelect.AddReport( csv.GetInt(n) );

	StockReportSelect.SaveReportSelection();
	
	EndDialog( IDOK );
}

/**********************************************************************/


