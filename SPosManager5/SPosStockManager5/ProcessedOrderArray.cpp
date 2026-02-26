/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "ProcessedOrder.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrderArray.h"
/**********************************************************************/

CProcessedOrderArray::CProcessedOrderArray(void)
{
	m_nConLevel = NODE_DATABASE;
	m_nDbNo = 0;
	m_nSpNo = 0;
	m_nSuppNo = 0;
	m_bModified = FALSE;
}

/**********************************************************************/

bool CProcessedOrderArray::FindHeader( CProcessedOrderEntity& entity, int& nPos )
{
	return m_arrayOrderHeaders.Find( entity, nPos ); 
}

/**********************************************************************/

void CProcessedOrderArray::FindFirstItem( CProcessedOrderEntity& entity, int& nPos )
{
	int nSearchType = entity.m_nSearchType;
	entity.m_nSearchType = 0;
	m_arrayOrderItems.Find( entity, nPos );
	entity.m_nSearchType = nSearchType;
}

/**********************************************************************/

void CProcessedOrderArray::FindLastItem( CProcessedOrderEntity& entity, int& nPos )
{
	int nSearchType = entity.m_nSearchType;
	entity.m_nSearchType = 2;
	m_arrayOrderItems.Find( entity, nPos );
	entity.m_nSearchType = nSearchType;
	nPos--;
}

/**********************************************************************/

void CProcessedOrderArray::GetItemIdxRange( CProcessedOrderEntity& entity, int& nStartIdx, int& nEndIdx )
{
	FindFirstItem( entity, nStartIdx );
	FindLastItem( entity, nEndIdx );
}

/**********************************************************************/

void CProcessedOrderArray::GetHeaderRecord( int nIndex, CProcessedOrderHeader& header )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetOrderCount() ) )
	{
		CProcessedOrderEntity headerItem;
		m_arrayOrderHeaders.GetAt( nIndex, headerItem );
		CCSV csv( headerItem.m_strDetail );
		header.ConvertFromCSV( csv );
	}
}

/**********************************************************************/

void CProcessedOrderArray::GetHeaderEntity( int nIndex, CProcessedOrderEntity& entity )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetOrderCount() ) )
		m_arrayOrderHeaders.GetAt(nIndex, entity );
}

/**********************************************************************/

void CProcessedOrderArray::SetHeaderRecord( int nIndex, CProcessedOrderHeader& header )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetOrderCount() ) )
	{
		CCSV csv;
		header.ConvertToCSV( csv );

		CProcessedOrderEntity headerItem;
		m_arrayOrderHeaders.GetAt( nIndex, headerItem );
		headerItem.m_strDetail = csv.GetLine();
		m_arrayOrderHeaders.SetAt( nIndex, headerItem );
	}
}

/**********************************************************************/

void CProcessedOrderArray::GetItemRecord( int nIndex, CProcessedOrderItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetItemCount() ) )
	{
		CProcessedOrderEntity arrayItem;
		m_arrayOrderItems.GetAt( nIndex, arrayItem );
		CCSV csv( arrayItem.m_strDetail );
		item.ConvertFromCSV( csv );
	}
}

/**********************************************************************/

void CProcessedOrderArray::SetItemRecord( int nIndex, CProcessedOrderItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetItemCount() ) )
	{
		CCSV csv;
		item.ConvertToCSV( csv );

		CProcessedOrderEntity arrayItem;
		m_arrayOrderItems.GetAt( nIndex, arrayItem );
		arrayItem.m_strDetail = csv.GetLine();
		m_arrayOrderItems.SetAt( nIndex, arrayItem );
	}
}

/**********************************************************************/

void CProcessedOrderArray::CancelItemOrder( int nItemIdx )
{
	if ( ( nItemIdx < 0 ) || ( nItemIdx >= GetItemCount() ) )
		return;

	CProcessedOrderItem item;
	GetItemRecord( nItemIdx, item );

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( item.GetStockCode(), nStockIdx ) == FALSE )
		return;
	
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	//apply stock point changes
	item.ExpandStockpointOrders( nStockIdx, StockRecord );
	for ( int nIndex = 0; nIndex < item.GetStockpointCount(); nIndex++ )
	{
		double dSUQty = item.GetDUItemSize() * item.GetDUItems( nIndex );
	
		if ( dSUQty != 0.0 )
		{
			CStockLevelsCSVRecord StockLevels;
			int nSpNo = item.GetSpNo( nIndex );
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
			StockLevels.AddOrder ( -dSUQty );
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		}
	}
}

