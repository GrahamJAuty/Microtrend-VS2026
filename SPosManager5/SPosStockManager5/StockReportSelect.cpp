/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockReportSelect.h"
/**********************************************************************/

CStockReportSelect::CStockReportSelect(void)
{
}

/**********************************************************************/

int CStockReportSelect::GetReportType( int nPos )
{
	if ( ( nPos >= 0 ) && ( nPos < m_arrayReports.GetSize() ) )
		return m_arrayReports.GetAt( nPos );
	else
		return STOCK_SALES;
}

/**********************************************************************/

void CStockReportSelect::LoadReportSelection( bool bSummary )
{
	m_arrayReports.RemoveAll();

	CSSFile fileReports;
	CFilenameUpdater FnUp( SysFiles::StockReportList );
	if ( fileReports.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		CString strBuffer;
		fileReports.ReadString ( strBuffer );
		fileReports.Close();

		CCSV csv( strBuffer );
		for ( int n = 0; n < csv.GetSize(); n++ )
			m_arrayReports.Add( csv.GetInt(n) );
	}
	else
	{
		m_arrayReports.Add( STOCK_SALES );
		m_arrayReports.Add( STOCK_TOP_100_SALES );
		m_arrayReports.Add( STOCK_ESTIMATED_PROFIT );
		m_arrayReports.Add( STOCK_REORDER );
		m_arrayReports.Add( ORDER_ONORDER_SUPPLIER );
		m_arrayReports.Add( ORDER_ONORDER_CATEGORY );
		m_arrayReports.Add( STOCK_APPARENT_QTY );
		m_arrayReports.Add( STOCK_APPARENT_SUMMARY );
		m_arrayReports.Add( STOCK_TOP_100_APPARENT );

		if ( TRUE == bSummary )
		{
			m_arrayReports.Add( STOCK_RETAIL_SUMMARY );
			m_arrayReports.Add( STOCK_COST_SUMMARY );
		}

		m_arrayReports.Add( STOCK_APPARENT_VALUE );
		m_arrayReports.Add( STOCK_OPENING_VALUE );
		m_arrayReports.Add( STOCK_OVERSTOCK_VALUE );
		m_arrayReports.Add( STOCK_TRANSFER_VALUE );
		m_arrayReports.Add( STOCK_TRANSFER_PERIOD );
		m_arrayReports.Add( STOCK_SUPPLIER );
		m_arrayReports.Add( STOCK_DELIVERY );
		m_arrayReports.Add( STOCK_RETURNS );
		m_arrayReports.Add( STOCK_ADJUST );
		m_arrayReports.Add( STOCK_SHEET );
		m_arrayReports.Add( STOCK_PRICE_LIST );
	}
}

/**********************************************************************/

void CStockReportSelect::SaveReportSelection()
{
	CCSV csv;
	for ( int n = 0; n < GetReportCount(); n++ )
		csv.Add( GetReportType( n ) );

	CSSFile fileReports;
	CFilenameUpdater FnUp( SysFiles::StockReportList );
	if ( fileReports.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE ) 
		fileReports.WriteLine( csv.GetLine() );
}

/**********************************************************************/
