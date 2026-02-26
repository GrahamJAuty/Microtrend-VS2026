/**********************************************************************/
#include "PriceHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewApparent.h"
/**********************************************************************/

CStockTreeReportNewApparent::CStockTreeReportNewApparent( bool bFull, CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bFull = bFull;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;

	if ( TRUE == bFull )
	{
		m_nReportParamsType = STOCK_APPARENT_QTY;
		m_nTotalisersPerLine = 11;
	}
	else
	{
		m_nReportParamsType = STOCK_APPARENT_SUMMARY;
		m_nTotalisersPerLine = 5;
	}

	m_bHideAboveMin = FALSE;
	m_bPositive = TRUE;
	m_bZero = TRUE;
	m_bNegative = TRUE;
}

/**********************************************************************/

void CStockTreeReportNewApparent::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewApparent::AddReportColumns()
{			
	if ( TRUE == m_bFull )
	{
		AddColumn ( "Opening", TA_RIGHT, 180, TRUE );
		AddColumn ( "Deliveries", TA_RIGHT, 210, TRUE );	
		AddColumn ( "Free", TA_RIGHT, 180, TRUE );
		AddColumn ( "Returns", TA_RIGHT, 180, TRUE );
			
		if ( Sysset.GetMaxStockPoints() > 1 )
			AddColumn ( "Xfer In", TA_RIGHT, 180, TRUE );
		
		AddColumn ( "Sales", TA_RIGHT, 180, TRUE );
		
		if ( m_bSubUnits == TRUE )
			AddColumn ( "Est. Waste", TA_RIGHT, 220, TRUE );
		else
			AddColumn ( "Epos Waste", TA_RIGHT, 220, TRUE );

		if ( Sysset.GetMaxStockPoints() > 1 )
			AddColumn ( "Xfer Out", TA_RIGHT, 180, TRUE );
			
		AddColumn ( "Adjust.", TA_RIGHT, 180, TRUE );
		AddColumn ( "Stocktake", TA_RIGHT, 210, TRUE );
		AddColumn ( "Total", TA_RIGHT, 180, TRUE );
	}
	else
	{
		AddColumn ( "Opening", TA_RIGHT, 210, TRUE );
		AddColumn ( "Stock In", TA_RIGHT, 210, TRUE );
		AddColumn ( "Stock Out", TA_RIGHT, 210, TRUE );
		AddColumn ( "Stocktake", TA_RIGHT, 210, TRUE );
		AddColumn ( "Total", TA_RIGHT, 210, TRUE );
	}
}

/**********************************************************************/