/**********************************************************************/

int CProcessedOrderArray::RemoveItem( CProcessedOrderEntity& entity, int nItemIdx )
{
	int nCount = 0;

	int nHeaderIdx;
	if ( FindHeader( entity, nHeaderIdx ) == TRUE )
	{
		int nStartIdx, nEndIdx;
		GetItemIdxRange( entity, nStartIdx, nEndIdx );

		if ( ( nItemIdx >= nStartIdx ) && ( nItemIdx <= nEndIdx ) )
		{
			CProcessedOrderItem item;
			GetItemRecord( nItemIdx, item );

			if ( item.GetDeliveredOldFlag() == FALSE )
			{
				CancelItemOrder( nItemIdx );
				MarkModifiedOrder( entity );
				m_arrayOrderItems.RemoveAt( nItemIdx );
				nCount++;
			}
		}
	}

	return nCount;
}

/**********************************************************************/

int CProcessedOrderArray::RemoveAllUndeliveredItems( CProcessedOrderEntity& entity )
{
	int nCount = 0;

	int nHeaderIdx;
	if ( FindHeader( entity, nHeaderIdx ) == TRUE )
	{
		int nStartIdx, nEndIdx;
		GetItemIdxRange( entity, nStartIdx, nEndIdx );

		for ( int nItemIdx = nEndIdx; nItemIdx >= nStartIdx; nItemIdx-- )
		{
			CProcessedOrderItem item;
			GetItemRecord( nItemIdx, item );

			if ( item.GetDeliveredOldFlag() == FALSE )
			{
				CancelItemOrder( nItemIdx );
				MarkModifiedOrder( entity );
				m_arrayOrderItems.RemoveAt( nItemIdx );
				nCount++;
			}
		}
	}

	return nCount;
}

/**********************************************************************/

int CProcessedOrderArray::RemoveAllUndeliveredItems()
{
	int nCount = 0;

	for ( int nHeaderIdx = 0; nHeaderIdx < GetOrderCount(); nHeaderIdx++ )
	{
		CProcessedOrderEntity entity;
		GetHeaderEntity( nHeaderIdx, entity );

		int nStartIdx, nEndIdx;
		GetItemIdxRange( entity, nStartIdx, nEndIdx );

		for ( int nItemIdx = nEndIdx; nItemIdx >= nStartIdx; nItemIdx-- )
		{
			CProcessedOrderItem item;
			GetItemRecord( nItemIdx, item );

			if ( item.GetDeliveredOldFlag() == FALSE )
			{
				CancelItemOrder( nItemIdx );
				MarkModifiedOrder( entity );
				m_arrayOrderItems.RemoveAt( nItemIdx );
				nCount++;
			}
		}
	}

	return nCount;
}

/**********************************************************************/

int CProcessedOrderArray::AddItem( CProcessedOrderEntity& entity, CProcessedOrderItem& item, int nInsertAfterIdx )
{
	CCSV csv;
	item.ConvertToCSV( csv );
	
	int nPos1, nPos2;
	FindFirstItem( entity, nPos1 );
	FindLastItem( entity, nPos2 );
	
	int nPos = 0;
	if ( ( nInsertAfterIdx >= nPos1 ) && ( nInsertAfterIdx < nPos2 ) )
		nPos = nInsertAfterIdx + 1;
	else
		nPos = nPos2 + 1;

	CProcessedOrderEntity itemEntity;
	itemEntity.m_nOrderNo = entity.m_nOrderNo;
	itemEntity.m_strDate = entity.m_strDate;
	itemEntity.m_strTime = entity.m_strTime;
	itemEntity.m_strDetail = csv.GetLine();
	itemEntity.m_nSearchType = 1;
	m_arrayOrderItems.InsertAt( nPos, itemEntity );

	return nPos;
}

/**********************************************************************/

void CProcessedOrderArray::AddHeader( CProcessedOrderEntity& entity, int nSuppNo )
{
	CProcessedOrderHeader header;
	header.SetDate();
	header.SetOrderNum( entity.m_nOrderNo );
	header.SetSuppNo( nSuppNo );
	header.SetModifiedFlag( TRUE );

	CCSV csv;
	header.ConvertToCSV( csv );

	entity.m_strDate = header.GetDateString();
	entity.m_strTime = header.GetTimeString();
	entity.m_strDetail = csv.GetLine();
	entity.m_nSearchType = 1;

	m_arrayOrderHeaders.Consolidate( entity );
}

/**********************************************************************/

