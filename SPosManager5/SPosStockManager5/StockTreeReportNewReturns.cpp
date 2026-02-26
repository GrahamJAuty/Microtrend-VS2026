/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewReturns.h"
/**********************************************************************/

CStockTreeReportNewReturns::CStockTreeReportNewReturns( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_RETURNS;
	m_nTotalisersPerLine = 2;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
}

/**********************************************************************/

void CStockTreeReportNewReturns::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewReturns::AddReportColumns()
{
	AddColumn ( "Returns", TA_RIGHT, 180, TRUE );	
	AddColumn ( "Value", TA_RIGHT, 180, TRUE );
	AddColumn ( "Average", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewReturns::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dReturns = StockLevels.GetReturnsQty();
	
	if ( dReturns == 0.0 )
		return FALSE;

	double dValue = StockLevels.GetReturnsValue();
	double dAverage = CalculateAverage( dValue, dReturns );
	
	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dReturns, nSubUnits );
	csv.AppendVal( dValue );
	csv.AppendVal( dAverage );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );
	
	AddToTotalisers( 0, dReturns );
	AddToTotalisers( 1, dValue );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewReturns::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dReturns = GetTotaliser( nDepth, 0 );
	double dValue = GetTotaliser( nDepth, 1 );
	double dAverage = CalculateAverage( dValue, dReturns );
	
	csv.AppendStockQty( dReturns );
	csv.AppendVal( dValue );
	csv.AppendVal( dAverage );
}

/**********************************************************************/
