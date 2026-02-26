/**********************************************************************/
 
/**********************************************************************/
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewCostSummary.h"
/**********************************************************************/

CStockTreeReportNewCostSummary::CStockTreeReportNewCostSummary( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_COST_SUMMARY;
	m_nTotalisersPerLine = 11;
}

/**********************************************************************/

void CStockTreeReportNewCostSummary::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewCostSummary::AddReportColumns()
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

bool CStockTreeReportNewCostSummary::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	double dUnitCost = m_LabelStockRecord.GetUnitCost(0);

	double dOpening = StockLevels.GetOpeningQty() * dUnitCost;
	double dDelivery = StockLevels.GetDeliveryQty() * dUnitCost;
	double dFree = StockLevels.GetFreeQty() * dUnitCost;
	double dReturns = StockLevels.GetReturnsQty() * dUnitCost;
	double dXferIn = StockLevels.GetTransferInQty() * dUnitCost;
	double dSales = StockLevels.GetTotalSalesQty() * dUnitCost;
	double dPending = StockLevels.GetPendingStocktakeQty() * dUnitCost;
	double dApparent = StockLevels.GetApparentQty() * dUnitCost;
	double dXferOut = StockLevels.GetTransferOutQty() * dUnitCost;
	double dAdjust = StockLevels.GetAdjustmentQty() * dUnitCost;
	
	double dWastage = StockLevels.GetEcrWastageQty();
	if ( m_bSubUnits == TRUE )	dWastage += EstimateWastageFromSales( dSales );
	dWastage *= dUnitCost;

	OutputStockCodeFields( csv, m_bStockUnit );

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

void CStockTreeReportNewCostSummary::CreateTotalsLine( CStockReportLine& csv, int nDepth )
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