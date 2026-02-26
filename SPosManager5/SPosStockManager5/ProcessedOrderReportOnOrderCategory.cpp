/**********************************************************************/
#include "CCSVReportLine.h"
#include "Consolidation.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "ProcessedOrderFilterArray.h"
#include "ReportConsolidationArray.h"
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrderReportOnOrderCategory.h"
/**********************************************************************/

CReportConsolOnOrderCategory::CReportConsolOnOrderCategory()
{
	Reset();
}

/**********************************************************************/

void CReportConsolOnOrderCategory::Reset()
{
	m_nCategory = 0;
	m_nOrderLine = 0;
	m_strData = "";
}

/**********************************************************************/

void CReportConsolOnOrderCategory::Add ( CReportConsolOnOrderCategory& source )
{
}

/**********************************************************************/

int CReportConsolOnOrderCategory::Compare( CReportConsolOnOrderCategory& source, int nHint )
{
	if ( m_nCategory != source.m_nCategory )
		return ( m_nCategory > source.m_nCategory ? 1 : -1 );

	if ( m_nOrderLine != source.m_nOrderLine )
		return ( m_nOrderLine > source.m_nOrderLine ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CProcessedOrderReportOnOrderCategory::CProcessedOrderReportOnOrderCategory( CWordArray& wSuppSelectArray ) : CProcessedOrderBaseReport()
{
	for ( int nIndex = 0; nIndex < wSuppSelectArray.GetSize(); nIndex++ )
		m_wSuppSelectArray.Add ( wSuppSelectArray.GetAt ( nIndex ) );

	m_nCategoryFilter = 0;
}

/**********************************************************************/

bool CProcessedOrderReportOnOrderCategory::CreateReport( int nDbIdx )
{
	m_nDbIdx = nDbIdx;
	bool bSuccess = FALSE;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		bSuccess = TRUE;
		CString strConLevel;

		ReportFile.SetStyle1 ( GetReportTitle() );
		ReportFile.AddColumn ( "Order No.", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Reference", TA_LEFT, 380 );
		ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Yield", TA_RIGHT, 250 );
		ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "DU Cost", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Total Cost", TA_RIGHT, 220 );
	
		CArray<int,int> arraySpIdx;

		for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
			if ( MatchStockpoint ( nSpIdx ) == TRUE )
				arraySpIdx.Add( nSpIdx );

		dbStockpoint.SortSpListByName( arraySpIdx );

		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdx = arraySpIdx.GetAt(n);
			ReportFile.AddColumn ( dbStockpoint.GetName( nSpIdx ), TA_RIGHT, 260 );
		}
	
		if ( DealerFlags.GetTransitionalOrderFlag() == TRUE )
		{
			//WRITE THE DATABASE ORDERS
			WriteOrders( ReportFile, nDbIdx, -1 );

			//WRITE THE STOCKPOINT ORDERS
			for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
			{
				int nSpIdx = arraySpIdx.GetAt(n);
				WriteOrders( ReportFile, nDbIdx, nSpIdx );
			}
		}
		else if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_ORDER )
		{
			//WRITE THE STOCKPOINT ORDERS
			for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
			{
				int nSpIdx = arraySpIdx.GetAt(n);
				WriteOrders( ReportFile, nDbIdx, nSpIdx );
			}
		}
		else
			//WRITE THE DATABASE ORDER
			WriteOrders( ReportFile, nDbIdx, -1 );
	}
	
	ShowStockpoints ( ReportFile );
	ReportFile.Close();
	
	return bSuccess;
}
	
/**********************************************************************/

