/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewValuationApparent.h"
/**********************************************************************/

CStockTreeReportNewValuationApparent::CStockTreeReportNewValuationApparent( bool bOverstock, CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bOverstock = bOverstock;

	if ( TRUE == m_bOverstock )
		m_nReportParamsType = STOCK_OVERSTOCK_VALUE;
	else
		m_nReportParamsType = STOCK_APPARENT_VALUE;
	
	m_nTotalisersPerLine = 9;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;

	m_bHideAboveMin = FALSE;
	m_bPositive = TRUE;
	m_bZero = TRUE;
	m_bNegative = TRUE;
	m_bZeroCost = TRUE;
}

/**********************************************************************/

void CStockTreeReportNewValuationApparent::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewValuationApparent::AddReportColumns()
{
	AddColumn ( "Apparent", TA_RIGHT, 200, TRUE );

	if ( TRUE == m_bOverstock )
	{
		AddColumn ( "Maximum", TA_RIGHT, 200, FALSE );
		AddColumn ( "Overstock", TA_RIGHT, 200, TRUE );
	}
	
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
}

/**********************************************************************/

bool CStockTreeReportNewValuationApparent::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
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

	double dMaximum = 0.0;
	double dOverstock = 0.0;

	if ( TRUE == m_bOverstock )
	{
		dMaximum = StockLevels.GetMaximumLevelQty();
		if ( ( dMaximum <= 0.0 )|| ( dApparentQty <= dMaximum ) )
			return FALSE;

		dOverstock = dApparentQty - dMaximum;
	}

	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dRetail = 0.0;
	double dTax = 0.0;
	double dExTax = 0.0;
	double dWaste = 0.0;
	
	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dApparentQty, nSubUnits );

	if ( TRUE == m_bOverstock )
	{
		csv.AppendStockQty( dMaximum, nSubUnits );
		csv.AppendStockQty( dOverstock, nSubUnits );
	}

	CArray<double,double> arrayCosts;
	double dQtyToValue = m_bOverstock ? dOverstock : dApparentQty;
	GetSortedUnitCosts( arrayCosts, dQtyToValue, 3 );

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
			dRetail = ( m_dRetailPrice1 * dQtyToValue ) / m_dRetailStockQty;
			dExTax = ReportHelpers.CalcNonTax( dRetail, m_dRetailTaxRate );
			dTax = dRetail - dExTax;
			
			csv.AppendVal ( dRetail );

			if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
				csv.AppendString( GetRetailTaxBandString() );
			
			csv.AppendVal ( dTax );
			csv.AppendVal ( dExTax );
			
			bGotPluData = TRUE;
		}
	}
	
	if ( bGotPluData == FALSE )
	{
		csv.AppendVal ( 0.0 );
		csv.AppendInt ( 0 );
		csv.AppendVal ( 0.0 );
		csv.AppendVal ( 0.0 );
	}

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dApparentQty );
	AddToTotalisers( 1, arrayCosts[0] );
	AddToTotalisers( 2, arrayCosts[1] );
	AddToTotalisers( 3, arrayCosts[2] );
	AddToTotalisers( 4, arrayCosts[3] );
	AddToTotalisers( 5, dRetail );
	AddToTotalisers( 6, dTax );
	AddToTotalisers( 7, dExTax );
	AddToTotalisers( 8, dOverstock );
	
	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewValuationApparent::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dApparent = GetTotaliser( nDepth, 0 );
	double dPrefCost = GetTotaliser( nDepth, 1 );
	double dCost1 = GetTotaliser( nDepth, 2 );
	double dCost2 = GetTotaliser( nDepth, 3 );
	double dCost3 = GetTotaliser( nDepth, 4 );
	double dRetail = GetTotaliser( nDepth, 5 );
	double dTax = GetTotaliser( nDepth, 6 );
	double dExTax = GetTotaliser( nDepth, 7 );
	double dOverstock = GetTotaliser( nDepth, 8 );

	csv.AppendStockQty( dApparent );
	
	if ( TRUE == m_bOverstock )
	{
		csv.AppendString( "" );
		csv.AppendStockQty( dOverstock );
	}

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
}

/**********************************************************************/