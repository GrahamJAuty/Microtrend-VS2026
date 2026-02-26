/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "StockMonitor.h"
#include "StockOptionsIni.h"
#include "StockTreeReportNew.h"
/**********************************************************************/
extern CSupplierCSVArray SupplierArray;
/**********************************************************************/

void CStockTreeReportNew::CreateReportLine( CStockLevelsCSVRecord& StockLevels )
{
	//APPARENT STOCK NEEDS THIS IN ORDER TO WORK CORRECTLY
	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
		StockLevels.SetAutoWastagePercent( m_LabelStockRecord.GetWastePercent() );
	else
		StockLevels.SetAutoWastagePercent( 0 );
	
	bool bSummaryReport = m_StockLevelTree.GetSummaryFlag();
	
	CStockReportLine csv;

	if ( ( CreateReportLine( csv, StockLevels ) == TRUE ) && ( bSummaryReport == FALSE ) ) 
	{
		WriteReportLine( csv.GetLine() );
		if ( m_bDrawLines == TRUE ) WriteReportLine ( "<LI>" );
	}
}

/**********************************************************************/

void CStockTreeReportNew::CreateReportLine( CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord& TransferDetail )
{
	//APPARENT STOCK NEEDS THIS IN ORDER TO WORK CORRECTLY
	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
		StockLevels.SetAutoWastagePercent( m_LabelStockRecord.GetWastePercent() );
	else
		StockLevels.SetAutoWastagePercent( 0 );
	
	bool bSummaryReport = m_StockLevelTree.GetSummaryFlag();
	
	CStockReportLine csv;

	if ( ( CreateReportLine( csv, StockLevels, TransferDetail ) == TRUE ) && ( bSummaryReport == FALSE ) ) 
	{
		WriteReportLine( csv.GetLine() );
		if ( m_bDrawLines == TRUE ) WriteReportLine ( "<LI>" );
	}
}

/**********************************************************************/

void CStockTreeReportNew::CreateReportLine( CStockLevelsCSVRecord& StockLevels, CSalesCostCSVRecord& SalesCost )
{
	//APPARENT STOCK NEEDS THIS IN ORDER TO WORK CORRECTLY
	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
		StockLevels.SetAutoWastagePercent( m_LabelStockRecord.GetWastePercent() );
	else
		StockLevels.SetAutoWastagePercent( 0 );
	
	bool bSummaryReport = m_StockLevelTree.GetSummaryFlag();
	
	CStockReportLine csv;

	if ( ( CreateReportLine( csv, StockLevels, SalesCost ) == TRUE ) && ( bSummaryReport == FALSE ) ) 
	{
		WriteReportLine( csv.GetLine() );
		if ( m_bDrawLines == TRUE ) WriteReportLine ( "<LI>" );
	}
}

/**********************************************************************/

void CStockTreeReportNew::CreateTotalsLine( int nDepth )
{
	bool bSummary = m_StockLevelTree.GetSummaryFlag();
	bool bSummaryLine = ( bSummary && ( nDepth == m_arrayTotalFlags.GetSize() - 2 ) );
	
	CStockReportLine csv;
		
	if ( bSummaryLine == FALSE )
	{		
		CCSV csvUnderline( '\t' );
		for ( int n = 0; n < m_arrayTotalPos.GetSize(); n++ )
		{
			int nPos = m_arrayTotalPos.GetAt(n);
			csvUnderline.SetAt( nPos, "<LI>" );
		}
		WriteReportLine( csvUnderline.GetLine() );
	
		AddTotalColumnText( csv, nDepth );
		while ( csv.GetSize() < m_nItemIDColumns )
			csv.Add( "" );
	}
	else
		OutputStockCodeFields( csv, FALSE );
		
	CreateTotalsLine( csv, nDepth );
		
	WriteReportLine ( csv.GetLine() );	
	if( bSummaryLine == FALSE ) WriteBlankLine( FALSE );
}

/**********************************************************************/

void CStockTreeReportNew::AddToTotalisers( int nTotaliser, double dValue )
{
	int nOffset = nTotaliser;
	while ( nOffset < m_arrayTotalisers.GetSize() )
	{
		m_arrayTotalisers[ nOffset ] += dValue;	
		nOffset += m_nTotalisersPerLine;			
	}
}

/**********************************************************************/

double CStockTreeReportNew::GetTotaliser( int nDepth, int nTotaliser )
{
	int nOffset = ( nDepth * m_nTotalisersPerLine ) + nTotaliser;

	if ( nOffset >= 0 && nOffset < m_arrayTotalisers.GetSize() )
		return m_arrayTotalisers.GetAt( nOffset );
	else
		return 0.0;
}

/**********************************************************************/