void CProcessedOrderArray::GetOrder( CProcessedOrderEntity& entity, CProcessedOrder& Order )
{
	CString strDummy = "";
	Order.Initialise( m_nConLevel, m_nDbNo, m_nSpNo, entity.m_nOrderNo, m_nSuppNo, ORDER_TYPE_OPEN, strDummy, strDummy );

	int nPos = 0;
	if ( FindHeader( entity, nPos ) == TRUE )
	{
		CProcessedOrderHeader header;
		GetHeaderRecord( nPos, header );

		Order.SetHeader( header );

		int nStartIdx, nEndIdx;
		GetItemIdxRange( entity, nStartIdx, nEndIdx );

		for ( int nItemIdx = nStartIdx; nItemIdx <= nEndIdx; nItemIdx++ )
		{
			CProcessedOrderItem item;
			GetItemRecord( nItemIdx, item );
			Order.AddItem( item );
		}
	}
}

/**********************************************************************/

void CProcessedOrderArray::MarkModifiedOrder( CProcessedOrderEntity& entity )
{
	int nHeaderIdx;
	if ( FindHeader( entity, nHeaderIdx ) == TRUE )
	{
		m_bModified = TRUE;

		CProcessedOrderHeader header;
		GetHeaderRecord( nHeaderIdx, header );
		header.SetModifiedFlag( TRUE );
		SetHeaderRecord( nHeaderIdx, header );

		CString strDebugLog;
		strDebugLog.Format( "Modified Order Num %d Header Idx %d", entity.m_nOrderNo, nHeaderIdx );
		MicrotrendDebugMessageBox( strDebugLog );
	}
}

/**********************************************************************/

void CProcessedOrderArray::LoadDatabaseOrders( int nSuppNo, int nDbIdx, const char* szDateFrom, const char* szDateTo, int nOrderType, bool bLoadOrders )
{
	m_arrayOrderHeaders.RemoveAll();
	m_arrayOrderItems.RemoveAll();
	LoadOrders( nSuppNo, NODE_DATABASE, nDbIdx, szDateFrom, szDateTo, nOrderType, bLoadOrders );
}

/**********************************************************************/

void CProcessedOrderArray::LoadStockpointOrders( int nSuppNo, int nSpIdx, const char* szDateFrom, const char* szDateTo, int nOrderType, bool bLoadOrders )
{
	m_arrayOrderHeaders.RemoveAll();
	m_arrayOrderItems.RemoveAll();
	LoadOrders( nSuppNo, NODE_STOCKPOINT, nSpIdx, szDateFrom, szDateTo, nOrderType, bLoadOrders );
}

/**********************************************************************/

