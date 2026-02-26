/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "SalesHistoryReportStock.h"
/**********************************************************************/

void CSalesHistoryReportStock::CreateDummySales()
{
	COleDateTime dateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom().m_oleDateTime;
	COleDateTime dateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo().m_oleDateTime;
	COleDateTime dateFirstMonth = COleDateTime ( dateFrom.GetYear(), dateFrom.GetMonth(), 1, 0, 0, 0 );
	COleDateTime dateLastMonth = COleDateTime ( dateTo.GetYear(), dateTo.GetMonth(), 1, 0, 0, 0 );

	while ( dateFirstMonth <= dateLastMonth )
	{
		m_QtyValueTable.SetBufferDate( 0, dateFirstMonth );
		
		CEntityTreeStockSalesHistoryNormalNode DummyNode;
		m_QtyValueTable.Consolidate( DummyNode );

		if ( dateFirstMonth.GetMonth() < 12 )
			dateFirstMonth = COleDateTime ( dateFirstMonth.GetYear(), dateFirstMonth.GetMonth() + 1, 1, 0, 0, 0 );
		else
			dateFirstMonth = COleDateTime ( dateFirstMonth.GetYear() + 1, 1, 1, 0, 0, 0 );
	}
}

/**********************************************************************/
	
void CSalesHistoryReportStock::WriteUnsoldItems()
{
	switch ( m_nReportType )
	{
	case SH_STK_UNSOLD:
	case SH_STK_UNSOLD_MONTH:
		{
			for ( int nStockIdx = 0; nStockIdx < m_arrayUnsoldItems.GetSize(); nStockIdx++ )
			{
				if ( m_arrayUnsoldItems[ nStockIdx ] )
				{
					CString strStockCode = DataManager.Stock.GetStockCode( nStockIdx );

					//FILTER BY STOCK RANGE
					if ( m_bItemRange == TRUE )
					{
						if ( m_strItemFrom != "" && strStockCode < m_strItemFrom )
							continue;

						if ( m_strItemTo != "" && strStockCode > m_strItemTo )
							continue;
					}

					int nCategory = DataManager.Stock.GetCategory( nStockIdx );
					if ( m_ReportFilters.CheckCategory( nCategory ) == TRUE )
					{
						CCSV csvOut ( '\t' );
						csvOut.Add( ReportHelpers.GetDisplayStockCode(strStockCode) );
						csvOut.Add( DataManager.Stock.GetDescription( nStockIdx ) );
						csvOut.Add( nCategory );
						WriteReportLine( csvOut.GetLine() );
					}
				}
			}
		}
		break;

	case SH_STK_UNSOLD_CAT:
	case SH_STK_UNSOLD_MONTH_CAT:
		{
			for ( int nCategory = 0; nCategory <= MAX_CATEGORY; nCategory++ )
			{
				if ( m_ReportFilters.CheckCategory( nCategory ) == FALSE )
					continue;

				bool bGotCategoryLine = FALSE;
				for ( int nPos = 0; nPos < m_CategorySorter.GetItemCount( nCategory ); nPos++ )
				{
					int nStockIdx = m_CategorySorter.GetItemIndex( nCategory, nPos );

					if ( m_arrayUnsoldItems[ nStockIdx ] )
					{
						CString strStockCode = DataManager.Stock.GetStockCode( nStockIdx );
						
						//FILTER BY STOCK RANGE
						if ( m_bItemRange == TRUE )
						{
							if ( m_strItemFrom != "" && strStockCode < m_strItemFrom )
								continue;

							if ( m_strItemTo != "" && strStockCode > m_strItemTo )
								continue;
						}
						
						CCSV csvOut ( '\t' );
						csvOut.Add( ReportHelpers.GetDisplayStockCode(strStockCode) );
						csvOut.Add( DataManager.Stock.GetDescription( nStockIdx ) );
						csvOut.Add( nCategory );
						WriteReportLine( csvOut.GetLine() );
						bGotCategoryLine = TRUE;
					}
				}
				if ( bGotCategoryLine )
					WriteBlankLine( FALSE );
			}
		}
		break;
	}
}

/**********************************************************************/

