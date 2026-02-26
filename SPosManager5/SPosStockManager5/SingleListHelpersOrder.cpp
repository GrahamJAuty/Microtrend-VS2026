/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ProcessedOrderFilterArray.h"
/**********************************************************************/
#include "SingleListHelpersOrder.h"
/**********************************************************************/

CSingleListContentHelpersOrderList::CSingleListContentHelpersOrderList( bool bClosed, bool& bFiltered ) : m_bFiltered( bFiltered )
{
	m_nSpIdx = -1;
	m_nDbIdx = 0;
	m_nOrderType = ( bClosed ) ? ORDER_TYPE_CLOSED : ORDER_TYPE_OPEN;
}

/**********************************************************************/

int CSingleListContentHelpersOrderList::GetArraySize()
{
	if ( m_bFiltered == FALSE )
		return m_arrayAll.GetSize();
	else
		return m_arrayFiltered.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersOrderList::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSupplierOrderInfo info;
		GetOrderInfo( nIndex, info );

		CCSV csv( '\t' );

		switch( info.m_nSuppIdx )
		{
		case -2:		
			csv.Add( "All Suppliers" );		
			break;
		
		case -1:		
			csv.Add( "Unspecified Supplier" );	
			break;

		default:		
			csv.Add( DataManager.Supplier.GetName( info.m_nSuppIdx ) );	
			break;
		}

		csv.Add( info.m_nLineCount );
	
		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersOrderList::BuildSupplierList()
{
	m_arrayAll.RemoveAll();
	m_arrayFiltered.RemoveAll();

	//SORT THE SUPPPLIER ALPHABETICALLY
	CReportConsolidationArray<CSortedIntByString> SortedArray( TRUE, 1 );
	for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
	{
		CSortedIntByString item;
		item.m_strItem = DataManager.Supplier.GetName ( nSuppIdx );
		item.m_nItem = nSuppIdx;
		SortedArray.Consolidate ( item );
	}

	//ADD THE SUPPLIER INFO IN SORTED ORDER
	int nTotalLines = 0;
	for ( int n = 0; n < SortedArray.GetSize(); n++ )
	{
		CSortedIntByString item;
		SortedArray.GetAt( n, item );

		int nSuppIdx = item.m_nItem;
		int nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
		
		CProcessedOrderArray arrayOrders;
		
		if ( m_nSpIdx < 0 )
			arrayOrders.LoadDatabaseOrders( nSuppNo, m_nDbIdx, "", "", m_nOrderType, TRUE );
		else
			arrayOrders.LoadStockpointOrders( nSuppNo, m_nSpIdx, "", "", m_nOrderType, TRUE );
			
		if ( arrayOrders.IsModified() == TRUE )
			arrayOrders.SaveOrders();

		CProcessedOrderFilterArray FilterArray( arrayOrders );
		FilterArray.SetSuppNo(-1);
		FilterArray.SetDescriptionFilter( "", 0, FALSE );
		FilterArray.SetIncludeDeliveredItemsFlag( ORDER_TYPE_CLOSED == m_nOrderType );
		FilterArray.SetIncludePendingItemsFlag( ORDER_TYPE_OPEN == m_nOrderType );
		FilterArray.BuildList();

		CSupplierOrderInfo info;
		info.m_nSuppIdx = nSuppIdx;
		info.m_nSuppNo = nSuppNo;
		info.m_nLineCount = FilterArray.GetSize();

		m_arrayAll.Add( info );
		
		if ( info.m_nLineCount > 0 ) 
		{
			m_arrayFiltered.Add( info );
			nTotalLines += info.m_nLineCount;
		}
	}

	//ADD THE UNSPECIFIED SUPPLIER
	{
		CProcessedOrderArray arrayOrders;
		
		if ( m_nSpIdx < 0 )
			arrayOrders.LoadDatabaseOrders( 0, m_nDbIdx, "", "", m_nOrderType, TRUE );
		else
			arrayOrders.LoadStockpointOrders( 0, m_nSpIdx, "", "", m_nOrderType, TRUE );
			
		if ( arrayOrders.IsModified() == TRUE )
			arrayOrders.SaveOrders();

		CProcessedOrderFilterArray FilterArray( arrayOrders );
		FilterArray.SetSuppNo(-1);
		FilterArray.SetDescriptionFilter( "", 0, FALSE );
		FilterArray.SetIncludeDeliveredItemsFlag( ORDER_TYPE_CLOSED == m_nOrderType );
		FilterArray.SetIncludePendingItemsFlag( ORDER_TYPE_OPEN == m_nOrderType );
		FilterArray.BuildList();

		CSupplierOrderInfo infoUnspecified;
		infoUnspecified.m_nSuppIdx = -1;
		infoUnspecified.m_nSuppNo = 0;
		infoUnspecified.m_nLineCount = FilterArray.GetSize();

		m_arrayAll.InsertAt( 0, infoUnspecified );
		
		if ( infoUnspecified.m_nLineCount > 0 ) 
		{
			m_arrayFiltered.InsertAt( 0, infoUnspecified );
			nTotalLines += infoUnspecified.m_nLineCount;
		}
	}
}

/**********************************************************************/

void CSingleListContentHelpersOrderList::GetOrderInfo( int nIndex, CSupplierOrderInfo& info )
{
	info.m_nSuppIdx = -1;
	info.m_nSuppNo = 0;
	info.m_nLineCount = 0;

	if ( m_bFiltered == FALSE )
	{
		if ( nIndex >= 0 && nIndex < m_arrayAll.GetSize() )
			info = m_arrayAll.GetAt( nIndex );
	}
	else
	{
		if ( nIndex >= 0 && nIndex < m_arrayFiltered.GetSize() )
			info = m_arrayFiltered.GetAt( nIndex );
	}
}

/**********************************************************************/

void CSingleListContentHelpersOrderList::UpdateOrderLines( int nSuppIdx )
{
	int nSuppNo = 0;
	if ( nSuppIdx >= 0 && nSuppIdx < DataManager.Supplier.GetSize() )
		nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );

	CProcessedOrderArray arrayOrders;

	if ( m_nSpIdx < 0 )
		arrayOrders.LoadDatabaseOrders( nSuppNo, m_nDbIdx, "", "", m_nOrderType, TRUE );
	else
		arrayOrders.LoadStockpointOrders( nSuppNo, m_nSpIdx, "", "", m_nOrderType, TRUE );
			
	CProcessedOrderFilterArray FilterArray( arrayOrders );
	FilterArray.SetSuppNo(-1);
	FilterArray.SetDescriptionFilter( "", 0, FALSE );
	FilterArray.SetIncludeDeliveredItemsFlag( ORDER_TYPE_CLOSED == m_nOrderType );
	FilterArray.SetIncludePendingItemsFlag( ORDER_TYPE_OPEN == m_nOrderType );
	FilterArray.BuildList();

	for ( int n = 0; n < m_arrayAll.GetSize(); n++ )
	{
		if ( m_arrayAll[n].m_nSuppIdx == nSuppIdx )
			m_arrayAll[n].m_nLineCount = FilterArray.GetSize();
	}

	for ( int n = 0; n < m_arrayFiltered.GetSize(); n++ )
	{
		if ( m_arrayFiltered[n].m_nSuppIdx == nSuppIdx )
			m_arrayFiltered[n].m_nLineCount = FilterArray.GetSize();
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSingleListContentHelpersOrderRepeat::CSingleListContentHelpersOrderRepeat()
{
	SetDbIdx(0);
}

/**********************************************************************/

void CSingleListContentHelpersOrderRepeat::SetDbIdx( int nDbIdx )
{
	m_nDbIdx = nDbIdx;
	m_strOrderFolder = dbDatabase.GetFolderPathDataOrdersRoot( nDbIdx );
}

/**********************************************************************/

int CSingleListContentHelpersOrderRepeat::GetArraySize()
{
	return m_arrayOrderInfo.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersOrderRepeat::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		if ( nIndex < 0 || nIndex >= m_arrayOrderInfo.GetSize() )
		return FALSE;

		CRepeatOrderInfo info = m_arrayOrderInfo.GetAt( nIndex );

		CString strName = info.m_strReference;
		if ( "" == strName )
			strName = "Previous Order";

		CCSV csvLine( '\t' );
		csvLine.Add( info.m_strDateDisplay );
		csvLine.Add( info.m_strTimeDisplay );
		csvLine.Add( strName );
		csvLine.Add( info.m_nLineCount );

		strLine = csvLine.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersOrderRepeat::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayOrderInfo.GetSize() ) )
	{
		CRepeatOrderInfo info = m_arrayOrderInfo.GetAt( nIndex );

		CString strDbIdx;
		strDbIdx.Format( "%4.4d", m_nDbIdx );

		CSortedStringItem item;
		item.m_strItem = strDbIdx + info.m_strDate + info.m_strTime;
		m_arrayDeleted.Consolidate( item );
	}
}

