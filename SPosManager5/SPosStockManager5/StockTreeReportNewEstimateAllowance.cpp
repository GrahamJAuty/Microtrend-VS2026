/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewEstimateAllowance.h"
/**********************************************************************/

CStockTreeReportNewEstimateAllowance::CStockTreeReportNewEstimateAllowance( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_ESTIMATE_ALLOWANCE;
	m_nTotalisersPerLine = 33;
}

/**********************************************************************/

void CStockTreeReportNewEstimateAllowance::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewEstimateAllowance::AddReportColumns()
{
	AddColumn ( "Sales Qty", TA_RIGHT, 250, TRUE );
	AddColumn ( "Sales Val", TA_RIGHT, 250, TRUE );
	AddColumn ( "Price1 Val", TA_RIGHT, 250, TRUE );
	AddColumn ( "Allowance", TA_RIGHT, 250, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewEstimateAllowance::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dSalesQty = StockLevels.GetStocktakeTotalSalesQty();
	double dSalesVal = StockLevels.GetStocktakeTotalSalesValue();
	double dEstSalesVal = DataManager.Stock.CalculateRetailValue( m_LabelStockRecord, dSalesQty );

	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dSalesQty, nSubUnits );
	csv.AppendVal( dSalesVal );
	csv.AppendVal( dEstSalesVal );
	csv.AppendVal( dEstSalesVal - dSalesVal );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dSalesQty );
	AddToTotalisers( 1, dSalesVal );
	AddToTotalisers( 2, dEstSalesVal );
	
	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewEstimateAllowance::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dTotalSalesQty = GetTotaliser( nDepth, 0 );
	double dTotalSalesVal = GetTotaliser( nDepth, 1 );
	double dEstSalesVal = GetTotaliser( nDepth, 2 );
	
	csv.AppendStockQty( dTotalSalesQty );
	csv.AppendVal( dTotalSalesVal );
	csv.AppendVal( dEstSalesVal );
	csv.AppendVal( dEstSalesVal - dTotalSalesVal );
}

/**********************************************************************/