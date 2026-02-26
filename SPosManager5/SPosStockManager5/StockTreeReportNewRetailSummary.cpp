/**********************************************************************/
 
/**********************************************************************/
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewRetailSummary.h"
/**********************************************************************/

CStockTreeReportNewRetailSummary::CStockTreeReportNewRetailSummary( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_RETAIL_SUMMARY;
	m_nTotalisersPerLine = 11;
}

/**********************************************************************/

void CStockTreeReportNewRetailSummary::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewRetailSummary::AddReportColumns()
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
	AddColumn ( "Stocktake", TA_RIGHT, 180, TRUE );
	AddColumn ( "Total", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewRetailSummary::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	double dOpening = StockLevels.GetOpeningQty();
	double dDelivery = StockLevels.GetDeliveryQty();
	double dFree = StockLevels.GetFreeQty();
	double dReturns = StockLevels.GetReturnsQty();
	double dXferIn = StockLevels.GetTransferInQty();
	double dSales = StockLevels.GetTotalSalesQty();
	double dPending = StockLevels.GetPendingStocktakeQty();
	double dApparent = StockLevels.GetApparentQty();
	double dXferOut = StockLevels.GetTransferOutQty();
	double dAdjust = StockLevels.GetAdjustmentQty();
	
	double dWastage = StockLevels.GetEcrWastageQty();
	if ( m_bSubUnits == TRUE )	dWastage += EstimateWastageFromSales( dSales );
	
	OutputStockCodeFields( csv, m_bStockUnit );

	double dPluRetail = 0.0;
	double dPluStockQty = 1.0;
	if ( GetRetailPluInfo( TRUE ) == TRUE )
	{
		dPluRetail = m_dRetailPrice1;
		dPluStockQty = m_dRetailStockQty;
	}

	//KLUDGE - AVOID DIVISION BY ZERO
	if ( dPluStockQty == 0.0 )
	{
		dPluStockQty = 1.0;
		dPluRetail = 0.0;
	}

	dOpening = ( dOpening * dPluRetail ) / dPluStockQty;
	dDelivery = ( dDelivery * dPluRetail ) / dPluStockQty;
	dFree = ( dFree * dPluRetail ) / dPluStockQty;
	dReturns = ( dReturns * dPluRetail ) / dPluStockQty;
	dXferIn = ( dXferIn * dPluRetail ) / dPluStockQty;
	dSales = ( dSales * dPluRetail ) / dPluStockQty;
	dWastage = ( dWastage * dPluRetail ) / dPluStockQty;
	dXferOut = ( dXferOut * dPluRetail ) / dPluStockQty;
	dAdjust = ( dAdjust * dPluRetail ) / dPluStockQty;
	dPending = ( dPending * dPluRetail ) / dPluStockQty;
	dApparent = ( dApparent * dPluRetail ) / dPluStockQty;

	csv.AppendVal( dOpening );
	csv.AppendVal( dDelivery );
	csv.AppendVal( dFree );
	csv.AppendVal( dReturns );

	if ( Sysset.GetMaxStockPoints() > 1 )
		csv.AppendVal( dXferIn );

	csv.AppendVal( dSales );
	csv.AppendVal( dWastage );

	if ( Sysset.GetMaxStockPoints() > 1 )
		csv.AppendVal( dXferOut );

	csv.AppendVal( dAdjust );
	csv.AppendVal( dPending );
	csv.AppendVal( dApparent );

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
	AddToTotalisers( 10, dApparent );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewRetailSummary::CreateTotalsLine( CStockReportLine& csv, int nDepth )
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
	
	csv.AppendVal( dOpening );
	csv.AppendVal( dDelivery );
	csv.AppendVal( dFree );
	csv.AppendVal( dReturns );

	if ( Sysset.GetMaxStockPoints() > 1 )
		csv.AppendVal( dXferIn );

	csv.AppendVal( dSales );
	csv.AppendVal( dWastage );

	if ( Sysset.GetMaxStockPoints() > 1 )
		csv.AppendVal( dXferOut );

	csv.AppendVal( dAdjust );
	csv.AppendVal( dPending );
	csv.AppendVal( dApparent );
}

/**********************************************************************/