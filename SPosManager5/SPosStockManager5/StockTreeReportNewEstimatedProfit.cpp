/**********************************************************************/
 
/**********************************************************************/
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "ReportHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewEstimatedProfit.h"
/**********************************************************************/

CStockTreeReportNewEstimatedProfit::CStockTreeReportNewEstimatedProfit( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nTimeType = REPORT_PERIOD;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_ESTIMATED_PROFIT;
	m_nTotalisersPerLine = 5;
	m_bWantSalesCost = DealerFlags.GetPeriodDeliveryCostProfitFlag();
}

/**********************************************************************/

void CStockTreeReportNewEstimatedProfit::PrepareReport()
{
	switch( m_nTimeType )
	{
	case REPORT_YTD:			m_strReportName = "YTD ";			break;
	case REPORT_YTD_PLUS_PERIOD:	m_strReportName = "YTD + Period ";		break;
	case REPORT_PERIOD:
	default:					m_strReportName = "Period ";			break;
	}
	
	m_strReportName += GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewEstimatedProfit::AddReportColumns()
{
	AddColumn ( "Sales Qty", TA_RIGHT, 200, TRUE );
	AddColumn ( "Value", TA_RIGHT, 200, TRUE );
	AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 200, TRUE );
	AddColumn ( "Est. Cost", TA_RIGHT, 200, TRUE );

	if ( m_bSubUnits == TRUE )
		AddColumn ( "Est. Waste", TA_RIGHT, 220, TRUE );
	else
		AddColumn ( "Epos Waste", TA_RIGHT, 220, TRUE );

	AddColumn ( "Est. GP", TA_RIGHT, 200, TRUE );
	AddColumn ( "Est. GP%", TA_RIGHT, 200, FALSE );
}

/**********************************************************************/

bool CStockTreeReportNewEstimatedProfit::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dSalesQty = 0.0;
	double dSalesVal = 0.0;
	double dEcrWasteQty = 0.0;
	
	if ( m_nTimeType != REPORT_YTD )
	{
		dSalesQty += StockLevels.GetTotalSalesQty();
		dSalesVal += StockLevels.GetTotalSalesValue();
		dEcrWasteQty += StockLevels.GetEcrWastageQty();
	}
	
	if ( m_nTimeType != REPORT_PERIOD )
	{
		dSalesQty += StockLevels.GetYTDSalesQty();
		dSalesVal += StockLevels.GetYTDSalesValue();
		dEcrWasteQty += StockLevels.GetYTDEcrWastageQty();
	}

	double dTax = 0.0;
	if ( GetRetailPluInfo( FALSE ) == TRUE )
		dTax = dSalesVal - ReportHelpers.CalcNonTax( dSalesVal, m_dRetailTaxRate );
	
	double dCost = EstimateCostFromSUQty( dSalesQty );
	
	double dWaste = EstimateCostFromSUQty( dEcrWasteQty );
	if ( m_bSubUnits == TRUE )
	{
		double dAutoWasteQty = EstimateWastageFromSales( dSalesQty );
		dWaste += EstimateCostFromSUQty( dAutoWasteQty );
	}

	if ( dSalesQty == 0.0 && dWaste == 0.0 )
		return FALSE;

	double dRetailExTax = dSalesVal - dTax;
	double dCostAndWaste = dCost + dWaste;

	double dGP = dRetailExTax - dCostAndWaste; 
	double dGPPercent = CalculateGPPercent( dRetailExTax, dCostAndWaste );

	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dSalesQty, nSubUnits );
	csv.AppendVal( dSalesVal );
	csv.AppendVal( dTax );
	csv.AppendVal( dCost );
	csv.AppendVal( dWaste );
	csv.AppendVal( dGP );
	csv.AppendVal( dGPPercent, 2 );
	
	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dSalesQty );
	AddToTotalisers( 1, dSalesVal );
	AddToTotalisers( 2, dTax );
	AddToTotalisers( 3, dCost );
	AddToTotalisers( 4, dWaste );
	
	return TRUE;
}

/**********************************************************************/

bool CStockTreeReportNewEstimatedProfit::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CSalesCostCSVRecord& SalesCost )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dSalesQty = 0.0;
	double dSalesVal = 0.0;
	double dEcrWasteQty = 0.0;
	
	if ( m_nTimeType != REPORT_YTD )
	{
		dSalesQty += StockLevels.GetTotalSalesQty();
		dSalesVal += StockLevels.GetTotalSalesValue();
		dEcrWasteQty += StockLevels.GetEcrWastageQty();
	}
	
	if ( m_nTimeType != REPORT_PERIOD )
	{
		dSalesQty += StockLevels.GetYTDSalesQty();
		dSalesVal += StockLevels.GetYTDSalesValue();
		dEcrWasteQty += StockLevels.GetYTDEcrWastageQty();
	}

	double dTax = 0.0;
	if ( GetRetailPluInfo( FALSE ) == TRUE )
		dTax = dSalesVal - ReportHelpers.CalcNonTax( dSalesVal, m_dRetailTaxRate );

	double dCost = SalesCost.GetTotalSalesCost();

	double dWaste = SalesCost.GetEcrWasteCost();
	
	if ( m_bSubUnits == TRUE )
		dWaste +=  SalesCost.GetAutoWasteCost();

	if ( dSalesQty == 0.0 && dWaste == 0.0 )
		return FALSE;

	double dRetailExTax = dSalesVal - dTax;
	double dCostAndWaste = dCost + dWaste;

	double dGP = dRetailExTax - dCostAndWaste; 
	double dGPPercent = CalculateGPPercent( dRetailExTax, dCostAndWaste );

	OutputStockCodeFields( csv, m_bStockUnit );

	csv.AppendStockQty( dSalesQty, nSubUnits );
	csv.AppendVal( dSalesVal );
	csv.AppendVal( dTax );
	csv.AppendVal( dCost );
	csv.AppendVal( dWaste );
	csv.AppendVal( dGP );
	csv.AppendVal( dGPPercent, 2 );
	
	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dSalesQty );
	AddToTotalisers( 1, dSalesVal );
	AddToTotalisers( 2, dTax );
	AddToTotalisers( 3, dCost );
	AddToTotalisers( 4, dWaste );
	
	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewEstimatedProfit::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dSalesQty = GetTotaliser( nDepth, 0 );
	double dSalesVal = GetTotaliser( nDepth, 1 );
	double dTax = GetTotaliser( nDepth, 2 );
	double dCost = GetTotaliser( nDepth, 3 );
	double dWaste = GetTotaliser( nDepth, 4 );
	
	double dRetailExTax = dSalesVal - dTax;
	double dCostAndWaste = dCost + dWaste;

	double dGP = dRetailExTax - dCostAndWaste; 
	double dGPPercent = CalculateGPPercent( dRetailExTax, dCostAndWaste );
	
	csv.AppendStockQty( dSalesQty );
	csv.AppendVal( dSalesVal );
	csv.AppendVal( dTax );
	csv.AppendVal( dCost );
	csv.AppendVal( dWaste );
	csv.AppendVal( dGP );
	csv.AppendVal( dGPPercent, 2 );
}

/**********************************************************************/

