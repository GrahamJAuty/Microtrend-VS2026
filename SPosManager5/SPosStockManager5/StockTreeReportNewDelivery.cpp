/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewDelivery.h"
/**********************************************************************/

CStockTreeReportNewDelivery::CStockTreeReportNewDelivery( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_DELIVERY;
	m_nTotalisersPerLine = 3;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
}

/**********************************************************************/

void CStockTreeReportNewDelivery::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewDelivery::AddReportColumns()
{
	AddColumn ( "Delivery", TA_RIGHT, 180, TRUE );	
	AddColumn ( "Free", TA_RIGHT, 180, TRUE );
	AddColumn ( "Total", TA_RIGHT, 180, TRUE );
	AddColumn ( "Cost", TA_RIGHT, 180, TRUE );
	AddColumn ( "Average", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewDelivery::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dDelivery = StockLevels.GetDeliveryQty();
	double dFree = StockLevels.GetFreeQty();
	
	if ( dDelivery == 0.0 && dFree == 0.0 )
		return FALSE;
	
	double dTotal = dDelivery + dFree;
	double dCost = StockLevels.GetDeliveryValue();
	double dAverage = CalculateAverage( dCost, dTotal );
	
	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dDelivery, nSubUnits );
	csv.AppendStockQty( dFree, nSubUnits );
	csv.AppendStockQty( dTotal, nSubUnits );
	csv.AppendVal( dCost );
	csv.AppendVal( dAverage );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dDelivery );
	AddToTotalisers( 1, dFree );
	AddToTotalisers( 2, dCost );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewDelivery::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dDelivery = GetTotaliser( nDepth, 0 );
	double dFree = GetTotaliser( nDepth, 1 );
	double dTotal = dDelivery + dFree;
	double dCost = GetTotaliser( nDepth, 2 );
	double dAverage = CalculateAverage( dCost, dTotal );
	
	csv.AppendStockQty( dDelivery );
	csv.AppendStockQty( dFree );
	csv.AppendStockQty( dTotal );
	csv.AppendVal( dCost );
	csv.AppendVal( dAverage );
}

/**********************************************************************/
