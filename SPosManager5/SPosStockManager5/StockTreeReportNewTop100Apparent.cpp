/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewTop100Apparent.h"
/**********************************************************************/

CStockTreeReportNewTop100Apparent::CStockTreeReportNewTop100Apparent( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_TOP_100_APPARENT;
	m_nTotalisersPerLine = 0;
}

/**********************************************************************/

void CStockTreeReportNewTop100Apparent::PrepareReport()
{
	CString strTopBottom = m_chartinfo.IsBottom100() ? "Bottom" : "Top";		
	CString strQtyVal = m_chartinfo.IsSortedByValue() ? "Value" : "Quantity";

	CString strChartSize = "";
	if ( m_chartinfo.GetChartType() == CHART_TYPE_MAXITEMS ) 
		strChartSize.Format ( " %d", m_chartinfo.GetChartSize() );

	m_strReportName.Format ( "%s%s Apparent Stock by %s report for %s",
		(const char*) strTopBottom,
		(const char*) strChartSize,
		(const char*) strQtyVal,
		(const char*) m_strConLevelText );
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddColumn ( "Pos", TA_LEFT, 150, FALSE );
	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewTop100Apparent::AddReportColumns()
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
	AddColumn ( "Qty", TA_RIGHT, 180, TRUE );
	AddColumn ( "Value", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

void CStockTreeReportNewTop100Apparent::CreateReportLine( CStockLevelsCSVRecord& StockLevels )
{
	//APPARENT STOCK NEEDS THIS IN ORDER TO WORK CORRECTLY
	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
		StockLevels.SetAutoWastagePercent( m_LabelStockRecord.GetWastePercent() );
	else
		StockLevels.SetAutoWastagePercent( 0 );
	
	CApparentStockInfoNew info;

	info.m_nStockIdx = m_nLabelStockIdx;
	info.m_dOpening = StockLevels.GetOpeningQty();
	info.m_dDelivery = StockLevels.GetDeliveryQty();
	info.m_dFree = StockLevels.GetFreeQty();
	info.m_dReturns = StockLevels.GetReturnsQty();
	info.m_dTransferIn = StockLevels.GetTransferInQty();
	info.m_dSales = StockLevels.GetTotalSalesQty();
	info.m_dStocktake = StockLevels.GetPendingStocktakeQty();
	info.m_dTransferOut = StockLevels.GetTransferOutQty();
	info.m_dAdjustment = StockLevels.GetAdjustmentQty();
	
	info.m_dWastage = StockLevels.GetEcrWastageQty();
	if ( m_bSubUnits == TRUE ) info.m_dWastage += EstimateWastageFromSales( info.m_dSales );
		
	info.m_dTotalQty = StockLevels.GetApparentQty();

	CArray<double,double> arrayCosts;
	GetSortedUnitCosts( arrayCosts, info.m_dTotalQty, 1 );
	info.m_dTotalValue = arrayCosts.GetAt(0);
	
	int nInsertPoint = GetChartInsertPos( info );

	if ( m_chartinfo.GetChartType() != CHART_TYPE_MAXITEMS )
		m_StockLevelChart.InsertAt( nInsertPoint, info );
	else
	{
		if ( nInsertPoint < m_chartinfo.GetChartSize() )
			m_StockLevelChart.InsertAt( nInsertPoint, info );

		if ( m_StockLevelChart.GetSize() > m_chartinfo.GetChartSize() )
			m_StockLevelChart.RemoveAt( m_StockLevelChart.GetSize() - 1 );
	}
}

/**********************************************************************/

int CStockTreeReportNewTop100Apparent::GetChartInsertPos( CApparentStockInfoNew& info )
{
	int nStartIdx = 0;
	int nEndIdx = m_StockLevelChart.GetSize() - 1;

	double dToInsert = 0.0;

	if ( m_chartinfo.IsSortedByValue() )
		dToInsert = info.m_dTotalValue;
	else
		dToInsert = info.m_dTotalQty;

	//ADD VERY SMALL FRACTION SO THAT ITEMS WITH SAME VALUE TO STOCKMAN
	//PRECISION WILL BE STORED IN ASCENDING STOCK CODE ORDER
	if ( m_chartinfo.IsBottom100() )
	{
		dToInsert = -dToInsert;
		dToInsert -= 0.0000001;
	}
	else
		dToInsert += 0.0000001;

	while ( nStartIdx <= nEndIdx )
	{
		int nArrayIdx = ( nStartIdx + nEndIdx ) / 2;
		CApparentStockInfoNew arrayInfo = m_StockLevelChart.GetAt( nArrayIdx );
		
		double dToCompare = 0.0;
		if ( m_chartinfo.IsSortedByValue() )
			dToCompare = arrayInfo.m_dTotalValue;
		else
			dToCompare = arrayInfo.m_dTotalQty;

		if ( m_chartinfo.IsBottom100() )
			dToCompare = -dToCompare;
	
		if ( dToInsert > dToCompare )
		{
			nEndIdx = nArrayIdx - 1;
			continue;
		}

		if ( dToInsert < dToCompare )
		{
			nStartIdx = nArrayIdx + 1;
			continue;
		}

		if ( dToInsert == dToCompare )
			return nArrayIdx;
	}
	return nStartIdx;	
}

/**********************************************************************/

void CStockTreeReportNewTop100Apparent::CreateTotalsLine( int nDepth )
{
	for ( int n = 0; n < m_StockLevelChart.GetSize(); n++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_nProgressTarget );

		CApparentStockInfoNew lineinfo = m_StockLevelChart.GetAt( n );
	
		if ( m_chartinfo.GetChartType() == CHART_TYPE_CUTOFF )
		{
			double dThisLine;
			
			if ( m_chartinfo.IsSortedByValue() ) 
				dThisLine = lineinfo.m_dTotalValue;
			else
				dThisLine = lineinfo.m_dTotalQty;
				
			double dCutOff = m_chartinfo.GetChartCutOff();

			if ( m_chartinfo.IsBottom100() )
			{
				dThisLine = -dThisLine;
				dCutOff = -dCutOff;
			}

			if ( dCutOff > dThisLine )
				break;
		}

		m_nLabelStockIdx = lineinfo.m_nStockIdx;
		DataManager.Stock.GetAt( m_nLabelStockIdx, m_LabelStockRecord );
		int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

		CStockReportLine csvOut;
		csvOut.AppendInt( n + 1 );
		OutputStockCodeFields( csvOut, m_bStockUnit );
		
		csvOut.AppendStockQty( lineinfo.m_dOpening, m_bSubUnits );
		csvOut.AppendStockQty( lineinfo.m_dDelivery, m_bSubUnits );
		csvOut.AppendStockQty( lineinfo.m_dFree, m_bSubUnits );
		csvOut.AppendStockQty( lineinfo.m_dReturns, m_bSubUnits );

		if ( Sysset.GetMaxStockPoints() > 1 )
			csvOut.AppendStockQty( lineinfo.m_dTransferIn, m_bSubUnits );

		csvOut.AppendStockQty( lineinfo.m_dSales, m_bSubUnits );
		csvOut.AppendStockQty( lineinfo.m_dWastage, m_bSubUnits );

		if ( Sysset.GetMaxStockPoints() > 1 )
			csvOut.AppendStockQty( lineinfo.m_dTransferOut, m_bSubUnits );

		csvOut.AppendStockQty( lineinfo.m_dAdjustment, m_bSubUnits );
		csvOut.AppendStockQty( lineinfo.m_dStocktake, m_bSubUnits );
		csvOut.AppendStockQty( lineinfo.m_dTotalQty, m_bSubUnits );
		csvOut.AppendVal( lineinfo.m_dTotalValue );

		OutputSupplierFields( csvOut, m_bSupplierName, m_bSupplierRef );

		WriteReportLine( csvOut.GetLine() );
	}

	WriteBlankLine( TRUE );
	m_StockLevelChart.RemoveAll();
}

/**********************************************************************/

