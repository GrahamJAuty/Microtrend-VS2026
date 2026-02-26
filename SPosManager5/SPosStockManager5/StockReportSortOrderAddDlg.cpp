/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "StockReportSortOrderAddDlg.h"
/**********************************************************************/

CStockReportSortOrderAddDlg::CStockReportSortOrderAddDlg( CWnd* pParent)
	: CSalesHistoryCSVRecordAddDlg(pParent)
{
}

/**********************************************************************/

void CStockReportSortOrderAddDlg::PrepareComboAndTitle()
{
	m_comboReport.AddString ( "Custom Sort Order" );
	m_comboReport.AddString( "All standard sort orders" );

	CStockReportSortOrderCSVRecord SortOrder;
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSystemSortOrderList( m_arrayReportTypes );
			
	for ( int n = 0; n < m_arrayReportTypes.GetSize(); n++ )
	{
		SortOrder.SetSortOrderType ( m_arrayReportTypes.GetAt(n) );
		m_comboReport.AddString( SortOrder.GetListName() );
	}

	SetWindowText ( "Add Sort Order" );

	m_arrayReportTypes.InsertAt( 0, SH_CUSTOM, 1 );
	m_arrayReportTypes.InsertAt( 1, 999, 1 );

	m_comboReport.SetCurSel(0);
}

/**********************************************************************/
