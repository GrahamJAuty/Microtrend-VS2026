/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewValuationClosing.h"
/**********************************************************************/

CStockTreeReportNewValuationClosing::CStockTreeReportNewValuationClosing( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_CLOSING_VALUE;
	m_nTotalisersPerLine = 9;
	m_bPositive = TRUE;
	m_bZero = TRUE;
	m_bNegative = TRUE;
	m_bZeroCost = TRUE;
}

/**********************************************************************/

void CStockTreeReportNewValuationClosing::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewValuationClosing::AddReportColumns()
{
	AddColumn ( "Closing", TA_RIGHT, 180, TRUE );
	
	switch ( StockOptions.GetStockMaxSuppliersPerItem() )
	{
	case 1:
		AddColumn ( "Cost", TA_RIGHT, 180, TRUE );
		break;

	case 2:
		AddColumn ( "Preferred", TA_RIGHT, 200, TRUE );
		AddColumn ( "Best", TA_RIGHT, 200, TRUE );
		AddColumn ( "Worst", TA_RIGHT, 200, TRUE );
		break;

	case 3:
		AddColumn ( "Preferred", TA_RIGHT, 200, TRUE );
		AddColumn ( "Best", TA_RIGHT, 200, TRUE );
		AddColumn ( "Medium", TA_RIGHT, 200, TRUE );
		AddColumn ( "Worst", TA_RIGHT, 200, TRUE );
		break;

	case 4:
	case 5:
	default:
		AddColumn ( "Preferred", TA_RIGHT, 200, TRUE );
		AddColumn ( "Best", TA_RIGHT, 200, TRUE );
		AddColumn ( "2nd Best", TA_RIGHT, 200, TRUE );
		AddColumn ( "3rd Best", TA_RIGHT, 200, TRUE );
		break;
	}

	AddColumn ( "Retail", TA_RIGHT, 180, TRUE );

	if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
		AddColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 200, FALSE );

	AddColumn ( EcrmanOptions.GetSalesTaxName(), TA_RIGHT, 180, TRUE );
	AddColumn ( EcrmanOptions.GetExTaxString(), TA_RIGHT, 180, TRUE );
	AddColumn ( "GP", TA_RIGHT, 180, TRUE );
	AddColumn ( "GP%", TA_RIGHT, 180, FALSE );
}

/**********************************************************************/

bool CStockTreeReportNewValuationClosing::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	if ( ( m_bPositive == FALSE ) || ( m_bZero == FALSE ) || ( m_bNegative == FALSE ) )
	{
		double dStockQty = StockLevels.GetStocktakeClosingQty();
	
		int nResult = CPriceHelpers::CompareDoubles( dStockQty, 0.0, 5 );

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

	if ( FALSE == m_bZeroCost )
	{
		bool bAccept = FALSE;
		for ( int n = 0; n < m_LabelStockRecord.GetSupplierCount(); n++ )
		{
			if ( m_LabelStockRecord.GetSuppNo(n) <= 0 )
				continue;

			if ( CPriceHelpers::CompareDoubles( m_LabelStockRecord.GetCost(n), 0.0, 3 ) != 0 )
			{
				bAccept = TRUE;
				break;
			}
		}

		if ( FALSE == bAccept )
			return FALSE;
	}

	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dClosing = StockLevels.GetStocktakeClosingQty();
	double dRetail = 0.0;
	double dTax = 0.0;
	double dExTax = 0.0;
	double dWaste = 0.0;
	double dGP = 0.0;
	
	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dClosing, nSubUnits );
	
	CArray<double,double> arrayCosts;
	GetSortedUnitCosts( arrayCosts, dClosing, 3 );

	int n = StockOptions.GetStockMaxSuppliersPerItem();
	if ( n >= 2 ) csv.AppendVal( arrayCosts[0] );
	if ( n >= 1 ) csv.AppendVal( arrayCosts[1] );
	if ( n >= 2 ) csv.AppendVal( arrayCosts[2] );
	if ( n >= 3 ) csv.AppendVal( arrayCosts[3] );
	
	bool bGotPluData = FALSE;

	if ( GetRetailPluInfo( TRUE ) == TRUE )
	{
		if ( m_dRetailStockQty != 0.0 )
		{
			dRetail = ( m_dRetailPrice1 * dClosing ) / m_dRetailStockQty;
			dExTax = ReportHelpers.CalcNonTax( dRetail, m_dRetailTaxRate );
			dTax = dRetail - dExTax;
			
			csv.AppendVal ( dRetail );

			if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
				csv.AppendString( GetRetailTaxBandString() );
			
			csv.AppendVal ( dTax );
			csv.AppendVal ( dExTax );
			
			double dCost = arrayCosts[0];
			dWaste = EstimateWastageFromSales( dCost );
			double dCostAndWaste = dCost + dWaste;

			dGP = dExTax - dCostAndWaste;
			double dGPPercent = CalculateGPPercent( dExTax, dCostAndWaste );

			csv.AppendVal ( dGP );
			csv.AppendVal ( dGPPercent, 2 );
			
			bGotPluData = TRUE;
		}
	}
	
	if ( bGotPluData == FALSE )
	{
		csv.AppendVal ( 0.0 );
		csv.AppendInt ( 0 );
		csv.AppendVal ( 0.0 );
		csv.AppendVal ( 0.0 );
		csv.AppendVal ( 0.0 );
		csv.AppendVal ( 0.0, 2 );
	}

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dClosing );
	AddToTotalisers( 1, arrayCosts[0] );
	AddToTotalisers( 2, arrayCosts[1] );
	AddToTotalisers( 3, arrayCosts[2] );
	AddToTotalisers( 4, arrayCosts[3] );
	AddToTotalisers( 5, dRetail );
	AddToTotalisers( 6, dTax );
	AddToTotalisers( 7, dExTax );
	AddToTotalisers( 8, dWaste );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewValuationClosing::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dClosing = GetTotaliser( nDepth, 0 );
	double dPrefCost = GetTotaliser( nDepth, 1 );
	double dCost1 = GetTotaliser( nDepth, 2 );
	double dCost2 = GetTotaliser( nDepth, 3 );
	double dCost3 = GetTotaliser( nDepth, 4 );
	double dRetail = GetTotaliser( nDepth, 5 );
	double dTax = GetTotaliser( nDepth, 6 );
	double dExTax = GetTotaliser( nDepth, 7 );
	double dWaste = GetTotaliser( nDepth, 8 );
	
	csv.AppendStockQty( dClosing );
	
	int n = StockOptions.GetStockMaxSuppliersPerItem();
	if ( n >= 2 ) csv.AppendVal( dPrefCost );
	if ( n >= 1 ) csv.AppendVal( dCost1 );
	if ( n >= 2 ) csv.AppendVal( dCost2 );
	if ( n >= 3 ) csv.AppendVal( dCost3 );

	csv.AppendVal ( dRetail );
	
	if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
		csv.AppendString ( "" );
	
	csv.AppendVal ( dTax );
	csv.AppendVal ( dExTax );

	double dCostAndWaste = dPrefCost + dWaste;
	double dGP = dExTax - dCostAndWaste;
	double dGPPercent = CalculateGPPercent( dExTax, dCostAndWaste );

	csv.AppendVal( dExTax - dCostAndWaste );
	csv.AppendVal( dGPPercent, 2 );
}

/**********************************************************************/