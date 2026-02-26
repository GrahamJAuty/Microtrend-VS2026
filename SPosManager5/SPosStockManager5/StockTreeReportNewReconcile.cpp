/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewReconcile.h"
/**********************************************************************/

CStockTreeReportNewReconcile::CStockTreeReportNewReconcile( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_RECONCILE;
	m_nTotalisersPerLine = 11;
}

/**********************************************************************/

void CStockTreeReportNewReconcile::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewReconcile::AddReportColumns()
{
	AddColumn ( "Opening", TA_RIGHT, 180, TRUE );
	AddColumn ( "Goods In", TA_RIGHT, 190, TRUE );
	AddColumn ( "Returns", TA_RIGHT, 180, TRUE );

	if ( Sysset.GetMaxStockPoints() > 1 )
		AddColumn ( "Xfer Out", TA_RIGHT, 180, TRUE );
	
	AddColumn ( "Adjust.", TA_RIGHT, 180, TRUE );
	AddColumn ( "Closing", TA_RIGHT, 180, TRUE );

	if ( m_bSubUnits )
		AddColumn ( "Est. Waste", TA_RIGHT, 220, TRUE );
	else
		AddColumn ( "Ecr Waste", TA_RIGHT, 220, TRUE );

	AddColumn ( "Est. Qty", TA_RIGHT, 180, TRUE );
	AddColumn ( "Est. Val", TA_RIGHT, 180, TRUE);
	AddColumn ( "Sales Qty", TA_RIGHT, 200, TRUE );
	AddColumn ( "Sales Val", TA_RIGHT, 200, TRUE );
	AddColumn ( "GP%", TA_RIGHT, 200, FALSE );
	AddColumn ( "Var. Qty", TA_RIGHT, 180, TRUE );
	AddColumn ( "Var. Val", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewReconcile::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dOpening = StockLevels.GetOpeningQty();
	double dGoodsIn = StockLevels.GetStocktakeTotalGoodsInQty();
	double dReturns = StockLevels.GetStocktakeReturnsQty();
	double dXferOut = StockLevels.GetStocktakeTransferOutQty();
	double dAdjustment = StockLevels.GetStocktakeAdjustmentQty();
	double dClosing = StockLevels.GetStocktakeClosingQty();
	double dEstSalesQty = StockLevels.EstimateStocktakeSalesQty();
	double dWaste = StockLevels.GetStocktakeEcrWastageQty();

	if ( m_bSubUnits == TRUE )
	{
		double dAutoWaste = EstimateWastageFromSalesAndWastage( dEstSalesQty ); 
		dWaste += dAutoWaste ;
		dEstSalesQty -= dAutoWaste;
	}

	double dEstNonRecipeSalesQty = dEstSalesQty - StockLevels.GetStocktakeRecipeSalesQty();
	
	double dEstSalesValue = StockLevels.GetStocktakeRecipeSalesValue();
	dEstSalesValue += DataManager.Stock.CalculateRetailValue( m_LabelStockRecord, dEstNonRecipeSalesQty );

	double dTotalSalesQty = StockLevels.GetStocktakeTotalSalesQty();
	double dTotalSalesValue = StockLevels.GetStocktakeTotalSalesValue();

	double dGP = 0.0;
	if ( GetRetailPluInfo( TRUE ) == TRUE )
	{
		if ( CPriceHelpers::CompareDoubles( dTotalSalesQty, 0.0, 3 ) == 0 )
		{
			if ( DealerFlags.GetDorbiereImportFlag() == TRUE )
			{
				double dCostPerPLU = EstimateCostFromSUQty( m_dRetailStockQty );
				
				if ( TRUE == m_bSubUnits )
					dCostPerPLU += EstimateWastageFromSales( dCostPerPLU );
			
				double dTaxPerPLU = m_dRetailPrice1 - ReportHelpers.CalcNonTax( m_dRetailPrice1, m_dRetailTaxRate );
				dGP = CalculateGPPercent( m_dRetailPrice1 - dTaxPerPLU, dCostPerPLU );
			}
		}
		else
		{
			double dSalesCost = EstimateCostFromSUQty( dTotalSalesQty );
		
			if ( m_bSubUnits == TRUE )
				dSalesCost += EstimateWastageFromSales( dSalesCost );

			dSalesCost += EstimateCostFromSUQty( StockLevels.GetStocktakeEcrWastageQty() );
		
			double dEstTax = dTotalSalesValue - ReportHelpers.CalcNonTax( dTotalSalesValue, m_dRetailTaxRate );
			dGP = CalculateGPPercent( dTotalSalesValue - dEstTax, dSalesCost );
		}
	}

	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dOpening, nSubUnits );
	csv.AppendStockQty( dGoodsIn, nSubUnits );
	csv.AppendStockQty( dReturns, nSubUnits );

	if ( Sysset.GetMaxStockPoints() > 1 )
		csv.AppendStockQty( dXferOut, nSubUnits );

	csv.AppendStockQty( dAdjustment, nSubUnits );
	csv.AppendStockQty( dClosing, nSubUnits );
	csv.AppendStockQty( dWaste, nSubUnits );
	csv.AppendStockQty( dEstSalesQty, nSubUnits );
	csv.AppendVal( dEstSalesValue );
	csv.AppendStockQty( dTotalSalesQty, nSubUnits );
	csv.AppendVal( dTotalSalesValue );
	csv.AppendVal( dGP, 2 );
	csv.AppendStockQty( dTotalSalesQty - dEstSalesQty, nSubUnits );
	csv.AppendVal( dTotalSalesValue - dEstSalesValue );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dOpening );
	AddToTotalisers( 1, dGoodsIn );
	AddToTotalisers( 2, dReturns );
	AddToTotalisers( 3, dXferOut );
	AddToTotalisers( 4, dAdjustment );
	AddToTotalisers( 5, dClosing );
	AddToTotalisers( 6, dWaste );
	AddToTotalisers( 7, dEstSalesQty );
	AddToTotalisers( 8, dEstSalesValue );
	AddToTotalisers( 9, dTotalSalesQty );
	AddToTotalisers( 10, dTotalSalesValue );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewReconcile::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dOpening = GetTotaliser( nDepth, 0 );
	double dGoodsIn = GetTotaliser( nDepth, 1 );
	double dReturns = GetTotaliser( nDepth, 2 );
	double dXferOut = GetTotaliser( nDepth, 3 );
	double dAdjustment = GetTotaliser( nDepth, 4 );
	double dClosing = GetTotaliser( nDepth, 5 );
	double dWaste = GetTotaliser( nDepth, 6 );
	double dEstSalesQty = GetTotaliser( nDepth, 7 );
	double dEstSalesValue = GetTotaliser( nDepth, 8 );
	double dTotalSalesQty = GetTotaliser( nDepth, 9 );
	double dTotalSalesValue = GetTotaliser( nDepth, 10 );

	csv.AppendStockQty( dOpening );
	csv.AppendStockQty( dGoodsIn );
	csv.AppendStockQty( dReturns );

	if ( Sysset.GetMaxStockPoints() > 1 )
		csv.AppendStockQty( dXferOut );

	csv.AppendStockQty( dAdjustment );
	csv.AppendStockQty( dClosing );
	csv.AppendStockQty( dWaste );
	csv.AppendStockQty( dEstSalesQty );
	csv.AppendVal( dEstSalesValue );
	csv.AppendStockQty( dTotalSalesQty );
	csv.AppendVal( dTotalSalesValue );
	csv.AppendString( "" );
	csv.AppendStockQty( dTotalSalesQty - dEstSalesQty );
	csv.AppendVal( dTotalSalesValue - dEstSalesValue );
}

/**********************************************************************/