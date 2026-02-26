/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StatusProgress.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewTop100Sales.h"
/**********************************************************************/

CStockTreeReportNewTop100Sales::CStockTreeReportNewTop100Sales( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_TOP_100_SALES;
	m_nTotalisersPerLine = 0;
	m_nTimeType = REPORT_PERIOD;
}

/**********************************************************************/

void CStockTreeReportNewTop100Sales::PrepareReport()
{
	CString strTopBottom = m_chartinfo.IsBottom100() ? "Bottom" : "Top";
	CString strQtyVal = m_chartinfo.IsSortedByValue() ? "Value" : "Quantity";

	CString strPeriodYtd;
	switch ( m_nTimeType )
	{
	case REPORT_YTD:				strPeriodYtd = "YTD";			break;
	case REPORT_YTD_PLUS_PERIOD:		strPeriodYtd = "YTD + Period";	break;
	case REPORT_PERIOD:
	default:						strPeriodYtd = "Period";			break;
	}
		
	CString strChartSize = "";
	if ( m_chartinfo.GetChartType() == CHART_TYPE_MAXITEMS ) 
		strChartSize.Format ( " %d", m_chartinfo.GetChartSize() );

	m_strReportName.Format ( "%s%s %s stock sellers by %s report for %s",
		(const char*) strTopBottom,
		(const char*) strChartSize,
		(const char*) strPeriodYtd,
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

void CStockTreeReportNewTop100Sales::AddReportColumns()
{
	AddColumn ( "Sales Qty", TA_RIGHT, 200, FALSE );
	AddColumn ( "Sales Val", TA_RIGHT, 200, FALSE );
}

/**********************************************************************/

void CStockTreeReportNewTop100Sales::CreateReportLine( CStockLevelsCSVRecord& StockLevels )
{
	CPeriodChartItemNew item;
	item.m_nStockIdx = m_nLabelStockIdx;
	
	item.m_dQty = 0.0;
	item.m_dValue = 0.0;
	
	if ( m_nTimeType != REPORT_YTD )
	{
		item.m_dQty += StockLevels.GetTotalSalesQty();
		item.m_dValue += StockLevels.GetTotalSalesValue();
	}
	
	if ( m_nTimeType != REPORT_PERIOD )
	{
		item.m_dQty += StockLevels.GetYTDSalesQty();
		item.m_dValue += StockLevels.GetYTDSalesValue();
	}

	int nInsertPoint = GetChartInsertPos( item );

	if ( m_chartinfo.GetChartType() != CHART_TYPE_MAXITEMS )
		m_SalesChart.InsertAt( nInsertPoint, item );
	else
	{
		if ( nInsertPoint < m_chartinfo.GetChartSize() )
			m_SalesChart.InsertAt( nInsertPoint, item );

		if ( m_SalesChart.GetSize() > m_chartinfo.GetChartSize() )
			m_SalesChart.RemoveAt( m_SalesChart.GetSize() - 1 );
	}
}

/**********************************************************************/

int CStockTreeReportNewTop100Sales::GetChartInsertPos( CPeriodChartItemNew& info )
{
	int nStartIdx = 0;
	int nEndIdx = m_SalesChart.GetSize() - 1;

	double dToInsert = 0.0;

	if ( m_chartinfo.IsSortedByValue() )
		dToInsert = info.m_dValue;
	else
		dToInsert = info.m_dQty;

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
		CPeriodChartItemNew arrayInfo = m_SalesChart.GetAt( nArrayIdx );
		
		double dToCompare = 0.0;
		if ( m_chartinfo.IsSortedByValue() )
			dToCompare = arrayInfo.m_dValue;
		else
			dToCompare = arrayInfo.m_dQty;

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

void CStockTreeReportNewTop100Sales::CreateTotalsLine( int nDepth )
{
	for ( int n = 0; n < m_SalesChart.GetSize(); n++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_nProgressTarget );

		CPeriodChartItemNew lineinfo = m_SalesChart.GetAt(n);

		if ( m_chartinfo.GetChartType() == CHART_TYPE_CUTOFF )
		{
			double dThisLine = 0.0;
			
			if ( m_chartinfo.IsSortedByValue() )
				dThisLine = lineinfo.m_dValue;
			else 
				dThisLine = lineinfo.m_dQty;
			
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
		csvOut.AppendStockQty( lineinfo.m_dQty, nSubUnits );
		csvOut.AppendVal( lineinfo.m_dValue );
		OutputSupplierFields( csvOut, m_bSupplierName, m_bSupplierRef );

		WriteReportLine( csvOut.GetLine() );
	}

	WriteBlankLine( TRUE );
	m_SalesChart.RemoveAll();
}

/**********************************************************************/

