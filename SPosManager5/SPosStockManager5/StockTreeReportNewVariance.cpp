/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewVariance.h"
/**********************************************************************/

CStockTreeReportNewVariance::CStockTreeReportNewVariance( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bZeroSkip = FALSE;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_VARIANCE;
	m_nTotalisersPerLine = 19;
}

/**********************************************************************/

void CStockTreeReportNewVariance::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewVariance::AddReportColumns()
{
	AddColumn ( "Opening", TA_RIGHT, 180, FALSE );
	AddColumn ( "Goods In", TA_RIGHT, 190, FALSE );
	AddColumn ( "Returns", TA_RIGHT, 180, FALSE );

	if (Sysset.GetMaxStockPoints() > 1)
	{
		AddColumn("Xfer Out", TA_RIGHT, 180, FALSE);
	}

	AddColumn ( "Adjust.", TA_RIGHT, 180, FALSE );
	AddColumn ( "Sales", TA_RIGHT, 180, FALSE );
	AddColumn ( "Wastage", TA_RIGHT, 190, FALSE );
	AddColumn ( "Apparent", TA_RIGHT, 200, FALSE );
	AddColumn ( "Closing", TA_RIGHT, 180, FALSE );
	AddColumn ( "Cls. Cost", TA_RIGHT, 220, FALSE );
	AddColumn ( "Variance", TA_RIGHT, 180, TRUE );
	AddColumn ( "Var. Value", TA_RIGHT, 220, TRUE );
	AddColumn ( "Var. Cost", TA_RIGHT, 220, TRUE );
	AddColumn ( "Var. %", TA_RIGHT, 180, FALSE );
}

/**********************************************************************/