void CProcessedOrderReportOnOrderCategory::WriteOrders( CReportFile& ReportFile, int nDbIdx, int nSingleSpIdx )
{
	bool bDateFilter = FALSE;
	CString strCheckDateFrom = "";
	CString strCheckDateTo = "";

	if ( ( m_strDateFrom.GetLength() == 10 ) && ( m_strDateTo.GetLength() == 10 ) )
	{
		bDateFilter = TRUE;
		strCheckDateFrom = m_strDateFrom.Right(4) + m_strDateFrom.Mid(3,2) + m_strDateFrom.Left(2) ;
		strCheckDateTo = m_strDateTo.Right(4) + m_strDateTo.Mid(3,2) + m_strDateTo.Left(2) ;
	}

	double dOrderCost = 0.0;

	int nOrderLineCounter = 0;
	CReportConsolidationArray<CReportConsolOnOrderCategory> arrayBuffer;
	CReportConsolidationArray<CConsolidatedDoubleByInt> arrayCost;

	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet( m_nCategoryFilter );

	CString strConLevel = "";

	if ( nSingleSpIdx == -1 )
		strConLevel.Format( ", %s", dbDatabase.GetName( nDbIdx ) );
	else if ( nSingleSpIdx >= 0 )
		strConLevel.Format( ", %s", dbStockpoint.GetName( nSingleSpIdx ) );

	//loop through all selected suppliers
	for ( int nSelectIdx = 0; nSelectIdx < m_wSuppSelectArray.GetSize(); nSelectIdx++)
	{
		CString strLine;
			
		int nSuppNo;
		int nSuppIdx = m_wSuppSelectArray.GetAt ( nSelectIdx ) - 1;

		if ( nSuppIdx >= 0 )
			nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
		else
			nSuppNo = 0;

		CProcessedOrderArray OrderArray;

		if ( nSingleSpIdx == -1 )
			OrderArray.LoadDatabaseOrders( nSuppNo, nDbIdx, "", "", ORDER_TYPE_OPEN, TRUE );
		else
			OrderArray.LoadStockpointOrders( nSuppNo, nSingleSpIdx, "", "", ORDER_TYPE_OPEN, TRUE );
		
		CProcessedOrderEntity HeaderEntity;
		HeaderEntity.m_nOrderNo = 0;
			
		CProcessedOrderFilterArray orderFilter( OrderArray );
		orderFilter.SetSuppNo( nSuppNo );
		orderFilter.SetHeaderEntity( HeaderEntity );
		orderFilter.SetIncludePendingItemsFlag( TRUE );
		orderFilter.SetIncludeDeliveredItemsFlag( FALSE );
		orderFilter.BuildList();

		for ( int nOrderIdx = 0; nOrderIdx < orderFilter.GetSize(); nOrderIdx++ )
		{
			int nHeaderIdx = orderFilter.GetHeaderIndexAt( nOrderIdx );
			int nItemIdx = orderFilter.GetItemIndexAt( nOrderIdx );
		
			CProcessedOrderHeader HeaderRecord;
			OrderArray.GetHeaderRecord( nHeaderIdx, HeaderRecord );

			CProcessedOrderItem ItemRecord;
			OrderArray.GetItemRecord( nItemIdx, ItemRecord );
			
			if( m_strStockCodeFrom != "" && m_strStockCodeFrom > ItemRecord.GetStockCode() )
				continue;

			if ( m_strStockCodeTo != "" && m_strStockCodeTo < ItemRecord.GetStockCode() )
				continue;

			int nStockIdx = 0;
			int nCategory = 0;

			if ( DataManager.Stock.FindStockItemByCode( ItemRecord.GetStockCode(), nStockIdx ) == TRUE )
				nCategory = DataManager.Stock.GetCategory( nStockIdx );

			if ( CategorySet.GetCategoryStatus( nCategory ) == FALSE )
				continue;

			CString strDate = HeaderRecord.GetDateString();

			if ( ( bDateFilter == FALSE ) || ( ( strDate >= strCheckDateFrom ) && ( strDate <= strCheckDateTo ) ) )
			{
				double dSUCost = ItemRecord.GetDUCost() * ItemRecord.GetDUQty();
				
				CStockReportLine csvOut;
				CString strNum;
				strNum.Format ( "%6.6d", HeaderRecord.GetOrderNum() );
				csvOut.AppendString ( strNum );
				CSSDate date ( ItemRecord.GetDate() );
				csvOut.AppendString ( date.GetDate() );
				csvOut.AppendString ( ReportHelpers.GetDisplayStockCode ( ItemRecord.GetStockCode() ) );
				csvOut.AppendString ( ItemRecord.GetDescription() );
				csvOut.AppendString ( ItemRecord.GetSuppRef() );
				csvOut.AppendInt ( ItemRecord.GetDUQty() );

				CString strItem;
				strItem.Format ( "%d x %.3f", ItemRecord.GetDUItems(), ItemRecord.GetDUItemSize() );
				csvOut.AppendString ( strItem );

				csvOut.AppendStockQty ( ItemRecord.CalculateSUQty(), 1, TRUE );
				csvOut.AppendVal ( ItemRecord.GetDUCost() );
				csvOut.AppendVal ( dSUCost );

				CArray<int,int> arraySpIdx;

				for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
					if ( MatchStockpoint ( nSpIdx ) == TRUE )
						arraySpIdx.Add( nSpIdx );

				dbStockpoint.SortSpListByName( arraySpIdx );

				for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
				{
					int nSpIdx = arraySpIdx.GetAt(n);
			
					if ( ( nSingleSpIdx < 0 ) || ( nSingleSpIdx == nSpIdx ) )
					{
						CString strItem;
						int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
						strItem.Format ( "%d x %.3f", ItemRecord.GetStockpointOrder ( nSpNo ), ItemRecord.GetDUItemSize() );
						csvOut.AppendString ( strItem );
					}
					else
						csvOut.AppendString( "" );
				}
	
				{
					CReportConsolOnOrderCategory item;
					item.m_nCategory = nCategory;
					item.m_nOrderLine = nOrderLineCounter++;
					item.m_strData = csvOut.GetLine();
					arrayBuffer.Consolidate( item );
				}

				{
					CConsolidatedDoubleByInt item;
					item.m_nKey = nCategory;
					item.m_dVal = dSUCost;
					arrayCost.Consolidate( item );
				}
			}
		}
	}
		
	int nCurrentCategory = -1;
	for ( int n = 0; n < arrayBuffer.GetSize(); n++ )
	{
		CReportConsolOnOrderCategory item;
		arrayBuffer.GetAt( n, item );

		if ( item.m_nCategory != nCurrentCategory )
		{
			if ( nCurrentCategory != -1 )
			{
				int nCostPos = 0;
				
				CConsolidatedDoubleByInt itemCost;
				itemCost.m_nKey = nCurrentCategory;

				if ( arrayCost.Find( itemCost, nCostPos ) == TRUE )
				{
					arrayCost.GetAt( nCostPos, itemCost );
					WriteOrderTotal( ReportFile, itemCost.m_dVal );
				}
				else
					WriteOrderTotal( ReportFile, 0.0 );
			}

			nCurrentCategory = item.m_nCategory;

			CString strHeader = "<..>";
			strHeader += DataManager.Category.GetText( nCurrentCategory );
			strHeader += strConLevel;

			ReportFile.WriteLine( strHeader );
			ReportFile.WriteLine( "<LI>" );
		}

		ReportFile.WriteLine( item.m_strData );
	}

	if ( nCurrentCategory != -1 )
	{
		CConsolidatedDoubleByInt itemCost;
		itemCost.m_nKey = nCurrentCategory;

		int nCostPos = 0;
		if ( arrayCost.Find( itemCost, nCostPos ) == TRUE )
		{
			arrayCost.GetAt( nCostPos, itemCost );
			WriteOrderTotal( ReportFile, itemCost.m_dVal );
		}
		else
			WriteOrderTotal( ReportFile, 0.0 );
	}
		
	ReportFile.WriteLine ( "" );	
}
	
/**********************************************************************/

void CProcessedOrderReportOnOrderCategory::WriteOrderTotal ( CReportFile& ReportFile, double dTotal )
{
	ReportFile.WriteLine ( "\t\t\t\t\t\t\t\t\t<LI>" );
	CStockReportLine csvTotal;
	csvTotal.AppendBlanks ( 2 );
	csvTotal.AppendString ( "Category Total" );
	csvTotal.AppendBlanks ( 6 );
	csvTotal.AppendVal ( dTotal );
	ReportFile.WriteLine ( csvTotal.GetLine() );
	ReportFile.WriteLine ( "" );
}

/**********************************************************************/

	