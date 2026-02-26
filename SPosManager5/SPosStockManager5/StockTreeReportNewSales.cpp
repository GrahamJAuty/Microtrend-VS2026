/**********************************************************************/
#include "ReportHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewSales.h"
/**********************************************************************/

CStockTreeReportNewSales::CStockTreeReportNewSales( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bZeroSkip = FALSE;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_SALES;
	m_nTotalisersPerLine = 9 + ( REPORT_EPOS_LEVELS * 2 );
}

/**********************************************************************/

void CStockTreeReportNewSales::PrepareReport( CStockReportSalesDlg* pDlg )
{
	m_strReportName = "Period ";
	m_strReportName += GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	//FIDDLE - SHOULD BE SALES_REPORT_BREAKDOWN
	m_bTotalSales = ( pDlg -> m_nSalesReportType != 1 );

	for ( int n = 0; n < 10; n++ )
		m_bEpos[n] = ( pDlg -> m_bEpos[n] != 0 );

	m_bManual = ( pDlg -> m_bManual != 0 );
	m_bImport = ( pDlg -> m_bImport != 0 );
	m_bRecipe = ( pDlg -> m_bRecipe != 0 );
	m_bAverage = ( pDlg -> m_bAverage != 0 );
	m_bTax = ( pDlg -> m_bTax != 0 );
	m_bZeroSkip = ( pDlg -> m_bZeroSkip != 0 );
	
	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewSales::AddReportColumns()
{			
	if ( m_bTotalSales == TRUE )
	{
		AddColumn( "Quantity", TA_RIGHT, 180, TRUE );		
		AddColumn( "Value", TA_RIGHT, 180, TRUE );
	}
	else
	{
		int nFieldCount = 0;

		for ( int nLevel = 0; nLevel < REPORT_EPOS_LEVELS; nLevel++ )
		{
			if ( m_bEpos [ nLevel ] == TRUE )
			{			
				CString strName;
				strName.Format ( "Epos%1.1d", nLevel + 1 ); 		
				AddColumn ( strName, TA_RIGHT, 200, TRUE );
				AddColumn( "Value", TA_RIGHT, 180, TRUE );
				nFieldCount++;
			}
		}

		if ( m_bManual == TRUE)
		{
			AddColumn( "Manual", TA_RIGHT, 180, TRUE );
			AddColumn( "Value", TA_RIGHT, 180, TRUE );
			nFieldCount++;
		}

		if ( m_bImport == TRUE )
		{
			AddColumn( "Import", TA_RIGHT, 180, TRUE );
			AddColumn( "Value", TA_RIGHT, 180, TRUE );
			nFieldCount++;
		}

		if ( m_bRecipe == TRUE )
		{
			AddColumn( "Recipe", TA_RIGHT, 180, TRUE );
			AddColumn( "Value", TA_RIGHT, 180, TRUE );
			nFieldCount++;
		}
	
		if ( nFieldCount >= 2 )
		{
			AddColumn( "Total", TA_RIGHT, 180, TRUE );
			AddColumn( "Value", TA_RIGHT, 180, TRUE );
		}
	}
	
	if ( m_bAverage == TRUE )
		AddColumn( "Average", TA_RIGHT, 180, TRUE );
		
	if ( m_bTax == TRUE )
		AddColumn( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 180, TRUE );
}
		
/**********************************************************************/

bool CStockTreeReportNewSales::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	OutputStockCodeFields( csv, m_bStockUnit );

	double dTotalQty = 0.0;
	double dTotalVal = 0.0;
	double dTax = 0.0;

	if ( m_bTotalSales == TRUE )
	{
		dTotalQty = StockLevels.GetTotalSalesQty();
		dTotalVal = StockLevels.GetTotalSalesValue();	
		csv.AppendStockQty( dTotalQty, nSubUnits );
		csv.AppendVal( dTotalVal );
	}
	else
	{
		int nFieldCount = 0;
		
		for ( int nLevel = 0; nLevel < REPORT_EPOS_LEVELS; nLevel++ )
		{
			if ( m_bEpos [ nLevel ] == TRUE )
			{	
				dTotalQty += StockLevels.GetEposSalesQty(nLevel);
				dTotalVal += StockLevels.GetEposSalesValue(nLevel);
				csv.AppendStockQty( StockLevels.GetEposSalesQty(nLevel), nSubUnits );
				csv.AppendVal( StockLevels.GetEposSalesValue(nLevel) );
				nFieldCount++;
			}
		}

		if ( m_bManual == TRUE)
		{
			dTotalQty += StockLevels.GetManualSalesQty();
			dTotalVal += StockLevels.GetManualSalesValue();	
			csv.AppendStockQty( StockLevels.GetManualSalesQty(), nSubUnits );
			csv.AppendVal( StockLevels.GetManualSalesValue() );
			nFieldCount++;
		}

		if ( m_bImport == TRUE )
		{
			dTotalQty += StockLevels.GetImportSalesQty();
			dTotalVal += StockLevels.GetImportSalesValue();
			csv.AppendStockQty( StockLevels.GetImportSalesQty(), nSubUnits );
			csv.AppendVal( StockLevels.GetImportSalesValue() );
			nFieldCount++;			
		}

		if ( m_bRecipe == TRUE )
		{
			dTotalQty += StockLevels.GetRecipeSalesQty();
			dTotalVal += StockLevels.GetRecipeSalesValue();
			csv.AppendStockQty( StockLevels.GetRecipeSalesQty(), nSubUnits );
			csv.AppendVal( StockLevels.GetRecipeSalesValue() );
			nFieldCount++;
		}
	
		if ( nFieldCount >= 2 )
		{
			csv.AppendStockQty( dTotalQty, nSubUnits );
			csv.AppendVal( dTotalVal );
		}
	}
	
	if ( ( dTotalQty == 0.0 ) && ( m_bZeroSkip == TRUE ) )
		return FALSE;

	if ( m_bAverage == TRUE )
	{
		double dAverage = CalculateAverage( dTotalVal, dTotalQty );
		csv.AppendVal( dAverage );
	}
	
	if ( m_bTax == TRUE )
	{
		if ( GetRetailPluInfo( FALSE ) == TRUE )
			dTax = dTotalVal - ReportHelpers.CalcNonTax( dTotalVal, m_dRetailTaxRate );
		
		csv.AppendVal( dTax);
	}
	
	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dTotalQty );
	AddToTotalisers( 1, dTotalVal );
	AddToTotalisers( 2, dTax );

	if ( m_bTotalSales == FALSE )
	{
		AddToTotalisers( 3, StockLevels.GetManualSalesQty() );
		AddToTotalisers( 4, StockLevels.GetManualSalesValue() );
		
		AddToTotalisers( 5, StockLevels.GetImportSalesQty() );
		AddToTotalisers( 6, StockLevels.GetImportSalesValue() );
		
		AddToTotalisers( 7, StockLevels.GetRecipeSalesQty() );
		AddToTotalisers( 8, StockLevels.GetRecipeSalesValue() );
		
		int nTot = 9;
		for ( int n = 0; n < REPORT_EPOS_LEVELS; n++ )
		{
			AddToTotalisers( nTot++, StockLevels.GetEposSalesQty(n) );
			AddToTotalisers( nTot++, StockLevels.GetEposSalesValue(n) );
		}
	}

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewSales::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dTotalQty = GetTotaliser( nDepth, 0 );
	double dTotalVal = GetTotaliser( nDepth, 1 );
	double dTax = GetTotaliser( nDepth, 2 );

	if ( m_bTotalSales == TRUE )
	{
		csv.AppendStockQty( dTotalQty );
		csv.AppendVal( dTotalVal );
	}
	else
	{
		int nFieldCount = 0;
		
		for ( int nLevel = 0; nLevel < REPORT_EPOS_LEVELS; nLevel++ )
		{
			if ( m_bEpos [ nLevel ] == TRUE )
			{	
				csv.AppendStockQty( GetTotaliser( nDepth, 9 + ( nLevel * 2 ) ) );
				csv.AppendVal( GetTotaliser( nDepth, 10 + ( nLevel * 2 ) ) );
				nFieldCount++;
			}
		}

		if ( m_bManual == TRUE)
		{
			csv.AppendStockQty( GetTotaliser( nDepth, 3 ) );
			csv.AppendVal( GetTotaliser( nDepth, 4 ) );
			nFieldCount++;
		}

		if ( m_bImport == TRUE )
		{
			csv.AppendStockQty( GetTotaliser( nDepth, 5 ) );
			csv.AppendVal( GetTotaliser( nDepth, 6 ) );
			nFieldCount++;			
		}

		if ( m_bRecipe == TRUE )
		{
			csv.AppendStockQty( GetTotaliser( nDepth, 7 ) );
			csv.AppendVal( GetTotaliser( nDepth, 8 ) );
			nFieldCount++;
		}
	
		if ( nFieldCount >= 2 )
		{
			csv.AppendStockQty( dTotalQty );
			csv.AppendVal( dTotalVal );
		}
	}
	
	if ( m_bAverage == TRUE )
	{
		double dAverage = CalculateAverage( dTotalVal, dTotalQty );
		csv.AppendVal( dAverage );
	}
	
	if ( m_bTax == TRUE )
		csv.AppendVal( dTax );
}

/**********************************************************************/