bool CStockTreeReportNewVariance::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dApparent = StockLevels.GetStocktakeApparentQty();
	double dClosing = StockLevels.GetStocktakeClosingQty();
	
	double dVarianceQty = dClosing - dApparent;
	if ((m_bZeroSkip == TRUE) && (dVarianceQty == 0.0))
	{
		return FALSE;
	}

	double dVarianceVal = 0.0;
	
	if ( GetRetailPluInfo( TRUE) == TRUE )
	{
		if (m_dRetailStockQty != 0.0)
		{
			dVarianceVal = ((m_dRetailPrice1 * dVarianceQty) / m_dRetailStockQty);
		}
	}

	double dUnitCost = m_LabelStockRecord.GetUnitCost(0);
	double dVarianceCost = dVarianceQty * dUnitCost ;
	double dClosingCost = dClosing * dUnitCost;

	double dOpening = StockLevels.GetOpeningQty();
	double dGoodsIn = StockLevels.GetStocktakeTotalGoodsInQty();
	double dReturns = StockLevels.GetStocktakeReturnsQty();
	double dXferOut = StockLevels.GetStocktakeTransferOutQty();
	double dSales = StockLevels.GetStocktakeTotalSalesQty();
	double dAdjustment = StockLevels.GetStocktakeAdjustmentQty();
	double dWastage = StockLevels.GetStocktakeEcrWastageQty();
	
	if (m_bSubUnits == TRUE)
	{
		dWastage += EstimateWastageFromSales(dSales);
	}

	double dVariancePercent = 0.0;
	double dPositiveGoods = dOpening + dGoodsIn;
	
	if (dPositiveGoods != 0.0)
	{
		dVariancePercent = (dVarianceQty * 100.0) / dPositiveGoods;
	}

	if (dVariancePercent < 0.0)
	{
		dVariancePercent = -dVariancePercent;
	}

	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dOpening, nSubUnits );
	csv.AppendStockQty( dGoodsIn, nSubUnits );
	csv.AppendStockQty( dReturns, nSubUnits );	

	if (Sysset.GetMaxStockPoints() > 1)
	{
		csv.AppendStockQty(dXferOut, nSubUnits);
	}

	csv.AppendStockQty( dAdjustment, nSubUnits );
	csv.AppendStockQty( dSales, nSubUnits );
	csv.AppendStockQty( dWastage, nSubUnits );
	csv.AppendStockQty( dApparent, nSubUnits );
	csv.AppendStockQty( dClosing, nSubUnits );
	csv.AppendVal( dClosingCost );
	csv.AppendStockQty( dVarianceQty, nSubUnits );
	csv.AppendVal( dVarianceVal );
	csv.AppendVal( dVarianceCost );
	csv.AppendVal( dVariancePercent, 2 );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	if ( dVarianceQty >= 0.0 )
	{
		AddToTotalisers( 0, dVarianceQty );
		AddToTotalisers( 1, dVarianceVal );
		AddToTotalisers( 16, dVarianceCost );
	}
	else
	{
		AddToTotalisers( 2, dVarianceQty );
		AddToTotalisers( 3, dVarianceVal );
		AddToTotalisers( 17, dVarianceCost );
	}

	AddToTotalisers( 4, dOpening );
	AddToTotalisers( 5, dGoodsIn );
	AddToTotalisers( 6, dReturns );
	AddToTotalisers( 7, dXferOut );
	AddToTotalisers( 8, dAdjustment );
	AddToTotalisers( 9, dSales );
	AddToTotalisers( 10, dWastage );
	AddToTotalisers( 11, dApparent );
	AddToTotalisers( 12, dClosing );
	AddToTotalisers( 13, dVarianceQty );
	AddToTotalisers( 14, dVarianceVal );
	AddToTotalisers( 15, dVarianceCost );
	AddToTotalisers( 18, dClosingCost );
	
	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewVariance::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	bool bSummary = m_StockLevelTree.GetSummaryFlag();
	bool bSummaryLine = ( bSummary && ( nDepth == m_arrayTotalFlags.GetSize() - 2 ) );
	
	if ( bSummaryLine == FALSE )
	{
		if (m_arrayTotalPos.GetSize() == 0)
		{
			return;
		}

		//csv line coming from base class already has text for total
		int nTotalPos = m_arrayTotalPos.GetAt(0);
		csv.AppendBlanks( nTotalPos - csv.GetSize() );
		csv.AppendStockQty( GetTotaliser( nDepth, 0 ) );
		csv.AppendVal( GetTotaliser( nDepth, 1 ) );
		csv.AppendVal( GetTotaliser( nDepth, 16 ) );
		WriteReportLine( csv.GetLine() );

		//base class will print this csv line and following blank 
		csv.RemoveAll();
		csv.AppendBlanks( nTotalPos );
		csv.AppendStockQty( GetTotaliser( nDepth, 2 ) );
		csv.AppendVal( GetTotaliser( nDepth, 3 ) );
		csv.AppendVal( GetTotaliser( nDepth, 17 ) );
	}
	else
	{
		double dOpening = GetTotaliser( nDepth, 4 );
		double dGoodsIn = GetTotaliser( nDepth, 5 );
		double dReturns = GetTotaliser( nDepth, 6 );
		double dXferOut = GetTotaliser( nDepth, 7 );
		double dAdjustment = GetTotaliser( nDepth, 8 );
		double dSales = GetTotaliser( nDepth, 9 );
		double dWastage = GetTotaliser( nDepth, 10 );
		double dApparent = GetTotaliser( nDepth, 11 );
		double dClosing = GetTotaliser( nDepth, 12 );
		double dVarianceQty = GetTotaliser( nDepth, 13 );
		double dVarianceVal = GetTotaliser( nDepth, 14 );
		double dVarianceCost = GetTotaliser( nDepth, 15 );
		double dClosingCost = GetTotaliser( nDepth, 18 );

		double dVariancePercent = 0.0;
		double dPositiveGoods = dOpening + dGoodsIn;
	
		if (dPositiveGoods != 0.0)
		{
			dVariancePercent = (dVarianceQty * 100.0) / dPositiveGoods;
		}

		if (dVariancePercent < 0.0)
		{
			dVariancePercent = -dVariancePercent;
		}

		csv.AppendStockQty( dOpening );
		csv.AppendStockQty( dGoodsIn );
		csv.AppendStockQty( dReturns );	

		if (Sysset.GetMaxStockPoints() > 1)
		{
			csv.AppendStockQty(dXferOut);
		}

		csv.AppendStockQty( dAdjustment );
		csv.AppendStockQty( dSales );
		csv.AppendStockQty( dWastage );
		csv.AppendStockQty( dApparent );
		csv.AppendStockQty( dClosing );
		csv.AppendStockQty( dClosingCost );
		csv.AppendStockQty( dVarianceQty );
		csv.AppendVal( dVarianceVal );
		csv.AppendVal( dVarianceCost );
		csv.AppendVal( dVariancePercent, 2 );
	}
}

/**********************************************************************/
