/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "NodeTypes.h"
#include "StatusProgress.h"
#include "StockPluArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportStock.h"
/**********************************************************************/

void CSalesHistoryReportStock::GetSoldItemsList( int nDbIdx, COleDateTime dateFrom, CByteArray& FlagArray )
{
	m_bEposSales = TRUE;
	m_bManualSales = TRUE;
	m_bImportSales = TRUE;

	m_ReportFilters.CategoryFilter.SetCategoryFilterNo( CATEGORYSET_ALL );

	m_SupplierIdxArray.RemoveAll();
	for ( int n = 0; n <= DataManager.Supplier.GetSize(); n++ )
		m_SupplierIdxArray.Add( n + 1 );

	m_nConLevel = NODE_DATABASE;
	m_nDbIdx = nDbIdx;
	m_nTableNo = dbDatabase.GetDbNo( m_nDbIdx );
	
	m_nReportType = SH_CUSTOM;
	
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	COleDateTime timeStart ( 2003, 1, 1, 0, 0, 0 );
	COleDateTime timeEnd ( 2003, 1, 1, 23, 59, 59 );
	
	CSessionCSVRecord ReportSession;
	SetDates( dateFrom, dateNow, ReportSession );

	m_arrayReportFields.RemoveAll();
	m_arrayReportFields.Add( SH_FIELD_CODE );
		
	m_arrayBufferedLines.RemoveAll();
	m_arraySalesChart.RemoveAll();
	m_arrayUnsoldItems.RemoveAll();
	
	PrepareFields();
	Consolidate( TRUE );

	FlagArray.RemoveAll();

	if ( DataManager.Stock.GetSize() > 0 )
		FlagArray.InsertAt( 0, 0, DataManager.Stock.GetSize() );

	for ( int n = 0; n < m_QtyValueTable.GetBranchSize(0); n++ )
	{
		int nStockIdx;
		CString strStockCode = m_QtyValueTable.GetLabelString( 0, n );
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
			if ( nStockIdx < FlagArray.GetSize() )
				FlagArray.SetAt( nStockIdx, TRUE );
	}

	//EXCLUDE STOCK ITEMS THAT ARE SOLD AS PART OF A RECIPE
	StatusProgress.Lock( TRUE, "Checking recipe items" );
	CStockPluRecord StockPluRecord;
	for ( int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, DataManager.Stock.GetSize() );
		CString strStockCode = DataManager.Stock.GetStockCode(nStockIdx);
		
		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
		{
			DataManager.StockPlu.GetAt ( nStockPluIdx, StockPluRecord );

			if ( StockPluRecord.GetRecipePluNoCount() > 0 )
				FlagArray.SetAt ( nStockIdx, TRUE );
		}
	}
	StatusProgress.Unlock();
}

/**********************************************************************/
