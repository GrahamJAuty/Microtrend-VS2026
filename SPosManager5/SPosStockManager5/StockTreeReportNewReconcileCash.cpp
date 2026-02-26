/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewReconcileCash.h"
/**********************************************************************/

CStockTreeReportNewReconcileCash::CStockTreeReportNewReconcileCash( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_RECONCILE_CASH;
	m_nTotalisersPerLine = 3;
	m_bBlankBeforeHeader = FALSE;
}

/**********************************************************************/

void CStockTreeReportNewReconcileCash::PrepareArrays()
{
	CStockTreeReportNew::PrepareArrays();
	
	for ( int n = 0; n < m_arrayTotalFlags.GetSize(); n++ )
		m_arrayTotalFlags.SetAt( n, 0 );

	for ( int n = 0; n < m_arrayHeaderFlags.GetSize(); n++ )
		m_arrayHeaderFlags.SetAt( n, 0 );

	if ( m_arrayReportFields.GetSize() == 2 )
		m_arrayHeaderFlags.SetAt( 1, 1 );
	
	m_arrayTotalFlags.SetAt( 0, 1 );
}

/**********************************************************************/

void CStockTreeReportNewReconcileCash::SetAllowanceValue ( int nIndex, double dValue )
{
	if ( nIndex >= 1 && nIndex <= MAX_ALLOWANCES )
		m_dAllowanceArray [ nIndex ] = dValue;
}

/**********************************************************************/

void CStockTreeReportNewReconcileCash::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddReportColumns();
}

/**********************************************************************/

void CStockTreeReportNewReconcileCash::AddReportColumns()
{
	AddColumn ( "", TA_LEFT, 600, FALSE );
	AddColumn ( "", TA_RIGHT, 250, FALSE );
	AddColumn ( "", TA_LEFT, 100, FALSE );		//gap between columns
	AddColumn ( "", TA_LEFT, 600, FALSE );
	AddColumn ( "", TA_RIGHT, 250, FALSE );
}

/**********************************************************************/

bool CStockTreeReportNewReconcileCash::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	double dEstSalesAndWasteQty = StockLevels.EstimateStocktakeSalesQty();	
	double dEstCost = EstimateCostFromSUQty( dEstSalesAndWasteQty );
	double dEstWasteQty = EstimateWastageFromSalesAndWastage( dEstSalesAndWasteQty );
	double dEstSalesQty = dEstSalesAndWasteQty - dEstWasteQty;
	double dEstRetail = StockLevels.GetStocktakeRecipeSalesValue();

	if ( GetRetailPluInfo( TRUE ) == TRUE )
	{
		if ( m_dRetailStockQty != 0.0 )
		{
			double dEstNonRecipeSalesQty = dEstSalesQty - StockLevels.GetStocktakeRecipeSalesQty();
			dEstRetail += ( ( m_dRetailPrice1 * dEstNonRecipeSalesQty ) / m_dRetailStockQty );
		}
	}

	int nCategory = m_LabelStockRecord.GetCategory();
	
	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( DataManager.Category.GetTaxBand( nCategory ), TaxRateInfo );

	double dEstTax = dEstRetail - ReportHelpers.CalcNonTax( dEstRetail, TaxRateInfo.m_dTaxRate );

	AddToTotalisers( 0, dEstCost );
	AddToTotalisers( 1, dEstRetail );
	AddToTotalisers( 2, dEstTax );

	return FALSE;
}

/**********************************************************************/

