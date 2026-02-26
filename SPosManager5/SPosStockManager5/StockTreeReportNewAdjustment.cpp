/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewAdjustment.h"
/**********************************************************************/

CStockTreeReportNewAdjustment::CStockTreeReportNewAdjustment( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_ADJUST;
	m_nTimeType = REPORT_PERIOD;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_bSales = TRUE;
	m_bBreakdown = TRUE;
	m_bAdjust = TRUE;
	m_bWasteEcr = TRUE;
	m_bWasteAuto = FALSE;
	m_nTotalisersPerLine = 8;
}

/**********************************************************************/

void CStockTreeReportNewAdjustment::PrepareReport()
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

void CStockTreeReportNewAdjustment::AddReportColumns()
{			
	int nFieldTypes = 0;

	if ( m_bSales == TRUE )
	{
		AddColumn ( "Sales", TA_RIGHT, 210, TRUE );
		AddColumn ( "Value", TA_RIGHT, 210, TRUE );
	}

	if ( m_bBreakdown == TRUE )
	{
		if ( m_bAdjust == TRUE )
		{
			AddColumn ( "Adjust", TA_RIGHT, 210, TRUE );
			AddColumn ( "Value", TA_RIGHT, 210, TRUE );
			nFieldTypes++;
		}

		if ( m_bWasteEcr == TRUE )
		{
			AddColumn ( "Epos Waste", TA_RIGHT, 210, TRUE );
			AddColumn ( "Value", TA_RIGHT, 210, TRUE );
			nFieldTypes++;
		}

		if ( m_bWasteAuto == TRUE )
		{
			AddColumn ( "Auto Waste", TA_RIGHT, 210, TRUE );
			AddColumn ( "Value", TA_RIGHT, 210, TRUE );
			nFieldTypes++;
		}
	}
	
	if ( ( nFieldTypes >= 2 ) || ( m_bBreakdown == FALSE ) )
	{
		AddColumn( "Total", TA_RIGHT, 210, TRUE );	
		AddColumn( "Value", TA_RIGHT, 210, TRUE );
	}

	if ( m_bSales == TRUE )
		AddColumn( "%Value", TA_RIGHT, 210, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewAdjustment::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dSalesQty = 0.0;
	double dSalesVal = 0.0;
	double dAdjustQty = 0.0;
	double dAdjustVal = 0.0;
	double dEcrWasteQty = 0.0;
	double dEcrWasteVal = 0.0;
	double dAutoWasteQty = 0.0; 
	double dAutoWasteVal = 0.0;

	if ( m_nTimeType != REPORT_YTD )
	{
		if ( m_bAdjust == TRUE )
		{
			dAdjustQty += StockLevels.GetAdjustmentQty();
			dAdjustVal += StockLevels.GetAdjustmentValue();
		}

		if ( m_bWasteEcr == TRUE )
		{
			dEcrWasteQty += StockLevels.GetEcrWastageQty();
			dEcrWasteVal += StockLevels.GetEcrWastageValue();
		}

		if ( m_bSales == TRUE )
		{
			dSalesQty += StockLevels.GetTotalSalesQty();
			dSalesVal += StockLevels.GetTotalSalesValue();
		}
	}

	if ( m_nTimeType != REPORT_PERIOD )
	{
		if ( m_bAdjust == TRUE )
		{
			dAdjustQty += StockLevels.GetYTDAdjustmentQty();
			dAdjustVal += StockLevels.GetYTDAdjustmentValue();
		}

		if ( m_bWasteEcr == TRUE )
		{
			dEcrWasteQty += StockLevels.GetYTDEcrWastageQty();
			dEcrWasteVal += StockLevels.GetYTDEcrWastageValue();
		}

		if ( m_bSales == TRUE )
		{
			dSalesQty += StockLevels.GetYTDSalesQty();
			dSalesVal += StockLevels.GetYTDSalesValue();
		}
	}

	if ( m_bWasteAuto == TRUE )
	{
		double dWastePercent = StockLevels.GetAutoWastagePercent();
		dAutoWasteQty += EstimateWastageFromSales( dSalesQty );
		dAutoWasteVal += EstimateWastageFromSales( dSalesVal );
	}
	
	if ( dSalesQty == 0.0 && dAdjustQty == 0.0 && dEcrWasteQty == 0.0 && dAutoWasteQty == 0.0 )
		return FALSE;

	double dTotalQty = dAdjustQty + dEcrWasteQty + dAutoWasteQty;
	double dTotalVal = dAdjustVal + dEcrWasteVal + dAutoWasteVal;
	double dSalesWasteQty = dTotalQty + dSalesQty;
	double dSalesWasteVal = dTotalVal + dSalesVal;

	OutputStockCodeFields( csv, m_bStockUnit );
	
	int nFieldTypes = 0;

	if ( m_bSales == TRUE )
	{
		csv.AppendStockQty( dSalesQty, nSubUnits );
		csv.AppendVal( dSalesVal );
	}

	if ( m_bBreakdown == TRUE )
	{
		if ( m_bAdjust == TRUE )
		{
			csv.AppendStockQty( dAdjustQty, nSubUnits );
			csv.AppendVal( dAdjustVal );
			nFieldTypes++;
		}

		if ( m_bWasteEcr == TRUE )
		{
			csv.AppendStockQty( dEcrWasteQty, nSubUnits );
			csv.AppendVal( dEcrWasteVal );
			nFieldTypes++;
		}

		if ( m_bWasteAuto == TRUE )
		{
			csv.AppendStockQty( dAutoWasteQty, nSubUnits );
			csv.AppendVal( dAutoWasteVal );
			nFieldTypes++;
		}
	}
	
	if ( ( nFieldTypes >= 2 ) || ( m_bBreakdown == FALSE ) )
	{
		csv.AppendStockQty( dTotalQty, nSubUnits );
		csv.AppendVal( dTotalVal );
	}
	
	if ( m_bSales == TRUE )
	{
		if ( dSalesWasteVal != 0.0 )
			csv.AppendVal( ( dTotalVal * 100.0 ) / dSalesWasteVal, 2 );
		else
			csv.AppendVal( 0.0, 2 );
	}

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	AddToTotalisers( 0, dSalesQty );
	AddToTotalisers( 1, dSalesVal );
	AddToTotalisers( 2, dAdjustQty );
	AddToTotalisers( 3, dAdjustVal );
	AddToTotalisers( 4, dAutoWasteQty );
	AddToTotalisers( 5, dAutoWasteVal );
	AddToTotalisers( 6, dEcrWasteQty );
	AddToTotalisers( 7, dEcrWasteVal );
	
	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewAdjustment::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dSalesQty = GetTotaliser( nDepth, 0 );
	double dSalesVal = GetTotaliser( nDepth, 1 );
	double dAdjustQty = GetTotaliser( nDepth, 2 );
	double dAdjustVal = GetTotaliser( nDepth, 3 );
	double dAutoWasteQty = GetTotaliser( nDepth, 4 );
	double dAutoWasteVal = GetTotaliser( nDepth, 5 );
	double dEcrWasteQty = GetTotaliser( nDepth, 6 );
	double dEcrWasteVal = GetTotaliser( nDepth, 7 );
	
	double dTotalQty = dAdjustQty + dEcrWasteQty + dAutoWasteQty;
	double dTotalVal = dAdjustVal + dEcrWasteVal + dAutoWasteVal;
	double dSalesWasteQty = dTotalQty + dSalesQty;
	double dSalesWasteVal = dTotalVal + dSalesVal;

	int nFieldTypes = 0;

	if ( m_bSales == TRUE )
	{
		csv.AppendStockQty( dSalesQty );
		csv.AppendVal( dSalesVal );
	}

	if ( m_bBreakdown == TRUE )
	{
		if ( m_bAdjust == TRUE )
		{
			csv.AppendStockQty( dAdjustQty );
			csv.AppendVal( dAdjustVal );
			nFieldTypes++;
		}

		if ( m_bWasteEcr == TRUE )
		{
			csv.AppendStockQty( dEcrWasteQty );
			csv.AppendVal( dEcrWasteVal );
			nFieldTypes++;
		}

		if ( m_bWasteAuto == TRUE )
		{
			csv.AppendStockQty( dAutoWasteQty );
			csv.AppendVal( dAutoWasteVal );
			nFieldTypes++;
		}
	}
	
	if ( ( nFieldTypes >= 2 ) || ( m_bBreakdown == FALSE ) )
	{
		csv.AppendStockQty( dTotalQty );
		csv.AppendVal( dTotalVal );
	}
	
	if ( m_bSales == TRUE )
	{
		if ( dSalesWasteVal != 0.0 )
			csv.AppendVal( ( dTotalVal * 100.0 ) / dSalesWasteVal, 2 );
		else
			csv.AppendVal( 0.0, 2 );
	}
}

/**********************************************************************/


