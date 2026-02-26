/**********************************************************************/
 
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewTransferPeriod.h"
/**********************************************************************/

CStockTreeReportNewTransferPeriod::CStockTreeReportNewTransferPeriod( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_TRANSFER_PERIOD;
	m_nTotalisersPerLine = 0;
	m_bWantTransferDetail = TRUE;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
}

/**********************************************************************/

void CStockTreeReportNewTransferPeriod::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewTransferPeriod::PrepareStockpointList()
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
	m_nTotalisersPerLine = m_arraySpIdx.GetSize() + 1;
}

/**********************************************************************/

void CStockTreeReportNewTransferPeriod::AddReportColumns()
{
	for ( int n = 0; n < m_arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = m_arraySpIdx.GetAt(n);
		AddColumn( dbStockpoint.GetName( nSpIdx ), TA_RIGHT, 180, TRUE );
	}

	AddColumn( "Total", TA_RIGHT, 180, TRUE );
}

/**********************************************************************/

bool CStockTreeReportNewTransferPeriod::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord& TransferDetail )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	OutputStockCodeFields( csv, m_bStockUnit );

	double dTotal = 0.0;
	
	int nSize = m_arraySpIdx.GetSize();
	for ( int n = 0; n < nSize; n++ )
	{
		int nSpIdx = m_arraySpIdx.GetAt(n);
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

		CTransferDetailInfo info;
		info.m_nSpNo = nSpNo;
		TransferDetail.GetTransferDetailInfo( info );

		double dQty = info.m_dTransferOutQty - info.m_dTransferInQty;
		
		csv.AppendStockQty( dQty, nSubUnits );
		AddToTotalisers( n, dQty );

		dTotal += dQty;
		
	}

	csv.AppendStockQty( dTotal, nSubUnits );
	AddToTotalisers( nSize, dTotal );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewTransferPeriod::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	for ( int n = 0; n < m_nTotalisersPerLine; n++ )
		csv.AppendStockQty( GetTotaliser( nDepth, n ) );
}

/**********************************************************************/
