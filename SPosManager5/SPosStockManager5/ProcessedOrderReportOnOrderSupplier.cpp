/**********************************************************************/
#include "CCSVReportLine.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "ProcessedOrderFilterArray.h"
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrderReportOnOrderSupplier.h"
/**********************************************************************/

CProcessedOrderReportOnOrderSupplier::CProcessedOrderReportOnOrderSupplier( CWordArray& wSuppSelectArray, CProcessedOrderArray& OrderArray ) : CProcessedOrderBaseReport(), m_OrderArray( OrderArray )
{
	for ( int nIndex = 0; nIndex < wSuppSelectArray.GetSize(); nIndex++ )
		m_wSuppSelectArray.Add ( wSuppSelectArray.GetAt ( nIndex ) );

	m_bCheckAllOrderFolders = FALSE;
	m_nCategoryFilter = 0;
}

/**********************************************************************/

bool CProcessedOrderReportOnOrderSupplier::CreateReport( int nDbIdx )
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
	
		if ( TRUE == m_bCheckAllOrderFolders )
		{
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
		else
			WriteOrders ( ReportFile, nDbIdx, -2 );
		
		ShowStockpoints ( ReportFile );
		ReportFile.Close();
	}
	return bSuccess;
}
	
/**********************************************************************/

void CProcessedOrderReportOnOrderSupplier::WriteOrders( CReportFile& ReportFile, int nDbIdx, int nSingleSpIdx )
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

	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet( m_nCategoryFilter );

	//loop through all selected suppliers
	for ( int nSelectIdx = 0; nSelectIdx < m_wSuppSelectArray.GetSize(); nSelectIdx++)
	{
		CString strLine;
			
		CString strConLevel = "";

		if ( nSingleSpIdx == -1 )
			strConLevel.Format( ", %s", dbDatabase.GetName( nDbIdx ) );
		else if ( nSingleSpIdx >= 0 )
			strConLevel.Format( ", %s", dbStockpoint.GetName( nSingleSpIdx ) );

		int nSuppNo;
		int nSuppIdx = m_wSuppSelectArray.GetAt ( nSelectIdx ) - 1;

		if ( nSuppIdx >= 0 )
		{
			nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
			strLine.Format ( "<..>Supplier %s%s", 
				DataManager.Supplier.GetName ( nSuppIdx ),
				(const char*) strConLevel );
		}
		else
		{
			nSuppNo = 0;
			strLine.Format ( "<..>Unspecified Supplier%s",
				(const char*) strConLevel );
		}

		if ( TRUE == m_bCheckAllOrderFolders )
		{
			if ( nSingleSpIdx == -1 )
				m_OrderArray.LoadDatabaseOrders( nSuppNo, nDbIdx, "", "", ORDER_TYPE_OPEN, TRUE );
			else
				m_OrderArray.LoadStockpointOrders( nSuppNo, nSingleSpIdx, "", "", ORDER_TYPE_OPEN, TRUE );
		}

		CProcessedOrderEntity HeaderEntity;
		HeaderEntity.m_nOrderNo = 0;
			
		CProcessedOrderFilterArray orderFilter( m_OrderArray );
		orderFilter.SetSuppNo( nSuppNo );
		orderFilter.SetHeaderEntity( HeaderEntity );
		orderFilter.SetIncludePendingItemsFlag( TRUE );
		orderFilter.SetIncludeDeliveredItemsFlag( FALSE );
		orderFilter.BuildList();

		ReportFile.WriteLine ( strLine );
		ReportFile.WriteLine ( "<LI>" );
		
		bool bGotOrder = FALSE;
		double dTotalCost = 0.0;

		for ( int nOrderIdx = 0; nOrderIdx < orderFilter.GetSize(); nOrderIdx++ )
		{
			int nHeaderIdx = orderFilter.GetHeaderIndexAt( nOrderIdx );
			int nItemIdx = orderFilter.GetItemIndexAt( nOrderIdx );
		
			CProcessedOrderHeader HeaderRecord;
			m_OrderArray.GetHeaderRecord( nHeaderIdx, HeaderRecord );

			CProcessedOrderItem ItemRecord;
			m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );
			
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
				dTotalCost += dSUCost;
				bGotOrder = TRUE;

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

				ReportFile.WriteLine ( csvOut.GetLine() );
			}
		}
		if ( bGotOrder == TRUE )
			WriteOrderTotal ( ReportFile, dTotalCost );
			
		ReportFile.WriteLine ( "" );	
	}
}
	
/**********************************************************************/

void CProcessedOrderReportOnOrderSupplier::WriteOrderTotal ( CReportFile& ReportFile, double dTotal )
{
	ReportFile.WriteLine ( "\t\t\t\t\t\t\t\t\t<LI>" );
	CStockReportLine csvTotal;
	csvTotal.AppendBlanks ( 2 );
	csvTotal.AppendString ( "Supplier Total" );
	csvTotal.AppendBlanks ( 6 );
	csvTotal.AppendVal ( dTotal );
	ReportFile.WriteLine ( csvTotal.GetLine() );
	ReportFile.WriteLine ( "" );
}

/**********************************************************************/

	