bool CSalesHistoryReportStock::CreateUnsoldMonthlyReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_CategorySorter.Build( m_ReportFilters );

	CreateDummySales();
	
	m_arrayUnsoldItems.SetSize( DataManager.Stock.GetSize() );
	
	m_ReportFile.SetStyle1 ( GetReportTitle( "Unsold Stock Items" ) );
	m_ReportFile.AddColumn ( "Stockcode", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Category", TA_RIGHT, 200 );
	
	StatusProgress.Lock( TRUE, "Creating report" );
	
	__int64 nProgressTarget = m_QtyValueTable.GetNodeCount();
	__int64 nProgressSoFar = 0;

	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);	
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		for ( int n = 0; n < DataManager.Stock.GetSize(); n++ )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( n, StockRecord );
		
			int nSuppNo = 0;
			if ( StockRecord.GetSupplierCount() > 0 )
				nSuppNo = StockRecord.GetSuppNo(0);

			bool bAllowInReport = TRUE;
			if ( m_SupplierReportMap.GetSortIdxBySuppNo( nSuppNo ) == -1 )
				bAllowInReport = FALSE;
			else
			{
				for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
				{
					int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
					int nValueNo = StockRecord.GetCustomFieldValueNo( nFieldNo );
					int nSortIdx = DataManager.CustomFieldManager.GetSortIdxByValueNo( nSlot, nValueNo );
		
					if ( nSortIdx == -1 )
					{
						bAllowInReport = FALSE;
						break;
					}
				}
			}

			m_arrayUnsoldItems[n] = bAllowInReport;
		}
		
		COleDateTime date = m_QtyValueTable.GetLabelDate( 0, nNodeIdx1 );
		
		WriteReportLine( GetRepMonth ( date, FALSE, TRUE ) );
		WriteReportLine( "<LI>" );
		
		int nArrayIdx2 = m_QtyValueTable.GetNextBranchIdx( 0, nNodeIdx1 );
		
		if ( nArrayIdx2 > 0 )
		{
			int nNodeCount2 = m_QtyValueTable.GetBranchSize(nArrayIdx2);
			for ( int nNodeIdx2 = 0; nNodeIdx2 < nNodeCount2; nNodeIdx2++ )
			{
				StatusProgress.SetPos ( nProgressSoFar++, nProgressTarget );

				CEntityTreeStockSalesHistoryNormalNode NodeSales;
				m_QtyValueTable.GetNode( nArrayIdx2, nNodeIdx2, NodeSales );
				if ( ( NodeSales.GetQty() == 0.0 ) && ( NodeSales.GetTotalVal() == 0.0 ) )
					continue;

				int nStockIdx = 0;
				CString strStockCode = m_QtyValueTable.GetLabelString( nArrayIdx2, nNodeIdx2 );
				if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) )
					if ( nStockIdx < m_arrayUnsoldItems.GetSize() )
						m_arrayUnsoldItems[ nStockIdx ] = FALSE;
			}
			WriteUnsoldItems();
			WriteBlankLine( TRUE );
		}
		else
		{
			WriteReportLine( "No sales on file" );
			WriteBlankLine( TRUE );
		}
	}
	StatusProgress.Unlock();
	
	WriteBlankLine( TRUE );
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

bool CSalesHistoryReportStock::CreateUnsoldReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_CategorySorter.Build( m_ReportFilters );
	
	m_arrayUnsoldItems.SetSize( DataManager.Stock.GetSize() );
	
	for ( int n = 0; n < DataManager.Stock.GetSize(); n++ )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( n, StockRecord );
		
		int nSuppNo = 0;
		if ( StockRecord.GetSupplierCount() > 0 )
			nSuppNo = StockRecord.GetSuppNo(0);

		bool bAllowInReport = TRUE;
		if ( m_SupplierReportMap.GetSortIdxBySuppNo( nSuppNo ) == -1 )
			bAllowInReport = FALSE;
		else
		{
			for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
			{
				int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
				int nValueNo = StockRecord.GetCustomFieldValueNo( nFieldNo );
				int nSortIdx = DataManager.CustomFieldManager.GetSortIdxByValueNo( nSlot, nValueNo );
		
				if ( nSortIdx == -1 )
				{
					bAllowInReport = FALSE;
					break;
				}
			}
		}

		m_arrayUnsoldItems[n] = bAllowInReport;
	}	

	m_ReportFile.SetStyle1 ( GetReportTitle( "Unsold Stock Items" ) );
	m_ReportFile.AddColumn ( "Stockcode", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Category", TA_RIGHT, 200 );
		
	StatusProgress.Lock( TRUE, "Creating report" );
	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		StatusProgress.SetPos ( nNodeIdx1, nNodeCount1 );
		
		CEntityTreeStockSalesHistoryNormalNode NodeSales;
		m_QtyValueTable.GetNode( 0, nNodeIdx1, NodeSales );
		if ( ( NodeSales.GetQty() == 0.0 ) && ( NodeSales.GetTotalVal() == 0.0 ) )		
			continue;
			
		int nStockIdx;
		CString strStockCode = m_QtyValueTable.GetLabelString( 0, nNodeIdx1 );
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) )
			if ( nStockIdx < m_arrayUnsoldItems.GetSize() )
				m_arrayUnsoldItems[ nStockIdx ] = FALSE;
	}
	StatusProgress.Unlock();
	WriteUnsoldItems();
	WriteBlankLine ( TRUE );
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

