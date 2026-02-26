/**********************************************************************/
 
/**********************************************************************/
#include "math.h"
/**********************************************************************/
#include "PriceHelpers.h"
#include "StockReportNames.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockTreeReportNewApparentDU.h"
/**********************************************************************/

CStockTreeReportNewApparentDU::CStockTreeReportNewApparentDU( bool bFull, CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bFull = bFull;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;

	if ( TRUE == bFull )
	{
		m_nReportParamsType = STOCK_APPARENT_QTY_DU;
		m_nTotalisersPerLine = 22;
	}
	else
	{
		m_nReportParamsType = STOCK_APPARENT_SUMMARY_DU;
		m_nTotalisersPerLine = 10;
	}

	m_bHideAboveMin = FALSE;
	m_bPositive = TRUE;
	m_bZero = TRUE;
	m_bNegative = TRUE;
}

/**********************************************************************/

void CStockTreeReportNewApparentDU::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewApparentDU::AddReportColumns()
{			
	if ( TRUE == m_bFull )
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
		AddColumn ( "Stocktake", TA_RIGHT, 210, TRUE );
		AddColumn ( "Total", TA_RIGHT, 180, TRUE );
	}
	else
	{
		AddColumn ( "Opening", TA_RIGHT, 210, TRUE );
		AddColumn ( "Stock In", TA_RIGHT, 210, TRUE );
		AddColumn ( "Stock Out", TA_RIGHT, 210, TRUE );
		AddColumn ( "Stocktake", TA_RIGHT, 210, TRUE );
		AddColumn ( "Total", TA_RIGHT, 210, TRUE );
	}
}

/**********************************************************************/

bool CStockTreeReportNewApparentDU::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	double dApparentQty = StockLevels.GetApparentQty();
	
	if ( ( m_bPositive == FALSE ) || ( m_bZero == FALSE ) || ( m_bNegative == FALSE ) )
	{	
		int nResult = CPriceHelpers::CompareDoubles( dApparentQty, 0.0, 5 );

		if ( m_bPositive == FALSE )
			if ( nResult == 1 )
				return FALSE;

		if ( m_bZero == FALSE )
			if ( nResult == 0 )
				return FALSE;

		if ( m_bNegative == FALSE )
			if ( nResult == -1 )
				return FALSE;
	}

	if ( TRUE == m_bHideAboveMin )
	{
		if ( dApparentQty >= StockLevels.GetMinimumLevelQty() )
			return FALSE;
	}

	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dDUYield = 1.0;
	if ( m_LabelStockRecord.GetSupplierCount() >= 1 )
		dDUYield = m_LabelStockRecord.GetDUItemSize(0) * m_LabelStockRecord.GetDUItems(0);

	OutputStockCodeFields( csv, m_bStockUnit );

	if ( TRUE == m_bFull )
	{		
		ProcessAmount( csv, StockLevels.GetOpeningQty(), dDUYield, 0 );
		ProcessAmount( csv, StockLevels.GetDeliveryQty(), dDUYield, 2 );
		ProcessAmount( csv, StockLevels.GetFreeQty(), dDUYield, 4 );
		ProcessAmount( csv, StockLevels.GetReturnsQty(), dDUYield, 6 );
		
		if ( Sysset.GetMaxStockPoints() > 1 )
			ProcessAmount( csv, StockLevels.GetTransferInQty(), dDUYield, 8 );
		
		double dSales = StockLevels.GetTotalSalesQty();
		ProcessAmount( csv, dSales, dDUYield, 10 );
		
		double dWastage = StockLevels.GetEcrWastageQty();
		if ( m_bSubUnits == TRUE )	
			dWastage += EstimateWastageFromSales( dSales );
		ProcessAmount( csv, dWastage, dDUYield, 12 );

		if ( Sysset.GetMaxStockPoints() > 1 )
			ProcessAmount( csv, StockLevels.GetTransferOutQty(), dDUYield, 14 );

		ProcessAmount( csv, StockLevels.GetAdjustmentQty(), dDUYield, 16 );
		ProcessAmount( csv, StockLevels.GetPendingStocktakeQty(), dDUYield, 18 );
		ProcessAmount( csv, dApparentQty, dDUYield, 20 );

		OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );
	}
	else
	{
		double dStockIn = 0.0;
		dStockIn += StockLevels.GetDeliveryQty();
		dStockIn += StockLevels.GetFreeQty();
		dStockIn += StockLevels.GetTransferInQty();
		
		double dSales = StockLevels.GetTotalSalesQty();
		
		double dStockOut = 0.0;
		dStockOut += dSales;
		dStockOut += StockLevels.GetReturnsQty();
		dStockOut += StockLevels.GetTransferOutQty();
		dStockOut += StockLevels.GetAdjustmentQty();
		dStockOut += StockLevels.GetEcrWastageQty();
		if ( m_bSubUnits == TRUE )	dStockOut += EstimateWastageFromSales( dSales );

		ProcessAmount( csv, StockLevels.GetOpeningQty(), dDUYield, 0 );
		ProcessAmount( csv, dStockIn, dDUYield, 2 );
		ProcessAmount( csv, dStockOut, dDUYield, 4 );
		ProcessAmount( csv, StockLevels.GetPendingStocktakeQty(), dDUYield, 6 );
		ProcessAmount( csv, dApparentQty, dDUYield, 8 );

		OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );
	}

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewApparentDU::ProcessAmount( CStockReportLine& csv, double dAmount, double dYield, int nTotalPos )
{
	if ( 0.0 == dYield )
		dYield = 1.0;

	int nDUCount = int ( floor ( dAmount / dYield ) );
	double dSUAmount = dAmount - ( dYield * nDUCount );

	CString strAmount;
	strAmount.Format( "%d/%.*f",
		nDUCount,
		SysInfo.GetDPValue(),
		dSUAmount );

	csv.AppendString( strAmount );

	AddToTotalisers( nTotalPos, (double) nDUCount );
	AddToTotalisers( nTotalPos + 1, dSUAmount );
}