void CProcessedOrderArray::LoadOrders( int nSuppNo, int nConLevel, int nTableIdx, const char* szDateFrom, const char* szDateTo, int nOrderType, bool bLoadOrders )
{
	m_nConLevel = nConLevel;
	m_nSuppNo = nSuppNo;

	m_nDbNo = 0;
	m_nSpNo = 0;
	
	CString strPath = "";

	CString strDateFrom = szDateFrom;
	CString strDateTo = szDateTo;

	if ( strDateFrom.GetLength() == 10 )
	{
		CString strTemp = strDateFrom.Right(4) + strDateFrom.Mid(3,2) + strDateFrom.Left(2);
		strDateFrom = strTemp;
	}
	else
		strDateFrom = "";

	if ( strDateTo.GetLength() == 10 )
	{
		CString strTemp = strDateTo.Right(4) + strDateTo.Mid(3,2) + strDateTo.Left(2);
		strDateTo = strTemp;
	}
	else
		strDateTo = "";

	CWordArray arrayOrderTypes;

	switch( nOrderType )
	{
	case ORDER_TYPE_OPEN:
		arrayOrderTypes.Add( ORDER_TYPE_OPEN );
		arrayOrderTypes.Add( ORDER_TYPE_MIXED );
		break;

	case ORDER_TYPE_CLOSED:
		arrayOrderTypes.Add( ORDER_TYPE_MIXED );
		arrayOrderTypes.Add( ORDER_TYPE_CLOSED );
		break;
	}

	for ( int n = 0; n < arrayOrderTypes.GetSize(); n++ )
	{
		nOrderType = arrayOrderTypes.GetAt(n);

		if ( NODE_STOCKPOINT == nConLevel )
		{
			m_nDbNo = dbStockpoint.GetDbNo( nTableIdx );
			m_nSpNo = dbStockpoint.GetSpNo( nTableIdx );	
			strPath = dbStockpoint.GetFolderPathDataOrdersType( nTableIdx, m_nSuppNo, nOrderType );
		}
		else
		{
			m_nDbNo = dbDatabase.GetDbNo( nTableIdx );
			strPath = dbDatabase.GetFolderPathDataOrdersType( nTableIdx, m_nSuppNo, nOrderType );
		}

		if ( TRUE == bLoadOrders )
		{
			strPath += "\\*.*";

			CFileFind finder;
			BOOL bWorking = finder.FindFile ( strPath );

			while ( bWorking == TRUE )
			{
				bWorking = finder.FindNextFile();

				if ( finder.IsDots() == TRUE )
					continue;

				if ( finder.IsDirectory() == TRUE )
					continue;

				CString strFilename = finder.GetFileName();
				strFilename.MakeUpper();

				if ( strFilename.GetLength() != 26 )
					continue;

				if ( strFilename.Mid(6,1) != "-" )
					continue;

				if ( strFilename.Mid(15,1) != "-" )
					continue;

				if ( strFilename.Right(4) != ".DAT" )
					continue;

				CString strOrderNum = strFilename.Left(6);
				if  ( ::TestNumeric( strOrderNum ) == FALSE )
					continue;

				CString strDate = strFilename.Mid(7,8);
				if ( ::TestNumeric( strDate ) == FALSE )
					continue;

				CString strTime = strFilename.Mid(16,6);
				if ( ::TestNumeric( strTime ) == FALSE )
					continue;

				CProcessedOrderEntity headerEntity;
				headerEntity.m_nOrderNo = atoi( strOrderNum );
				headerEntity.m_strDate = strDate;
				headerEntity.m_strTime = strTime;
				headerEntity.m_nSearchType = 1;

				int nPos = 0;
				if ( m_arrayOrderHeaders.Find( headerEntity, nPos ) == TRUE )
					continue;

				CProcessedOrder Order;
				Order.Initialise( m_nConLevel, m_nDbNo, m_nSpNo, headerEntity.m_nOrderNo, m_nSuppNo, nOrderType, strDate, strTime );
				Order.ReadOrder();

				if ( Order.GetItemCount() > 0 )
				{
					CProcessedOrderHeader header;
					Order.GetHeader( header );
					
					header.SetModifiedFlag( FALSE );
					header.SetOrderType( nOrderType );
					header.SetDateString( strDate );
					header.SetTimeString( strTime );

					bool bAccept = TRUE;

					if ( ( strDateFrom != "" ) || ( strDateFrom != "" ) )
					{
						if ( strDateFrom != "" )
							if ( strDate < strDateFrom )
								bAccept = FALSE;
				
						if ( strDateTo != "" )
							if ( strDate > strDateTo )
								bAccept = FALSE;
					}

					if ( TRUE == bAccept )
					{
						bool bClosedLines = FALSE;
						bool bOpenLines = FALSE;

						for ( int nLineNo = 0; nLineNo < Order.GetItemCount(); nLineNo++ )
						{
							CProcessedOrderItem item;
							Order.GetItem( nLineNo, item );

							if ( item.GetDeliveredFlag() == TRUE )
							{
								bClosedLines = TRUE;
								item.SetDeliveredOldFlag();
							}
							else
							{
								bOpenLines = TRUE;
								item.ClearDeliveredOldFlag();
							}

							CCSV csv;
							item.ConvertToCSV( csv );
							
							int nPos;
							FindLastItem( headerEntity, nPos );

							CProcessedOrderEntity itemEntity;
							itemEntity.m_nOrderNo = headerEntity.m_nOrderNo;
							itemEntity.m_strDate = headerEntity.m_strDate;
							itemEntity.m_strTime = headerEntity.m_strTime;
							itemEntity.m_nSearchType = 1;
							itemEntity.m_strDetail = csv.GetLine();
							m_arrayOrderItems.InsertAt( nPos + 1, itemEntity );
						}

						//MARK ORDER ARRAY AND INDIVIDUAL HEADER AS MODIFIED
						//IF THE STATUS DEDUCED FROM THE ITEM LINES DOES NOT
						//AGREE WITH THE STATUS OF THE PARENT FOLDER.
						
						int nTypeToWrite = ORDER_TYPE_CLOSED;

						if ( TRUE == bOpenLines )
						{
							if ( TRUE == bClosedLines )
								nTypeToWrite = ORDER_TYPE_MIXED;
							else
								nTypeToWrite = ORDER_TYPE_OPEN;
						}

						if ( nTypeToWrite != nOrderType )
						{
							header.SetModifiedFlag( TRUE );
							m_bModified = TRUE;
						}

						CCSV csvHeader;
						header.ConvertToCSV( csvHeader );
						headerEntity.m_strDetail = csvHeader.GetLine();
						m_arrayOrderHeaders.Consolidate( headerEntity );						
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CProcessedOrderArray::SaveOrders()
{
	for ( int nHeaderIdx = 0; nHeaderIdx < GetOrderCount(); nHeaderIdx++ )
	{
		CProcessedOrderHeader HeaderRecord;
		CProcessedOrderEntity HeaderEntity;
		GetHeaderRecord( nHeaderIdx, HeaderRecord );
		GetHeaderEntity( nHeaderIdx, HeaderEntity );

		if ( HeaderRecord.IsModified() == TRUE )
		{
			CString strDate = HeaderRecord.GetDateString();
			CString strTime = HeaderRecord.GetTimeString();

			CProcessedOrder Order;
			Order.Initialise( m_nConLevel, m_nDbNo, m_nSpNo, 0, m_nSuppNo, ORDER_TYPE_OPEN, strDate, strTime );
			Order.SetHeader( HeaderRecord );
		
			CString strDebugLog;
			strDebugLog.Format( "Save Modified Order %d", HeaderEntity.m_nOrderNo );
			MicrotrendDebugMessageBox( strDebugLog );

			int nStartIdx, nEndIdx;
			GetItemIdxRange( HeaderEntity, nStartIdx, nEndIdx );

			if ( nEndIdx < nStartIdx )
			{
				CString strFilename = Order.GetOrderFilename();
				CFileRemover FileRemover( strFilename );
			}
			else
			{
				bool bOpenLines = FALSE;
				bool bClosedLines = FALSE;

				for ( int nItemIdx = nStartIdx; nItemIdx <= nEndIdx; nItemIdx++ )
				{
					CProcessedOrderItem item;
					GetItemRecord( nItemIdx, item );
					Order.AddItem( item );
					
					if ( item.GetDeliveredFlag() == TRUE )
						bClosedLines = TRUE;
					else
						bOpenLines = TRUE;
				}

				int nTypeToWrite = ORDER_TYPE_CLOSED;

				if ( TRUE == bOpenLines )
				{
					if ( TRUE == bClosedLines )
						nTypeToWrite = ORDER_TYPE_MIXED;
					else
						nTypeToWrite = ORDER_TYPE_OPEN;
				}

				Order.WriteOrder();

				if ( HeaderRecord.GetOrderType() != nTypeToWrite )
				{
					CString strOldFilename = Order.GetOrderFilename();

					HeaderRecord.SetOrderType( nTypeToWrite );
					Order.SetHeader( HeaderRecord );
					
					CString strNewFilename = Order.GetOrderFilename();

					rename( strOldFilename, strNewFilename );
				}
			}
		}
	}
}

/**********************************************************************/

bool CProcessedOrderArray::CheckForOpenOrders( int nSuppNo, int nConLevel, int nTableIdx )
{
	CString strPath = "";

	CWordArray arrayOrderTypes;
	arrayOrderTypes.Add( ORDER_TYPE_OPEN );
	arrayOrderTypes.Add( ORDER_TYPE_MIXED );
	
	for ( int n = 0; n < arrayOrderTypes.GetSize(); n++ )
	{
		int nOrderType = arrayOrderTypes.GetAt(n);

		if ( NODE_STOCKPOINT == nConLevel )
			strPath = dbStockpoint.GetFolderPathDataOrdersType( nTableIdx, nSuppNo, nOrderType );
		else
			strPath = dbDatabase.GetFolderPathDataOrdersType( nTableIdx, nSuppNo, nOrderType );
		
		strPath += "\\*.*";

		CFileFind finder;
		BOOL bWorking = finder.FindFile ( strPath );

		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();
			strFilename.MakeUpper();

			if ( strFilename.GetLength() != 26 )
				continue;

			if ( strFilename.Mid(6,1) != "-" )
				continue;

			if ( strFilename.Mid(15,1) != "-" )
				continue;

			if ( strFilename.Right(4) != ".DAT" )
				continue;

			CString strOrderNum = strFilename.Left(6);
			if  ( ::TestNumeric( strOrderNum ) == FALSE )
				continue;

			CString strDate = strFilename.Mid(7,8);
			if ( ::TestNumeric( strDate ) == FALSE )
				continue;

			CString strTime = strFilename.Mid(16,6);
			if ( ::TestNumeric( strTime ) == FALSE )
				continue;

			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/