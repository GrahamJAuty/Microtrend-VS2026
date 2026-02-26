/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "ReportHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewReconcileCategory.h"
/**********************************************************************/

CStockTreeReportNewReconcileCategory::CStockTreeReportNewReconcileCategory( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_RECONCILE_CATEGORY;
	m_nTotalisersPerLine = 8;
}

/**********************************************************************/

void CStockTreeReportNewReconcileCategory::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( FALSE );
	AddReportColumns();
}

/**********************************************************************/

void CStockTreeReportNewReconcileCategory::AddReportColumns()
{
	AddColumn ( "Est. Cost", TA_RIGHT, 190, TRUE );
	AddColumn ( "Est. Retail", TA_RIGHT, 220, TRUE );
	AddColumn ( EcrmanOptions.GetTaxString( "Est. R. %T" ), TA_RIGHT, 220, TRUE );
	AddColumn ( "Sales Cost", TA_RIGHT, 220, TRUE );
	AddColumn ( "Sales Val", TA_RIGHT, 200, TRUE );
	AddColumn ( EcrmanOptions.GetTaxString( "Sales %T" ), TA_RIGHT, 200, TRUE );
	AddColumn ( "Close Cost", TA_RIGHT, 230, TRUE );
	AddColumn ( "Close Retail", TA_RIGHT, 240, TRUE );
	AddColumn ( "GP%", TA_RIGHT, 180, FALSE );
}

/**********************************************************************/

bool CStockTreeReportNewReconcileCategory::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	double dEstSalesAndWasteQty = StockLevels.EstimateStocktakeSalesQty();
	double dEstCost = EstimateCostFromSUQty( dEstSalesAndWasteQty );

	double dEstWasteQty = 0.0;
	if ( m_bSubUnits == TRUE )
		dEstWasteQty = EstimateWastageFromSalesAndWastage( dEstSalesAndWasteQty );
		
	double dClosingQty = StockLevels.GetStocktakeClosingQty();
	double dClosingEstWaste = EstimateWastageFromSales( dClosingQty );
	double dClosingCost = EstimateCostFromSUQty( dClosingQty + dClosingEstWaste );
	
	double dEstRetail = StockLevels.GetStocktakeRecipeSalesValue();
	double dClosingRetail = 0.0;

	if ( GetRetailPluInfo( TRUE ) == TRUE )
	{
		if ( m_dRetailStockQty != 0.0 )
		{
			double dEstSalesQty = dEstSalesAndWasteQty - dEstWasteQty;
			double dEstNonRecipeSalesQty = dEstSalesQty - StockLevels.GetStocktakeRecipeSalesQty();
			dEstRetail += ( ( m_dRetailPrice1 * dEstNonRecipeSalesQty ) / m_dRetailStockQty );
			dClosingRetail += ( ( m_dRetailPrice1 * dClosingQty ) / m_dRetailStockQty );
		}
	}

	int nCat = m_LabelStockRecord.GetCategory();
	
	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( DataManager.Category.GetTaxBand( nCat ), TaxRateInfo );

	double dEstTax = dEstRetail - ReportHelpers.CalcNonTax( dEstRetail, TaxRateInfo.m_dTaxRate );

	double dSalesQty = StockLevels.GetStocktakeTotalSalesQty();
	double dSalesEstWaste = EstimateWastageFromSales( dSalesQty );
	double dSalesCost = EstimateCostFromSUQty( dSalesQty + dSalesEstWaste );
	double dSalesRetail = StockLevels.GetStocktakeTotalSalesValue();
	double dSalesTax = dSalesRetail - ReportHelpers.CalcNonTax( dSalesRetail, TaxRateInfo.m_dTaxRate );

	AddToTotalisers( 0, dEstCost );
	AddToTotalisers( 1, dEstRetail );
	AddToTotalisers( 2, dEstTax );
	AddToTotalisers( 3, dSalesCost );
	AddToTotalisers( 4, dSalesRetail );
	AddToTotalisers( 5, dSalesTax );
	AddToTotalisers( 6, dClosingCost );
	AddToTotalisers( 7, dClosingRetail );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewReconcileCategory::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dEstCost = GetTotaliser( nDepth, 0 );
	double dEstRetail = GetTotaliser( nDepth, 1 );
	double dEstTax = GetTotaliser( nDepth, 2 );
	double dSalesCost = GetTotaliser( nDepth, 3 );
	double dSalesRetail = GetTotaliser( nDepth, 4 );
	double dSalesTax = GetTotaliser( nDepth, 5 );
	double dClosingCost = GetTotaliser( nDepth, 6 );
	double dClosingRetail = GetTotaliser( nDepth, 7 );

	double dExTax = dSalesRetail - dSalesTax;
	double dGPPercent = CalculateGPPercent( dExTax, dEstCost );

	csv.AppendVal( dEstCost );
	csv.AppendVal( dEstRetail );
	csv.AppendVal( dEstTax );
	csv.AppendVal( dSalesCost );
	csv.AppendVal( dSalesRetail );
	csv.AppendVal( dSalesTax );
	csv.AppendVal( dClosingCost );
	csv.AppendVal( dClosingRetail );
	csv.AppendVal( dGPPercent, 2 );
}

/**********************************************************************/