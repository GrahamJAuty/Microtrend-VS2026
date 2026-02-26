/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewYtdSales.h"
/**********************************************************************/

CStockTreeReportNewYtdSales::CStockTreeReportNewYtdSales( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bPeriodSales = FALSE;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_SALES;
	m_nTotalisersPerLine = 2;
}

/**********************************************************************/

void CStockTreeReportNewYtdSales::PrepareReport()
{
	if ( m_bPeriodSales == FALSE )
		m_strReportName = "YTD ";
	else
		m_strReportName = "YTD + Period ";
	
	m_strReportName += GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewYtdSales::AddReportColumns()
{
	AddColumn( "Total", TA_RIGHT, 180, TRUE );
	AddColumn( "Value", TA_RIGHT, 180, TRUE );
	AddColumn( "Average", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewYtdSales::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dQty = StockLevels.GetYTDSalesQty();
	double dValue = StockLevels.GetYTDSalesValue();

	if ( m_bPeriodSales == TRUE ) 
	{
		dQty += StockLevels.GetTotalSalesQty();
		dValue += StockLevels.GetTotalSalesValue();
	}

	double dAverage = CalculateAverage( dValue, dQty );

	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dQty, nSubUnits );
	csv.AppendVal( dValue );
	csv.AppendVal( dAverage );
	
	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dQty );
	AddToTotalisers( 1, dValue );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewYtdSales::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dQty = GetTotaliser( nDepth, 0 );
	double dValue = GetTotaliser( nDepth, 1 );
	double dAverage = CalculateAverage( dValue, dQty );
	
	csv.AppendStockQty( dQty );
	csv.AppendVal( dValue );
	csv.AppendVal( dAverage );
}

/**********************************************************************/