/**********************************************************************/

void CSingleListContentHelpersOrderRepeat::RebuildList()
{
	m_arrayOrderInfo.RemoveAll();

	CString strDbIdx;
	strDbIdx.Format( "%4.4d", m_nDbIdx );

	CReportConsolidationArray<CSortedStringByString> arrayOrderTemp;

	CFileFind finder;
	CString strPath = m_strOrderFolder + "\\";
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
		strFilename.MakeUpper();

		if ( strFilename.GetLength() != 24 )
			continue;

		if ( strFilename.Left(5) != "DATA_" )
			continue;

		if ( strFilename.Right(4) != ".DAT" )
			continue;

		if ( strFilename.Mid(13,1) != "_" )
			continue;

		CString strDate = strFilename.Mid(5,8);
		if ( ::TestNumeric( strDate ) == FALSE )
			continue;

		CString strTime = strFilename.Mid(14,6);
		if ( ::TestNumeric( strTime ) == FALSE )
			continue;

		CSortedStringItem itemString;
		itemString.m_strItem = strDbIdx + strDate + strTime;

		int nPos = 0;
		if ( m_arrayDeleted.Find( itemString, nPos ) == TRUE )
			continue;
		
		CString strOrderName = "";
		CString strOrderLine = "";
		int nConLevel = NODE_DATABASE;
		int nTableNo = 1;

		CString strInfoFilename = "INFO_";
		strInfoFilename += strFilename.Right(19);

		CSSFile fileInfo;
		if ( fileInfo.Open( strPath + strInfoFilename, "rb" ) == TRUE )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv( strBuffer );

			if ( csv.GetInt(0) == 1 )
			{
				strOrderName = csv.GetString(1);
				strOrderLine = csv.GetString(2);
				nConLevel = csv.GetInt(3);
				nTableNo = csv.GetInt(4);
			}
		}

		CCSV csv;
		csv.Add( strOrderName );
		csv.Add( strOrderLine );
		csv.Add( nConLevel );
		csv.Add( nTableNo );

		CSortedStringByString item;
		item.m_strKey = strDate + strTime;
		item.m_strData = csv.GetLine();
		arrayOrderTemp.Consolidate( item );
	}

	for ( int n = 0; n < arrayOrderTemp.GetSize(); n++ )
	{
		CSortedStringByString item;
		arrayOrderTemp.GetAt( n, item );
		CString strLabel = item.m_strKey;
		CString strOrder = item.m_strData;
		CCSV csvOrder( strOrder );

		CRepeatOrderInfo info;
		info.m_strDate = strLabel.Left(8);
		info.m_strTime = strLabel.Right(6);

		info.m_strDateDisplay.Format( "%s/%s/%s",
			(const char*) info.m_strDate.Right(2),
			(const char*) info.m_strDate.Mid(4,2),
			(const char*) info.m_strDate.Left(4) );

		info.m_strTimeDisplay.Format( "%s:%s",
			(const char*) info.m_strTime.Left(2),
			(const char*) info.m_strTime.Mid(2,2) );
		
		CString strFilename = info.m_strDate;
		strFilename += "_";
		strFilename += info.m_strTime;
		strFilename += ".dat";

		info.m_strInfoFilename = "INFO_";
		info.m_strInfoFilename += strFilename;

		info.m_strDataFilename = "DATA_";
		info.m_strDataFilename += strFilename;
		
		info.m_strReference = csvOrder.GetString(0);
		info.m_nLineCount = csvOrder.GetInt(1);
		info.m_nConLevel = csvOrder.GetInt(2);
		info.m_nTableNo = csvOrder.GetInt(3);

		m_arrayOrderInfo.Add( info );
	}
}

/**********************************************************************/

int CSingleListContentHelpersOrderRepeat::GetOrderInfoArraySize()
{
	return m_arrayOrderInfo.GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersOrderRepeat::GetOrderInfo( int nIndex, CRepeatOrderInfo& info )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayOrderInfo.GetSize() ) )
		info = m_arrayOrderInfo.GetAt( nIndex );
}

/**********************************************************************/

int CSingleListContentHelpersOrderRepeat::GetDeletedArraySize()
{
	return m_arrayDeleted.GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersOrderRepeat::GetDeletedItem( int nIndex, CSortedStringItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayDeleted.GetSize() ) )
		m_arrayDeleted.GetAt( nIndex, item );
}

/**********************************************************************/

void CSingleListContentHelpersOrderRepeat::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Date", LVCFMT_LEFT, 100 );
	list.InsertColumn ( 1, "Time", LVCFMT_LEFT, 60 );
	list.InsertColumn ( 2, "Order Name", LVCFMT_LEFT, 260 );
	list.InsertColumn ( 3, "Lines", LVCFMT_RIGHT, 60 );
}

/**********************************************************************/