/**********************************************************************/

void CStockTreeReportNewApparentDU::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	if ( TRUE == m_bFull )
	{
		ProcessTotal( csv, nDepth, 0 );			//OPEN
		ProcessTotal( csv, nDepth, 2 );			//DELIVERY
		ProcessTotal( csv, nDepth, 4 );			//FREE
		ProcessTotal( csv, nDepth, 6 );			//RETURNS

		if ( Sysset.GetMaxStockPoints() > 1 )
			ProcessTotal( csv, nDepth, 8 );		//XFER IN

		ProcessTotal( csv, nDepth, 10 );		//SALES
		ProcessTotal( csv, nDepth, 12 );		//WASTAGE

		if ( Sysset.GetMaxStockPoints() > 1 )
			ProcessTotal( csv, nDepth, 14 );	//XFER OUT

		ProcessTotal( csv, nDepth, 16 );		//ADJUST
		ProcessTotal( csv, nDepth, 18 );		//PENDING
		ProcessTotal( csv, nDepth, 20 );		//APPARENT
	}
	else
	{
		ProcessTotal( csv, nDepth, 0 );			//OPEN
		ProcessTotal( csv, nDepth, 2 );			//STOCK IN
		ProcessTotal( csv, nDepth, 4 );			//STOCK OUT
		ProcessTotal( csv, nDepth, 6 );			//PENDING
		ProcessTotal( csv, nDepth, 8 );			//APPARENT
	}
}

/**********************************************************************/

void CStockTreeReportNewApparentDU::ProcessTotal( CStockReportLine& csv, int nDepth, int nTotalPos )
{
	int nDUCount = int ( floor ( GetTotaliser( nDepth, nTotalPos ) ) );
	double dSUAmount = GetTotaliser( nDepth, nTotalPos + 1 );

	CString strAmount;
	strAmount.Format( "%d/%.*f",
		nDUCount,
		SysInfo.GetDPValue(),
		dSUAmount );

	csv.AppendString( strAmount );
}

/**********************************************************************/

