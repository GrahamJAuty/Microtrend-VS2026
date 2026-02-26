/**********************************************************************/
 
/**********************************************************************/
//#include "DealerFlags.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewPriceList.h"
/**********************************************************************/

CStockTreeReportNewPriceList::CStockTreeReportNewPriceList( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_PRICE_LIST;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nTotalisersPerLine = 0;
}

/**********************************************************************/

void CStockTreeReportNewPriceList::PrepareArrays()
{
	CStockTreeReportNew::PrepareArrays();

	for ( int n = 0; n < m_arrayTotalFlags.GetSize(); n++ )
		m_arrayTotalFlags.SetAt( n, 0 );

	for ( int n = 0; n < m_arrayHeaderFlags.GetSize(); n++ )
		m_arrayHeaderFlags.SetAt( n, 0 );

	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		int nType = ( m_arrayReportFields.GetAt(n) ) % 100;
		if ( nType == SH_FIELD_CODE )
			m_arrayHeaderFlags.SetAt( n, 1 );
	}
}

/**********************************************************************/
	
void CStockTreeReportNewPriceList::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
	AddReportColumns();
}

/**********************************************************************/

void CStockTreeReportNewPriceList::AddReportColumns()
{
	AddColumn ( "DU Name", TA_LEFT, 250 );
	AddColumn ( "DU Cost", TA_RIGHT, 180 );
	AddColumn ( "DU Yield", TA_RIGHT, 220 );
	AddColumn ( "SU Cost", TA_RIGHT, 180 );

	if ( m_bSubUnits == TRUE )
		AddColumn ( "SU Waste", TA_RIGHT, 220 );
		
	AddColumn ( "Plu No", TA_LEFT, 380 );
	AddColumn ( "Retail", TA_RIGHT, 180 );
	AddColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 180 );
	AddColumn ( "GP", TA_RIGHT, 180 );
	AddColumn ( "GP%", TA_RIGHT, 180 );
}

/**********************************************************************/

bool CStockTreeReportNewPriceList::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	OutputStockCodeFields( csv, m_bStockUnit );
	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	if ( m_LabelStockRecord.GetSupplierCount() == 0 )
	{
		csv.AppendBlanks(3);
		csv.AppendVal( 0.0 );
		if ( m_bSubUnits ) csv.AppendVal( 0.0 );
	}
	else
	{
		csv.AppendString( m_LabelStockRecord.GetCase(0) );
		csv.AppendVal( m_LabelStockRecord.GetCost(0) );
		
		CString strItem;
		strItem.Format ( "%d x %.3f", m_LabelStockRecord.GetDUItems(0), m_LabelStockRecord.GetDUItemSize(0) );
		csv.AppendString( strItem );
		
		double dUnitCost = EstimateCostFromSUQty( 1.0 );
		csv.AppendVal( dUnitCost );
		
		if ( m_bSubUnits == TRUE )
		{
			double dWasteCost = EstimateWastageFromSales( dUnitCost );
			csv.AppendVal( dWasteCost );	
		}
	}

	if ( GetRetailPluInfo( TRUE ) == FALSE )
		return TRUE;

	if ( m_dRetailStockQty == 0.0 )
		return TRUE;

	double dTaxRate = m_dRetailTaxRate;
	double dStockRetail = m_dRetailPrice1 / m_dRetailStockQty;	
	double dStockCost = EstimateCostFromSUQty( 1.0 );
	double dStockTax = dStockRetail - ReportHelpers.CalcNonTax( dStockRetail, dTaxRate );
	double dStockWaste = EstimateWastageFromSales( dStockCost );
	double dRetailExTax = dStockRetail - dStockTax; 
	double dCostAndWaste = dStockCost + dStockWaste;
	double dStockGP = dRetailExTax - dCostAndWaste;
	double dStockGPPercent = CalculateGPPercent( dRetailExTax, dCostAndWaste );

	csv.AppendString( ReportHelpers.GetDisplayPluNo( _atoi64(m_strRetailPluNo) ) );
	csv.AppendVal ( dStockRetail );
	csv.AppendVal ( dStockTax );
	csv.AppendVal ( dStockGP );
	csv.AppendVal ( dStockGPPercent, 2 );
	
	return TRUE;
}

/**********************************************************************/