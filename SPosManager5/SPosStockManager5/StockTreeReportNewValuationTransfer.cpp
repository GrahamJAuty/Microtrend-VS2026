/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewValuationTransfer.h"
/**********************************************************************/

CStockTreeReportNewValuationTransfer::CStockTreeReportNewValuationTransfer( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_TRANSFER_VALUE;
	m_nTotalisersPerLine = 10;
	m_bPositive = TRUE;
	m_bZero = TRUE;
	m_bNegative = TRUE;
	m_bZeroCost = TRUE;
}

/**********************************************************************/

void CStockTreeReportNewValuationTransfer::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewValuationTransfer::AddReportColumns()
{
	AddColumn ( "Xfer In", TA_RIGHT, 180, TRUE );
	AddColumn ( "Xfer Out", TA_RIGHT, 180, TRUE );	
	AddColumn ( "Net Xfer In", TA_RIGHT, 220, TRUE );	
	
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

bool CStockTreeReportNewValuationTransfer::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	if ( ( m_bPositive == FALSE ) || ( m_bZero == FALSE ) || ( m_bNegative == FALSE ) )
	{
		double dStockQty = StockLevels.GetTransferInQty() - StockLevels.GetTransferOutQty();
	
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

	double dXferIn = StockLevels.GetTransferInQty();
	double dXferOut = StockLevels.GetTransferOutQty();
	double dNetXferIn = dXferIn - dXferOut;

	if ( ( dXferIn == 0.0 ) && ( dXferOut == 0.0 ) )
		return FALSE;

	double dRetail = 0.0;
	double dTax = 0.0;
	double dExTax = 0.0;
	double dWaste = 0.0;
	double dGP = 0.0;
	
	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dXferIn, nSubUnits );
	csv.AppendStockQty( dXferOut, nSubUnits );
	csv.AppendStockQty( dNetXferIn, nSubUnits );
		
	CArray<double,double> arrayCosts;
	GetSortedUnitCosts( arrayCosts, dNetXferIn, 3 );

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
			dRetail = ( m_dRetailPrice1 * dNetXferIn ) / m_dRetailStockQty;
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

	AddToTotalisers( 0, dXferIn );
	AddToTotalisers( 1, dXferOut );
	AddToTotalisers( 2, arrayCosts[0] );
	AddToTotalisers( 3, arrayCosts[1] );
	AddToTotalisers( 4, arrayCosts[2] );
	AddToTotalisers( 5, arrayCosts[3] );
	AddToTotalisers( 6, dRetail );
	AddToTotalisers( 7, dTax );
	AddToTotalisers( 8, dExTax );
	AddToTotalisers( 9, dWaste );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewValuationTransfer::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dXferIn = GetTotaliser( nDepth, 0 );
	double dXferOut = GetTotaliser( nDepth, 1 );
	double dNetXferIn = dXferIn - dXferOut;
	double dPrefCost = GetTotaliser( nDepth, 2 );
	double dCost1 = GetTotaliser( nDepth, 3 );
	double dCost2 = GetTotaliser( nDepth, 4 );
	double dCost3 = GetTotaliser( nDepth, 5 );
	double dRetail = GetTotaliser( nDepth, 6 );
	double dTax = GetTotaliser( nDepth, 7 );
	double dExTax = GetTotaliser( nDepth, 8 );
	double dWaste = GetTotaliser( nDepth, 9 );
		
	csv.AppendStockQty( dXferIn );
	csv.AppendStockQty( dXferOut );
	csv.AppendStockQty( dNetXferIn );
		
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