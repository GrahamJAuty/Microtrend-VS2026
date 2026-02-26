/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewTransferClosing.h"
/**********************************************************************/

CStockTreeReportNewTransferClosing::CStockTreeReportNewTransferClosing( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_TRANSFER_CLOSING;
	m_nTotalisersPerLine = 0;
	m_bWantTransferDetail = TRUE;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
}

/**********************************************************************/

void CStockTreeReportNewTransferClosing::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewTransferClosing::PrepareStockpointList()
{
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( m_StockLevelTree.GetDbIdx(), nStartIdx, nEndIdx );

	m_arraySpIdx.RemoveAll();
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
	{
		if ( dbStockpoint.GetSpNo( nSpIdx ) != m_StockLevelTree.GetTableNo() )
			m_arraySpIdx.Add( nSpIdx );
	}

	dbStockpoint.SortSpListByName( m_arraySpIdx );
	m_nTotalisersPerLine = m_arraySpIdx.GetSize() + 8;
}

/**********************************************************************/

void CStockTreeReportNewTransferClosing::AddReportColumns()
{
	AddColumn ( "Opening", TA_RIGHT, 180, TRUE );
	AddColumn ( "Delivery", TA_RIGHT, 180, TRUE );
	AddColumn ( "Returns", TA_RIGHT, 180, TRUE );
	AddColumn ( "Sales", TA_RIGHT, 180, TRUE );
	AddColumn ( "Waste", TA_RIGHT, 180, TRUE );
	AddColumn ( "Adjust.", TA_RIGHT, 180, TRUE );
	
	for ( int n = 0; n < m_arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = m_arraySpIdx.GetAt(n);
		AddColumn( dbStockpoint.GetName( nSpIdx ), TA_RIGHT, 180, TRUE );
	}

	AddColumn ( "Closing", TA_RIGHT, 180, TRUE );
	AddColumn ( "Variance", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewTransferClosing::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord& TransferDetail )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	OutputStockCodeFields( csv, m_bStockUnit );

	double dOpeningQty = StockLevels.GetOpeningQty();
	double dDeliveryQty = StockLevels.GetStocktakeDeliveryQty() + StockLevels.GetStocktakeFreeQty();
	double dReturnsQty = StockLevels.GetStocktakeReturnsQty(); 
	double dSalesQty = StockLevels.GetStocktakeTotalSalesQty();
	double dWasteQty = StockLevels.GetStocktakeAutoWastageQty() + StockLevels.GetStocktakeEcrWastageQty();
	double dAdjustQty = StockLevels.GetStocktakeAdjustmentQty();
	double dApparentQty = StockLevels.GetStocktakeApparentQty();
	double dClosingQty = StockLevels.GetStocktakeClosingQty();
	double dVarianceQty = dClosingQty - dApparentQty;
	
	csv.AppendStockQty( dOpeningQty, nSubUnits );
	csv.AppendStockQty( dDeliveryQty, nSubUnits );
	csv.AppendStockQty( dReturnsQty, nSubUnits );
	csv.AppendStockQty( dSalesQty, nSubUnits );
	csv.AppendStockQty( dWasteQty, nSubUnits );
	csv.AppendStockQty( dAdjustQty, nSubUnits );

	int nSize = m_arraySpIdx.GetSize();
	for ( int n = 0; n < nSize; n++ )
	{
		int nSpIdx = m_arraySpIdx.GetAt(n);
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

		CTransferDetailInfo info;
		info.m_nSpNo = nSpNo;
		TransferDetail.GetTransferDetailInfo( info );

		double dQty = info.m_dStocktakeTransferOutQty - info.m_dStocktakeTransferInQty;
		csv.AppendStockQty( dQty, nSubUnits );

		AddToTotalisers( n + 6, dQty );
	}

	csv.AppendStockQty( dClosingQty, nSubUnits );
	csv.AppendStockQty( dVarianceQty, nSubUnits );

	AddToTotalisers( 0, dOpeningQty );
	AddToTotalisers( 1, dDeliveryQty );
	AddToTotalisers( 2, dReturnsQty );
	AddToTotalisers( 3, dSalesQty );
	AddToTotalisers( 4, dWasteQty );
	AddToTotalisers( 5, dAdjustQty );
	AddToTotalisers( 6 + nSize, dClosingQty );
	AddToTotalisers( 7 + nSize, dVarianceQty );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewTransferClosing::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	for ( int n = 0; n < m_nTotalisersPerLine; n++ )
		csv.AppendStockQty( GetTotaliser( nDepth, n ) );
}

/**********************************************************************/