void CStockTreeReportNewReconcileCash::CreateTotalsLine( int nDepth )
{
	double dEstCost = GetTotaliser( nDepth, 0 );
	double dEstRetail = GetTotaliser( nDepth, 1 );
	double dEstTax = GetTotaliser( nDepth, 2 );

	CStockReportLine csv;
	csv.AppendString ( "Est. Takings" );
	csv.AppendVal ( dEstRetail );
	csv.AppendBlanks ( 1 );
	csv.AppendString ( "Cash Takings" );
	csv.AppendVal ( m_dCashTakings );
	WriteReportLine ( csv.GetLine() );
		
	csv.RemoveAll();
	csv.AppendString ( EcrmanOptions.GetTaxContentString() );
	csv.AppendVal ( dEstTax );
	csv.AppendBlanks ( 1 );	
	csv.AppendString ( EcrmanOptions.GetTaxContentString() );
	csv.AppendVal ( m_dCashTakingsTax );
	WriteReportLine ( csv.GetLine() );
	
	WriteBlankLine( FALSE );	
		
	double dTotalAllowance = 0.0;
	for ( int nIndex = 1; nIndex <= MAX_ALLOWANCES; nIndex++ )
	{
		if ( DataManager.Allowance.GetReportEnable ( nIndex ) == TRUE )
		{
			double dAllowance = m_dAllowanceArray [ nIndex ];
				
			csv.RemoveAll();
			CString strAllowance = "Less ";
			strAllowance += DataManager.Allowance.GetText ( nIndex );

			csv.AppendString ( strAllowance );
			csv.AppendVal ( dAllowance );
			WriteReportLine ( csv.GetLine() );
			dTotalAllowance += dAllowance;
		}
	}

	csv.RemoveAll();
	csv.AppendString ( "Total Allowances" );
	csv.AppendVal ( dTotalAllowance );
	WriteReportLine ( csv.GetLine() );
	
	csv.RemoveAll();
	csv.AppendString ( EcrmanOptions.GetTaxContentString() );
	csv.AppendVal ( m_dAllowanceTax );
	WriteReportLine ( csv.GetLine() );
	
	WriteBlankLine( FALSE );

	double dNewEstTakings = dEstRetail - dTotalAllowance; 
	double dNewTaxContent = dEstTax - m_dAllowanceTax; 
	
	csv.RemoveAll();
	csv.AppendString ( "Adjusted Est. Takings" );
	csv.AppendVal ( dNewEstTakings );
	WriteReportLine ( csv.GetLine() );
		
	csv.RemoveAll();
	csv.AppendString ( EcrmanOptions.GetTaxString ( "Adjusted %T content" ) );
	csv.AppendVal ( dNewTaxContent );
	WriteReportLine ( csv.GetLine() );
		
	double dExTax1 = dNewEstTakings - dNewTaxContent;
	double dExTax2 = m_dCashTakings - m_dCashTakingsTax;

	csv.RemoveAll();
	csv.AppendString ( EcrmanOptions.GetTaxString( "Est. Takings (ex %T)" ) );
	csv.AppendVal ( dExTax1 );
	csv.AppendBlanks ( 1 );							//gap between columns
	csv.AppendString ( EcrmanOptions.GetTaxString( "Cash Takings (ex %T)" ) );
	csv.AppendVal ( dExTax2 );
	WriteReportLine ( csv.GetLine() );
		
	csv.RemoveAll();
	csv.AppendString ( "Less Actual Cost" );
	csv.AppendVal ( dEstCost );
	csv.AppendBlanks ( 1 );							//gap between columns
	csv.AppendString ( "Less Actual Cost" );
	csv.AppendVal ( dEstCost );
	WriteReportLine ( csv.GetLine() );
	
	double dGrossProfit1 = dNewEstTakings - dNewTaxContent - dEstCost;
	double dGrossProfit2 = m_dCashTakings - m_dCashTakingsTax - dEstCost;

	csv.RemoveAll();
	csv.AppendString ( "Expected Gross Profit" );
	csv.AppendVal ( dGrossProfit1 );
	csv.AppendBlanks ( 1 );							//gap between columns
	csv.AppendString ( "Actual Gross Profit" );
	csv.AppendVal ( dGrossProfit2 );
	WriteReportLine ( csv.GetLine() );
		
	double dGPPercent1 = CalculateGPPercent( dExTax1, dEstCost );
	double dGPPercent2 = CalculateGPPercent( dExTax2, dEstCost );
	
	csv.RemoveAll();
	csv.AppendString ( "Expected Gross Profit %" );
	csv.AppendVal ( dGPPercent1 );
	csv.AppendBlanks ( 1 );							//gap between columns
	csv.AppendString ( "Actual Gross Profit %" );
	csv.AppendVal ( dGPPercent2 );
	WriteReportLine ( csv.GetLine() );
		
	WriteBlankLine( FALSE );

	csv.RemoveAll();
	csv.AppendBlanks ( 3 );
	csv.AppendString ( "Cash Takings" );
	csv.AppendVal ( m_dCashTakings );
	WriteReportLine ( csv.GetLine() );
		
	csv.RemoveAll();
	csv.AppendBlanks( 3 );
	csv.AppendString ( "Estimated Cash" );
	csv.AppendVal ( dNewEstTakings );
	WriteReportLine ( csv.GetLine() );
		
	double dResult = m_dCashTakings - dNewEstTakings;
	CString strResult = "Net Result ";

	if ( dResult > 0.0 )
		strResult += "(SURPLUS)";

	if ( dResult <= 0.0 )
	{
		strResult += "(DEFICIT)";
		dResult = -dResult;
	}

	csv.RemoveAll();
	csv.AppendBlanks ( 3 );
	csv.AppendString ( strResult );
	csv.AppendVal ( dResult );
	WriteReportLine ( csv.GetLine() );

	double dPercent = 0.0;

	if ( m_dCashTakings != 0.0 )
		dPercent = ( dResult / m_dCashTakings ) * 100.0;
			
	csv.RemoveAll();
	csv.AppendBlanks ( 3 );	
	csv.AppendString ( "Result % Actual Takings" );
	csv.AppendVal ( dPercent );
	WriteReportLine ( csv.GetLine() );
		
	WriteBlankLine( FALSE );
}

/**********************************************************************/