bool CStockTreeReportNewApparent::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	double dApparentQty = StockLevels.GetApparentQty();
	
	if ( ( m_bPositive == FALSE ) || ( m_bZero == FALSE ) || ( m_bNegative == FALSE ) )
	{
		int nResult = CPriceHelpers::CompareDoubles( dApparentQty, 0.0, 5 );

		if ( m_bPositive == FALSE )
			if ( nResult == 1 )
				return FALSE;

		if ( m_bZero == FALSE )
			if ( nResult == 0 )
				return FALSE;

		if ( m_bNegative == FALSE )
			if ( nResult == -1 )
				return FALSE;
	}

	if ( TRUE == m_bHideAboveMin )
	{
		if ( dApparentQty >= StockLevels.GetMinimumLevelQty() )
			return FALSE;
	}

	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	OutputStockCodeFields( csv, m_bStockUnit );

	if ( TRUE == m_bFull )
	{		
		double dOpening = StockLevels.GetOpeningQty();
		double dDelivery = StockLevels.GetDeliveryQty();
		double dFree = StockLevels.GetFreeQty();
		double dReturns = StockLevels.GetReturnsQty();
		double dXferIn = StockLevels.GetTransferInQty();
		double dSales = StockLevels.GetTotalSalesQty();
		double dPending = StockLevels.GetPendingStocktakeQty();
		double dXferOut = StockLevels.GetTransferOutQty();
		double dAdjust = StockLevels.GetAdjustmentQty();
		
		double dWastage = StockLevels.GetEcrWastageQty();
		if ( m_bSubUnits == TRUE )	dWastage += EstimateWastageFromSales( dSales );
		
		csv.AppendStockQty( dOpening, nSubUnits );
		csv.AppendStockQty( dDelivery, nSubUnits );
		csv.AppendStockQty( dFree, nSubUnits );
		csv.AppendStockQty( dReturns, nSubUnits );

		if ( Sysset.GetMaxStockPoints() > 1 )
			csv.AppendStockQty( dXferIn, nSubUnits );

		csv.AppendStockQty( dSales, nSubUnits );
		csv.AppendStockQty( dWastage, nSubUnits );

		if ( Sysset.GetMaxStockPoints() > 1 )
			csv.AppendStockQty( dXferOut, nSubUnits );

		csv.AppendStockQty( dAdjust, nSubUnits );
		csv.AppendStockQty( dPending, nSubUnits );

		csv.AppendStockQty( dApparentQty, nSubUnits );

		OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

		AddToTotalisers( 0, dOpening );
		AddToTotalisers( 1, dDelivery );
		AddToTotalisers( 2, dFree );
		AddToTotalisers( 3, dReturns );
		AddToTotalisers( 4, dXferIn );
		AddToTotalisers( 5, dSales );
		AddToTotalisers( 6, dWastage );
		AddToTotalisers( 7, dXferOut );
		AddToTotalisers( 8, dAdjust );
		AddToTotalisers( 9, dPending );
		AddToTotalisers( 10, dApparentQty );
	}
	else
	{
		double dOpening = StockLevels.GetOpeningQty();

		double dStockIn = 0.0;
		dStockIn += StockLevels.GetDeliveryQty();
		dStockIn += StockLevels.GetFreeQty();
		dStockIn += StockLevels.GetTransferInQty();
		
		double dSales = StockLevels.GetTotalSalesQty();
		
		double dStockOut = 0.0;
		dStockOut += dSales;
		dStockOut += StockLevels.GetReturnsQty();
		dStockOut += StockLevels.GetTransferOutQty();
		dStockOut += StockLevels.GetAdjustmentQty();
		dStockOut += StockLevels.GetEcrWastageQty();
		if ( m_bSubUnits == TRUE )	dStockOut += EstimateWastageFromSales( dSales );
		
		double dPending = StockLevels.GetPendingStocktakeQty();
		
		csv.AppendStockQty( dOpening, nSubUnits );
		csv.AppendStockQty( dStockIn, nSubUnits );
		csv.AppendStockQty( dStockOut, nSubUnits );
		csv.AppendStockQty( dPending, nSubUnits );
		csv.AppendStockQty( dApparentQty, nSubUnits );
		
		OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

		AddToTotalisers( 0, dOpening );
		AddToTotalisers( 1, dStockIn );
		AddToTotalisers( 2, dStockOut );
		AddToTotalisers( 3, dPending );
		AddToTotalisers( 4, dApparentQty );
	}

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewApparent::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	if ( TRUE == m_bFull )
	{
		double dOpening = GetTotaliser( nDepth, 0 );
		double dDelivery = GetTotaliser( nDepth, 1 );
		double dFree = GetTotaliser( nDepth, 2 );
		double dReturns = GetTotaliser( nDepth, 3 );
		double dXferIn = GetTotaliser( nDepth, 4 );
		double dSales = GetTotaliser( nDepth, 5 );
		double dWastage = GetTotaliser( nDepth, 6 );
		double dXferOut = GetTotaliser( nDepth, 7 );
		double dAdjust = GetTotaliser( nDepth, 8 );
		double dPending = GetTotaliser( nDepth, 9 );
		double dApparent = GetTotaliser( nDepth, 10 );
		
		csv.AppendStockQty( dOpening );
		csv.AppendStockQty( dDelivery );
		csv.AppendStockQty( dFree );
		csv.AppendStockQty( dReturns );

		if ( Sysset.GetMaxStockPoints() > 1 )
			csv.AppendStockQty( dXferIn );

		csv.AppendStockQty( dSales );
		csv.AppendStockQty( dWastage );

		if ( Sysset.GetMaxStockPoints() > 1 )
			csv.AppendStockQty( dXferOut );

		csv.AppendStockQty( dAdjust );
		csv.AppendStockQty( dPending );
		csv.AppendStockQty( dApparent );
	}
	else
	{
		double dOpening = GetTotaliser( nDepth, 0 );
		double dStockIn = GetTotaliser( nDepth, 1 );
		double dStockOut = GetTotaliser( nDepth, 2 );
		double dPending = GetTotaliser( nDepth, 3 );
		double dApparent = GetTotaliser( nDepth, 4 );
		
		csv.AppendStockQty( dOpening );
		csv.AppendStockQty( dStockIn );
		csv.AppendStockQty( dStockOut );
		csv.AppendStockQty( dPending );
		csv.AppendStockQty( dApparent );
	}
}

/**********************************************************************/



