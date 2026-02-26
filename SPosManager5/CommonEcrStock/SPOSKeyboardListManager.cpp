/**********************************************************************/
#include "SPOSKeyboardDefines.h"
#include "SPOSKeyboardKeyHelpers.h"
#include "SPOSKeyboardView.h"
/**********************************************************************/
#include "SPOSKeyboardListManager.h"
/**********************************************************************/

CSPOSKeyboardListManager::CSPOSKeyboardListManager()
{
	m_bIsModified = FALSE;
	m_strListNameExportFolder = "";
	m_nStoredPageListNo = -1;
	m_nStoredPagePageNo = -1;
}

/**********************************************************************/

void CSPOSKeyboardListManager::Reset()
{
	m_ScreenArray.RemoveAll();
	m_ListArray.RemoveAll();
	m_KeyArray.RemoveAll();
	m_StoredPageKeyArray.RemoveAll();
	m_bIsModified = FALSE;
}

/**********************************************************************/
//SCREENS
/**********************************************************************/

bool CSPOSKeyboardListManager::FindScreen( int nScreenNo, int& nScreenIdx )
{
	int nStart = 0;
	int nEnd = m_ScreenArray.GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nScreenIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = m_ScreenArray[nScreenIdx].GetScreenNo();
		
		if ( nArrayNo == nScreenNo )
			return TRUE;
		else if ( nArrayNo < nScreenNo )
			nStart = nScreenIdx + 1;
		else 
			nEnd = nScreenIdx - 1;
	}
	nScreenIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::FindOrInsertScreen( int nScreenNo, int& nScreenIdx )
{
	if ( FindScreen ( nScreenNo, nScreenIdx ) )
		return TRUE;

	if ( nScreenNo < 1 || nScreenNo > MAX_SCREEN_NO )
		return FALSE;

	CSPOSKeyboardScreenRecord ScreenRecord;
	ScreenRecord.SetScreenNo( nScreenNo );

	CString strName;
	strName.Format ( "Screen %d", nScreenNo );
	ScreenRecord.SetName( strName );
	
	m_ScreenArray.InsertAt ( nScreenIdx, ScreenRecord );
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GetNextFreeScreenNo( int nScreenNo, int& nFreeScreenNo )
{
	int nScreenIdx;
	int nScreenNoToTry = nScreenNo + 1;
	FindScreen( nScreenNoToTry, nScreenIdx );

	while ( nScreenIdx < m_ScreenArray.GetSize() )
	{
		if ( m_ScreenArray[nScreenIdx].GetScreenNo() != nScreenNoToTry )
			break;

		nScreenIdx++;
		nScreenNoToTry++;
	}

	if ( nScreenNoToTry > MAX_SCREEN_NO )
	{
		nFreeScreenNo = 0;
		return FALSE;
	}

	nFreeScreenNo = nScreenNoToTry;
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::CopyScreen( int nSourceScreenNo, int nDestScreenNo )
{
	int nSourceIdx;
	if ( FindScreen( nSourceScreenNo, nSourceIdx ) == FALSE )
		return FALSE;

	//MUST DO THIS BEFORE WE INSERT THE NEW SCREEN
	CSPOSKeyboardScreenRecord ScreenRecord;
	GetScreenRecord( nSourceIdx, ScreenRecord );
	
	int nDestIdx;
	if ( FindScreen( nDestScreenNo, nDestIdx ) == TRUE )
		return FALSE;

	if ( FindOrInsertScreen( nDestScreenNo, nDestIdx ) == FALSE )
		return FALSE;

	ScreenRecord.SetScreenNo( nDestScreenNo );
	SetScreenRecord( nDestIdx, ScreenRecord );

	return TRUE;
}
	
/**********************************************************************/

bool CSPOSKeyboardListManager::DeleteScreen( int nScreenIdx )
{
	if ( nScreenIdx < 0 || nScreenIdx >= m_ScreenArray.GetSize() )
		return FALSE;

	m_ScreenArray.RemoveAt( nScreenIdx );
	return TRUE;
}

/**********************************************************************/

int CSPOSKeyboardListManager::GetScreenCount()
{
	return m_ScreenArray.GetSize();
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GetScreenRecord( int nScreenIdx, CSPOSKeyboardScreenRecord& ScreenRecord )
{
	if ( nScreenIdx < 0 || nScreenIdx >= m_ScreenArray.GetSize() )
	{
		ScreenRecord.Reset();
		return FALSE;
	}
	else
	{
		ScreenRecord = m_ScreenArray.GetAt( nScreenIdx );
		return TRUE;
	}
}

/**********************************************************************/

bool CSPOSKeyboardListManager::SetScreenRecord( int nScreenIdx, CSPOSKeyboardScreenRecord& ScreenRecord )
{
	if ( nScreenIdx < 0 || nScreenIdx >= m_ScreenArray.GetSize() )
		return FALSE;
	
	m_ScreenArray.SetAt( nScreenIdx, ScreenRecord );
	return TRUE;
}

/**********************************************************************/
//LISTS
/**********************************************************************/

bool CSPOSKeyboardListManager::FindList( __int64 nListNo, int& nListIdx )
{
	int nStart = 0;
	int nEnd = m_ListArray.GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nListIdx = ( nStart + nEnd ) / 2;
		__int64 nArrayNo = m_ListArray[nListIdx].GetListNo();
		
		if ( nArrayNo == nListNo )
			return TRUE;
		else if ( nArrayNo < nListNo )
			nStart = nListIdx + 1;
		else 
			nEnd = nListIdx - 1;
	}
	nListIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::FindOrInsertList( __int64 nListNo, int& nListIdx )
{
	if ( FindList ( nListNo, nListIdx ) )
		return TRUE;

	if ( nListNo < 1 || nListNo > MAX_LIST_NO )
		return FALSE;

	CSPOSKeyboardListRecord ListRecord;
	ListRecord.SetListNo( nListNo );

	CString strName;
	strName.Format ( "List %d", nListNo );
	ListRecord.SetName( strName );
	
	m_ListArray.InsertAt ( nListIdx, ListRecord );
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GetNextFreeListNo( __int64 nListNo, __int64& nFreeListNo )
{
	int nListIdx;
	__int64 nListNoToTry = nListNo + 1;
	FindList( nListNoToTry, nListIdx );

	while ( nListIdx < m_ListArray.GetSize() )
	{
		if ( m_ListArray[nListIdx].GetListNo() != nListNoToTry )
			break;

		nListIdx++;
		nListNoToTry++;
	}

	if ( nListNoToTry > MAX_LIST_NO )
	{
		nFreeListNo = 0;
		return FALSE;
	}

	nFreeListNo = nListNoToTry;
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::CopyList( __int64 nSourceListNo, __int64 nDestListNo )
{
	int nSourceIdx;
	if ( FindList( nSourceListNo, nSourceIdx ) == FALSE )
		return FALSE;

	//MUST DO THIS BEFORE WE INSERT THE NEW LIST!
	CSPOSKeyboardListRecord ListRecord;
	GetListRecord( nSourceIdx, ListRecord );

	int nDestIdx;
	if ( FindList( nDestListNo, nDestIdx ) == TRUE )
		return FALSE;

	if ( FindOrInsertList( nDestListNo, nDestIdx ) == FALSE )
		return FALSE;

	ListRecord.SetListNo( nDestListNo );
	SetListRecord( nDestIdx, ListRecord );

	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> KeyBuffer;

	int nStartKeyIdx, nEndKeyIdx;
	GetListKeyRange( nSourceListNo, nStartKeyIdx, nEndKeyIdx );
	
	for ( int n = nStartKeyIdx; n < nEndKeyIdx; n++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord;
		GetKeyRecord( n, KeyRecord );
		KeyBuffer.Add( KeyRecord );
	}

	GetListKeyRange( nDestListNo, nStartKeyIdx, nEndKeyIdx );

	for ( int n = nEndKeyIdx - 1; n >= nStartKeyIdx; n-- )
		m_KeyArray.RemoveAt(n);

	for ( int n = 0; n < KeyBuffer.GetSize(); n++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord = KeyBuffer.GetAt(n);
		KeyRecord.SetListNo( nDestListNo );
		m_KeyArray.InsertAt( nStartKeyIdx + n, KeyRecord );
	}

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::DeleteList( int nListIdx )
{
	if ( nListIdx < 0 || nListIdx >= m_ListArray.GetSize() )
		return FALSE;

	__int64 nListNo = m_ListArray[nListIdx].GetListNo();
	m_ListArray.RemoveAt( nListIdx );
	
	for ( int n = 0; n < m_ScreenArray.GetSize(); n++ )
	{
		CSPOSKeyboardScreenRecord ScreenRecord;
		GetScreenRecord( n, ScreenRecord );

		if ( ScreenRecord.GetListFastBar() == nListNo )			
			ScreenRecord.SetListFastBar( 0 );

		if ( ScreenRecord.GetTradingListTopLevel() == nListNo )	
			ScreenRecord.SetTradingListTopLevel( 0 );

		if ( ScreenRecord.GetTradingListFunction() == nListNo )	
			ScreenRecord.SetTradingListFunction( 0 );

		if ( ScreenRecord.GetPaymentListTopLevel() == nListNo )	
			ScreenRecord.SetPaymentListTopLevel( 0 );

		if ( ScreenRecord.GetPaymentListFunction() == nListNo )	
			ScreenRecord.SetPaymentListFunction( 0 );

		if ( ScreenRecord.GetLoggedOffListFunction() == nListNo )	
			ScreenRecord.SetLoggedOffListFunction( 0 );

		if ( ScreenRecord.GetLoggedOffListFunctionLarge() == nListNo )	
			ScreenRecord.SetLoggedOffListFunctionLarge( 0 );
		
		SetScreenRecord( n, ScreenRecord );
	}
	
	int nStartIdx, nEndIdx;
	GetListKeyRange( nListNo, nStartIdx, nEndIdx );
	
	for ( int k = nEndIdx - 1; k >= nStartIdx; k-- )
		DeleteKey( k );

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::InitialiseKeysForList( int nListIdx )
{
	if ( nListIdx < 0 || nListIdx >= m_ListArray.GetSize() )
		return FALSE;

	__int64 nListNo = m_ListArray[nListIdx].GetListNo();
	if ( nListNo < 0 || nListNo > MAX_LIST_NO )
		return FALSE;
	
	int nKeysPerPage = m_ListArray[nListIdx].GetKeysPerPage();

	int nKeyStartIdx, nKeyEndIdx;
	GetListKeyRange( nListNo, nKeyStartIdx, nKeyEndIdx );
	
	while ( nKeyEndIdx > nKeyStartIdx )
		m_KeyArray.RemoveAt( --nKeyEndIdx );
				
	for ( int nPageNo = 1; nPageNo <= m_ListArray[nListIdx].GetPageCount(); nPageNo++ )
	{
		for ( int nKeyNo = 1; nKeyNo <= m_ListArray[nListIdx].GetKeysPerPage(); nKeyNo++ )
		{
			CSPOSKeyboardKeyRecord KeyRecord;
			KeyRecord.SetListNo( nListNo );
			KeyRecord.SetPageNo( nPageNo );
			KeyRecord.SetKeyNo( nKeyNo );
			KeyRecord.SetBoldFlag( SPOSKeyboardManager.GetLayoutOptions() -> GetDefaultBoldFlag() );
			m_KeyArray.InsertAt( nKeyStartIdx++, KeyRecord );
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::UpdateKeysForList( int nListIdx, int nOldFormat )
{
	if ( nListIdx < 0 || nListIdx >= m_ListArray.GetSize() )
		return FALSE;

	if ( m_ListArray[nListIdx].GetFormat() == nOldFormat )
		return TRUE;

	int nOldRows = nOldFormat / 100;
	int nOldCols = nOldFormat % 100;

	int nNewFormat = m_ListArray[nListIdx].GetFormat();
	int nNewRows = nNewFormat / 100;
	int nNewCols = nNewFormat % 100;
	
	__int64 nListNo = m_ListArray[nListIdx].GetListNo();
	int nPageCount = m_ListArray[nListIdx].GetPageCount();

	for ( int nPageNo = 1; nPageNo <= nPageCount; nPageNo++ )
	{
		int nKeyStartIdx, nKeyEndIdx;
		GetPageKeyRange( nListNo, nPageNo, nKeyStartIdx, nKeyEndIdx );

		CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> arrayOldKeys;

		int nKeyIdxLoop;
		for ( int nKeyIdxLoop = nKeyStartIdx; nKeyIdxLoop < nKeyEndIdx; nKeyIdxLoop++ )
		{
			CSPOSKeyboardKeyRecord KeyRecord;
			GetKeyRecord( nKeyIdxLoop, KeyRecord );
			arrayOldKeys.Add( KeyRecord );
		}

		while ( nKeyEndIdx > nKeyStartIdx )
			m_KeyArray.RemoveAt( --nKeyEndIdx );
	
		int nKeyNo = 1;
		nKeyIdxLoop = nKeyStartIdx;

		for ( int nRow = 0; nRow < nNewRows; nRow++ )
		{
			for ( int nCol = 0; nCol < nNewCols; nCol++ )
			{
				CSPOSKeyboardKeyRecord KeyRecord;

				if ( nRow < nOldRows && nCol < nOldCols )
				{
					int nOldKeyIdx = ( nRow * nOldCols ) + nCol;

					if ( nOldKeyIdx < arrayOldKeys.GetSize() )
						KeyRecord = arrayOldKeys.GetAt( nOldKeyIdx );
				}

				KeyRecord.SetListNo( nListNo );
				KeyRecord.SetPageNo( nPageNo );
				KeyRecord.SetKeyNo( nKeyNo++ );
				m_KeyArray.InsertAt( nKeyIdxLoop++, KeyRecord );
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GetKeyExtentMap( __int64 nListNo, int nPageNo, CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent>& map )
{
	map.RemoveAll();

	int nListIdx;
	if ( FindList( nListNo, nListIdx ) == FALSE )
		return FALSE;

	CSPOSKeyboardListRecord ListRecord;
	GetListRecord( nListIdx, ListRecord );
	
	int nKeyCount = ListRecord.GetKeysPerPage();
	int nRowMax = ListRecord.GetRowCount();
	int nColMax = ListRecord.GetColumnCount();

	int nKeyStartIdx, nKeyEndIdx;
	GetPageKeyRange( nListNo, nPageNo, nKeyStartIdx, nKeyEndIdx );

	if( nKeyEndIdx != nKeyStartIdx + nKeyCount )
		return FALSE;

	for ( int nKeyIdxLoop = nKeyStartIdx; nKeyIdxLoop < nKeyEndIdx; nKeyIdxLoop++ )
		m_KeyArray[ nKeyIdxLoop ].SetTempMarker( FALSE );
	
	int nKeyIdx = nKeyStartIdx;
	
	for ( int nRow = 0; nRow < nRowMax; nRow++ )
	{
		for ( int nCol = 0; nCol < nColMax; nCol++ )
		{
			if ( m_KeyArray[ nKeyIdx ].GetTempMarker() == FALSE )
			{
				//CHECK THAT THE ROW AND COLUMN SIZE FOR THIS KEY IS VALID
				int nRowSize = m_KeyArray[nKeyIdx].GetRowSize();
				int nColSize = m_KeyArray[nKeyIdx].GetColSize();

				if ( nRowSize <= 0 ) nRowSize = 1;
				if ( nColSize <= 0 ) nColSize = 1;

				if ( nRow + nRowSize > nRowMax )
					nRowSize = nRowMax - nRow;

				if ( nCol + nColSize > nColMax )
					nColSize = nColMax - nCol;

				//CHECK THAT NONE OF THE KEYS IN THIS GRID ARE ALREADY MARKED
				bool bFoundClash = FALSE;
				int nKeyCheckIdx = nKeyIdx;
				for ( int nRowOff = 0; ( nRowOff < nRowSize ) && ( bFoundClash == FALSE); nRowOff++ )
				{
					for ( int nColOff = 0; ( nColOff < nColSize ) && ( bFoundClash == FALSE); nColOff++ )
						bFoundClash = m_KeyArray[ nKeyCheckIdx + nColOff ].GetTempMarker();

					nKeyCheckIdx += nColMax;
				}

				if ( bFoundClash == TRUE )
				{
					m_KeyArray[ nKeyIdx ].SetTempMarker ( TRUE );
					m_KeyArray[ nKeyIdx ].SetRowSize ( 0 );
					m_KeyArray[ nKeyIdx ].SetColSize ( 0 );
				
					CSPOSKeyboardKeyExtent item;
					item.m_nKeyNo = ( nKeyIdx - nKeyStartIdx ) + 1;
					item.m_nRowStart = nRow;
					item.m_nColStart = nCol;
					item.m_nRowSize = 1;
					item.m_nColSize = 1;

					map.Add( item );
				}
				else
				{
					int nKeyCheckIdx = nKeyIdx;
					for ( int nRowOff = 0; nRowOff < nRowSize; nRowOff++ )
					{
						for ( int nColOff = 0; nColOff < nColSize; nColOff++ )
						{
							m_KeyArray[ nKeyCheckIdx + nColOff ].SetTempMarker ( TRUE );
							m_KeyArray[ nKeyCheckIdx + nColOff ].SetColSize ( 0 );
							m_KeyArray[ nKeyCheckIdx + nColOff ].SetRowSize ( 0 );
						}
						nKeyCheckIdx += nColMax;
					}

					m_KeyArray[ nKeyIdx ].SetTempMarker ( TRUE );
					m_KeyArray[ nKeyIdx ].SetColSize ( nColSize );
					m_KeyArray[ nKeyIdx ].SetRowSize ( nRowSize );

					CSPOSKeyboardKeyExtent item;
					item.m_nKeyNo = ( nKeyIdx - nKeyStartIdx ) + 1;
					item.m_nRowStart = nRow;
					item.m_nColStart = nCol;
					item.m_nRowSize = nRowSize;
					item.m_nColSize = nColSize;

					map.Add( item );
				}
			}
			nKeyIdx++;
		}
	}

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardListManager::GetListComboInfo( int nMode, CArray<CSPOSKeyboardListComboItem,CSPOSKeyboardListComboItem>& array )
{
	array.RemoveAll();

	switch ( nMode )
	{
	case VIEWMODE_LIST_ITEM_STANDARD:
	case VIEWMODE_LIST_ITEM_LARGE:
	case VIEWMODE_LIST_FUNCTION_STANDARD:
	case VIEWMODE_LIST_FUNCTION_LARGE:
	case VIEWMODE_LIST_FUNCTION_BOTTOM:
		{
			for ( int n = 0; n < m_ListArray.GetSize(); n++ )
			{
				if ( m_ListArray[n].MatchViewModeType( nMode ) == TRUE )
				{
					CSPOSKeyboardListComboItem item;
					item.m_nListNo = m_ListArray[n].GetListNo();
					item.m_nPageNo = 0;
					item.m_nScreenNo = 0;
					item.m_nScreenListType = 0;

					CSPOSKeyboardKeyHelpers KeyHelpers;
					item.m_strText = KeyHelpers.GetCombiText( KEY_TYPE_LIST, item.m_nListNo );

					array.Add( item );
				}
			}
		}
		break;

	case VIEWMODE_SCREEN:
		{
			for ( int n = 0; n < m_ScreenArray.GetSize(); n++ )
			{
				CString strScreenNo;
				strScreenNo.Format ( "S%5.5d,",
					m_ScreenArray[n].GetScreenNo() % 100000 );

				CSPOSKeyboardListComboItem item;
				item.m_nScreenNo = m_ScreenArray[n].GetScreenNo();
				item.m_nPageNo = 0;

				item.m_nScreenListType = SCREENLIST_TYPE_TRADING_TOPLEVEL;
				item.m_nListNo = m_ScreenArray[n].GetTradingListTopLevel();
				item.m_strText = strScreenNo + "Trading,(Top Level)";
				array.Add( item );

				item.m_nScreenListType = SCREENLIST_TYPE_TRADING_FUNCTION;
				item.m_nListNo = m_ScreenArray[n].GetTradingListFunction();
				item.m_strText = strScreenNo + "Trading,(Function)";
				array.Add( item );

				if ( m_ScreenArray[n].GetLoggedOffListTypeFunction() == LISTTYPE_FUNCTION_STANDARD )
				{
					item.m_nScreenListType = SCREENLIST_TYPE_LOGGEDOFF_FUNCTION;
					item.m_nListNo = m_ScreenArray[n].GetLoggedOffListFunction();
				}
				else
				{
					item.m_nScreenListType = SCREENLIST_TYPE_LOGGEDOFF_FUNCTION_LARGE;
					item.m_nListNo = m_ScreenArray[n].GetLoggedOffListFunctionLarge();
				}

				item.m_strText = strScreenNo + "Logged Off,(Function)";
				array.Add( item );

				item.m_nScreenListType = SCREENLIST_TYPE_PAYMENT_TOPLEVEL;
				item.m_nListNo = m_ScreenArray[n].GetPaymentListTopLevel();
				item.m_strText = strScreenNo + "Payment,(Top Level)";
				array.Add( item );

				item.m_nScreenListType = SCREENLIST_TYPE_PAYMENT_FUNCTION;
				item.m_nListNo = m_ScreenArray[n].GetPaymentListFunction();
				item.m_strText = strScreenNo + "Payment,(Function)";
				array.Add( item );
			}
		}
		break;
	}
}

/**********************************************************************/
	
int CSPOSKeyboardListManager::GetListCount()
{
	return m_ListArray.GetSize();
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GetListRecord( int nListIdx, CSPOSKeyboardListRecord& ListRecord )
{
	if ( nListIdx < 0 || nListIdx >= m_ListArray.GetSize() )
	{
		ListRecord.Reset();
		return FALSE;
	}
	else
	{
		ListRecord = m_ListArray.GetAt( nListIdx );
		return TRUE;
	}
}

/**********************************************************************/

bool CSPOSKeyboardListManager::SetListRecord( int nListIdx, CSPOSKeyboardListRecord& ListRecord )
{
	if ( nListIdx < 0 || nListIdx >= m_ListArray.GetSize() )
		return FALSE;
	
	m_ListArray.SetAt( nListIdx, ListRecord );
	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardListManager::WriteListNames()
{
	//CREATE A FILE OF LIST NAMES FOR USE WHEN EDITING PLU
	//ITEMS IN ECR/STOCK MANAGER
	if (m_strListNameExportFolder != "")
	{
		CSSFile fileListNames;
		if (fileListNames.Open(m_strListNameExportFolder + "\\listname.dat", "wb") == TRUE)
		{
			for (int nListIdx = 0; nListIdx < m_ListArray.GetSize(); nListIdx++)
			{
				CSPOSKeyboardListRecord ListRecord;
				GetListRecord(nListIdx, ListRecord);

				CCSV csvOut;
				csvOut.Add(ListRecord.GetListNo());
				csvOut.Add(ListRecord.GetName());
				csvOut.Add(ListRecord.GetOfferPluLinkFlag() ? 0 : 1);

				fileListNames.WriteLine(csvOut.GetLine());
			}
		}
	}

	CString strKeyboardListPath = SPOSKeyboardManager.GetLayoutOptions()->GetDataPath();
	strKeyboardListPath += "\\listname.dat";

	CSSFile fileListNames;
	if (fileListNames.Open(strKeyboardListPath, "wb") == TRUE)
	{
		for (int nListIdx = 0; nListIdx < m_ListArray.GetSize(); nListIdx++)
		{
			CSPOSKeyboardListRecord ListRecord;
			GetListRecord(nListIdx, ListRecord);

			CCSV csvOut;
			csvOut.Add(ListRecord.GetListNo());
			csvOut.Add(ListRecord.GetName());
			csvOut.Add(ListRecord.GetOfferPluLinkFlag() ? 0 : 1);

			fileListNames.WriteLine(csvOut.GetLine());
		}
	}
}

/**********************************************************************/
//KEYS
/**********************************************************************/

bool CSPOSKeyboardListManager::FindKey( __int64 nListNo, int nPageNo, int nKeyNo, int& nKeyIdx )
{
	int nStart = 0;
	int nEnd = m_KeyArray.GetSize() - 1;

	CSPOSKeyboardKeyRecord KeyRecord;
	KeyRecord.SetListNo( nListNo );
	KeyRecord.SetPageNo( nPageNo );
	KeyRecord.SetKeyNo( nKeyNo );
	__int64 nLookUpKey = KeyRecord.GetLookUpKey();

	while ( nStart <= nEnd )
	{
		nKeyIdx = ( nStart + nEnd ) / 2;
		__int64 nArrayNo = m_KeyArray[nKeyIdx].GetLookUpKey();
		
		if ( nArrayNo == nLookUpKey )
			return TRUE;
		else if ( nArrayNo < nLookUpKey )
			nStart = nKeyIdx + 1;
		else 
			nEnd = nKeyIdx - 1;
	}
	nKeyIdx = nStart;
	return FALSE;
}

/**********************************************************************/

void CSPOSKeyboardListManager::GetPageKeyRange( __int64 nListNo, int nPageNo, int& nStartIdx, int& nEndIdx )
{
	FindKey( nListNo, nPageNo, 0, nStartIdx );

	nPageNo++;
	if ( nPageNo > MAX_LIST_PAGES )
	{
		nListNo++;
		nPageNo = 1;
	}
	
	FindKey( nListNo, nPageNo, 0, nEndIdx );
}

/**********************************************************************/

void CSPOSKeyboardListManager::GetListKeyRange( __int64 nListNo, int& nStartIdx, int& nEndIdx )
{
	FindKey( nListNo, 0, 0, nStartIdx );
	FindKey( nListNo + 1, 0, 0, nEndIdx );
}

/**********************************************************************/

bool CSPOSKeyboardListManager::FindOrInsertKey( __int64 nListNo, int nPageNo, int nKeyNo, int& nKeyIdx )
{
	if ( nPageNo < 1 || nPageNo > MAX_LIST_PAGES )
		return FALSE;

	if ( nKeyNo < 1 || nKeyNo > POS_KEY_COUNT )
		return FALSE;

	if ( FindKey ( nListNo, nPageNo, nKeyNo, nKeyIdx ) )
		return TRUE;

	CSPOSKeyboardKeyRecord KeyRecord;
	KeyRecord.SetListNo( nListNo );
	KeyRecord.SetPageNo( nPageNo );
	KeyRecord.SetKeyNo( nKeyNo );

	m_KeyArray.InsertAt ( nKeyIdx, KeyRecord );
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::DeleteKey( int nKeyIdx )
{
	if ( nKeyIdx < 0 || nKeyIdx >= m_KeyArray.GetSize() )
		return FALSE;

	m_KeyArray.RemoveAt( nKeyIdx );
	return TRUE;
}

/**********************************************************************/

int CSPOSKeyboardListManager::GetKeyCount()
{
	return m_KeyArray.GetSize();
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GetKeyRecord( int nKeyIdx, CSPOSKeyboardKeyRecord& KeyRecord )
{
	if ( nKeyIdx < 0 || nKeyIdx >= m_KeyArray.GetSize() )
	{
		KeyRecord.Reset();
		return FALSE;
	}
	else
	{
		KeyRecord = m_KeyArray.GetAt( nKeyIdx );
		return TRUE;
	}
}

/**********************************************************************/

bool CSPOSKeyboardListManager::SetKeyRecord( int nKeyIdx, CSPOSKeyboardKeyRecord& KeyRecord )
{
	if ( nKeyIdx < 0 || nKeyIdx >= m_KeyArray.GetSize() )
		return FALSE;
	
	m_KeyArray.SetAt( nKeyIdx, KeyRecord );
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::SplitKey( __int64 nListNo, int nPageNo, int nKeyNo )
{
	int nListIdx;
	if ( FindList( nListNo, nListIdx ) == FALSE )
		return FALSE;

	CSPOSKeyboardListRecord ListRecord;
	GetListRecord( nListIdx, ListRecord );
	
	int nKeyCount = ListRecord.GetKeysPerPage();
	int nRowMax = ListRecord.GetRowCount();
	int nColMax = ListRecord.GetColumnCount();

	if ( nColMax == 0 || nRowMax == 0 )
		return FALSE;

	int nKeyStartIdx, nKeyEndIdx;
	GetPageKeyRange( nListNo, nPageNo, nKeyStartIdx, nKeyEndIdx );

	if( nKeyEndIdx != nKeyStartIdx + nKeyCount )
		return FALSE;

	int nRow = ( nKeyNo - 1 ) / nColMax;
	int nCol = ( nKeyNo - 1 ) % nColMax;

	int nKeyIdx = nKeyStartIdx + nKeyNo - 1;

	CSPOSKeyboardKeyRecord KeyRecord;
	GetKeyRecord( nKeyIdx, KeyRecord );

	int nRowSize = KeyRecord.GetRowSize();
	int nColSize = KeyRecord.GetColSize();

	if ( nRowSize <= 0 ) nRowSize = 1;
	if ( nColSize <= 0 ) nColSize = 1;

	if ( ( nRow + nRowSize > nRowMax ) || ( nCol + nColSize > nColMax ) )
	{
		nRowSize = 1;
		nColSize = 1;
	}

	for ( int nRowOff = 0; nRowOff < nRowSize; nRowOff++ )
	{
		for ( int nColOff = 0; nColOff < nColSize; nColOff++ )
		{
			m_KeyArray[ nKeyIdx + nColOff ].SetColSize( 0 );
			m_KeyArray[ nKeyIdx + nColOff ].SetRowSize( 0 );

			if ( nRowOff != 0 || nColOff != 0 )
			{
				m_KeyArray[ nKeyIdx + nColOff ].Reset();
				m_KeyArray[ nKeyIdx + nColOff ].SetListNo( nListNo );
				m_KeyArray[ nKeyIdx + nColOff ].SetPageNo( nPageNo );
				m_KeyArray[ nKeyIdx + nColOff ].SetKeyNo( ( nKeyIdx + nColOff + 1 - nKeyStartIdx ) );
				m_KeyArray[ nKeyIdx + nColOff ].SetBoldFlag( SPOSKeyboardManager.GetLayoutOptions() -> GetDefaultBoldFlag() );
			}
		}
		nKeyIdx += nColMax;		
	}

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListManager::GroupKey( __int64 nListNo, int nPageNo, int nSourceKeyPos, int nDestKeyPos )
{
	int nListIdx;
	if ( FindList( nListNo, nListIdx ) == FALSE )
		return FALSE;

	CSPOSKeyboardListRecord ListRecord;
	GetListRecord( nListIdx, ListRecord );

	int nKeysPerPage = ListRecord.GetKeysPerPage();

	int nStartKeyIdx, nEndKeyIdx;
	GetPageKeyRange( nListNo, nPageNo, nStartKeyIdx, nEndKeyIdx );

	if ( nEndKeyIdx != nStartKeyIdx + nKeysPerPage )
		return FALSE;

	if ( nSourceKeyPos < 1 || nSourceKeyPos > nKeysPerPage )
		return FALSE;

	if ( nDestKeyPos < 1 || nDestKeyPos > nKeysPerPage )
		return FALSE;

	CSPOSKeyboardKeyRecord SourceKeyRecord, DestKeyRecord;
	GetKeyRecord( nStartKeyIdx + nSourceKeyPos - 1, SourceKeyRecord );
	GetKeyRecord( nStartKeyIdx + nDestKeyPos - 1, DestKeyRecord );

	int nColCount = ListRecord.GetColumnCount();
	int nRowCount = ListRecord.GetRowCount();

	int nSourceColStart = ( nSourceKeyPos - 1 ) % nColCount;
	int nSourceColEnd = nSourceColStart + SourceKeyRecord.GetColSize() - 1;
	int nSourceRowStart = ( nSourceKeyPos - 1 ) / nColCount;
	int nSourceRowEnd = nSourceRowStart + SourceKeyRecord.GetRowSize() - 1;
	if ( nSourceRowEnd < nSourceRowStart ) nSourceRowEnd = nSourceRowStart;
	if ( nSourceColEnd < nSourceColStart ) nSourceColEnd = nSourceColStart;

	int nDestColStart = ( nDestKeyPos - 1 ) % nColCount;
	int nDestColEnd = nDestColStart + DestKeyRecord.GetColSize() - 1;
	int nDestRowStart = ( nDestKeyPos - 1 ) / nColCount;
	int nDestRowEnd = nDestRowStart + DestKeyRecord.GetRowSize() - 1;
	if ( nDestRowEnd < nDestRowStart ) nDestRowEnd = nDestRowStart;
	if ( nDestColEnd < nDestColStart ) nDestColEnd = nDestColStart;

	int nNewColStart = __min( nSourceColStart, nDestColStart );
	int nNewColEnd = __max( nSourceColEnd, nDestColEnd );
	int nNewRowStart = __min( nSourceRowStart, nDestRowStart );
	int nNewRowEnd = __max( nSourceRowEnd, nDestRowEnd );

	if ( nNewColStart < 0 || nNewColStart >= nColCount )
		return FALSE;

	if ( nNewRowStart < 0 || nNewRowStart >= nRowCount )
		return FALSE;

	if ( nNewColEnd < 0 || nNewColEnd >= nColCount )
		return FALSE;

	if ( nNewRowEnd < 0 || nNewRowEnd >= nRowCount )
		return FALSE;

	if ( nNewColEnd < nNewColStart )
		return FALSE;

	if ( nNewRowEnd < nNewRowStart )
		return FALSE;

	int nKeyNo = nNewColStart + ( nNewRowStart * nColCount ) + 1;
	int nKeyIdx = nStartKeyIdx + nKeyNo - 1;

	SourceKeyRecord.SetKeyNo( nKeyNo );
	SourceKeyRecord.SetColSize( nNewColEnd + 1 - nNewColStart );
	SourceKeyRecord.SetRowSize( nNewRowEnd + 1 - nNewRowStart );
	SetKeyRecord( nKeyIdx, SourceKeyRecord );

	for ( int nRowOff = 0; nRowOff <= nNewRowEnd - nNewRowStart; nRowOff++ )
	{
		for ( int nColOff = 0; nColOff <= nNewColEnd - nNewColStart; nColOff++ )
		{
			if ( nColOff == 0 && nRowOff == 0 )
				continue;

			CSPOSKeyboardKeyRecord KeyRecord;
			GetKeyRecord( nKeyIdx + nColOff, KeyRecord );
			KeyRecord.SetType( KEY_TYPE_EMPTY );
			KeyRecord.SetValue( 0 );
			KeyRecord.SetColSize( 0 );
			KeyRecord.SetRowSize( 0 );
			SetKeyRecord( nKeyIdx + nColOff, KeyRecord );
		}
		nKeyIdx += nColCount;
	}

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardListManager::UpdateSettingsGlobally( CSPOSKeyboardGlobalChangeRequest& ChangeRequest )
{		
	__int64 nCurrentListNo = -1;
	bool bCanChangeList = FALSE;

	CWaitCursor wait;
	for ( int nKeyIdx = 0; nKeyIdx < GetKeyCount(); nKeyIdx++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord;
		GetKeyRecord( nKeyIdx, KeyRecord );

		__int64 nListNo = KeyRecord.GetListNo();

		if ( nListNo != nCurrentListNo )
		{
			int nListIdx;
			if (FindList(nListNo, nListIdx) == FALSE)
			{
				bCanChangeList = FALSE;
			}
			else
			{
				CSPOSKeyboardListRecord ListRecord;
				GetListRecord( nListIdx, ListRecord );

				switch( ChangeRequest.m_nChangesMode )
				{
				//VISIBLE LIST ONLY
				case 0:
					bCanChangeList = ( KeyRecord.GetListNo() == ChangeRequest.m_nVisibleListNo );
					break;

				case 1:		//TICKED LIST TYPES (ALL)
				case 2:		//TICKED LIST TYPES (SELECTED)
					switch( ListRecord.GetListType() )
					{
					case LISTTYPE_ITEM_STANDARD:
						bCanChangeList = ChangeRequest.m_bChangeItemStandard;
						break;

					case LISTTYPE_ITEM_LARGE:
						bCanChangeList = ChangeRequest.m_bChangeItemLarge;
						break;

					case LISTTYPE_FUNCTION_STANDARD:
						bCanChangeList = ChangeRequest.m_bChangeFunctionStandard;
						break;

					case LISTTYPE_FUNCTION_LARGE:
						bCanChangeList = ChangeRequest.m_bChangeFunctionLarge;
						break;

					case LISTTYPE_FUNCTION_BOTTOM:
						bCanChangeList = ChangeRequest.m_bChangeBottomBar;
						break;
					}
					break;
				}

				switch( ChangeRequest.m_nChangesMode )
				{
				case 2:
					bCanChangeList &= ListRecord.GetAllowChangeFlag();
					break;
				}
			}

			nCurrentListNo = nListNo;
		}

		if (FALSE == bCanChangeList)
		{
			continue;
		}

		bool bChangeKey = FALSE;
		switch ( KeyRecord.GetType() )
		{
		case KEY_TYPE_EMPTY:		bChangeKey = ChangeRequest.m_bEmpty;		break;
		case KEY_TYPE_FUNCTION:		bChangeKey = ChangeRequest.m_bFunction;		break;
		case KEY_TYPE_PAYMENT:		bChangeKey = ChangeRequest.m_bPayment;		break;
		case KEY_TYPE_MODIFIER:		bChangeKey = ChangeRequest.m_bModifier;		break;
		case KEY_TYPE_CONTROL:		bChangeKey = ChangeRequest.m_bControl;		break;
		case KEY_TYPE_LIST:			bChangeKey = ChangeRequest.m_bList;			break;
		case KEY_TYPE_SVRPRESET:	bChangeKey = ChangeRequest.m_bServer;		break;
		case KEY_TYPE_MACRO:		bChangeKey = ChangeRequest.m_bMacro;		break;
		case KEY_TYPE_PLUNUMBER:	bChangeKey = ChangeRequest.m_bPlu;			break;
		}

		if (bChangeKey == FALSE)
		{
			continue;
		}

		CSPOSKeyboardDefaultTextRecord TextRecord;
		if (	ChangeRequest.m_nColourMode == 1 || ChangeRequest.m_nFontSize == 998 || 
				ChangeRequest.m_bKeyText == TRUE || ChangeRequest.m_nHorizontal == 998 ||
				ChangeRequest.m_nVertical == 998 || ChangeRequest.m_nBold == 998 )
		{
			CSPOSKeyboardKeyHelpers KeyHelpers;
			KeyHelpers.GetDefaultTextRecord( KeyRecord, TextRecord );
		}

		switch ( ChangeRequest.m_nFontSize )
		{
		//NO CHANGE	
		case 999:
			break;

		//KEY DEFAULT
		case 998:
			KeyRecord.SetFontSize( TextRecord.GetFontSize() );
			break;

		default:
			KeyRecord.SetFontSize( ChangeRequest.m_nFontSize );
			break;
		}

		switch ( ChangeRequest.m_nHorizontal )
		{
		//NO CHANGE	
		case 999:
			break;

		//KEY DEFAULT
		case 998:
			KeyRecord.SetAlignmentH( TextRecord.GetHAlign() );
			break;

		default:
			KeyRecord.SetAlignmentH( ChangeRequest.m_nHorizontal );
			break;
		}

		switch ( ChangeRequest.m_nVertical )
		{
		//NO CHANGE	
		case 999:
			break;

		//KEY DEFAULT
		case 998:
			KeyRecord.SetAlignmentV( TextRecord.GetVAlign() );
			break;

		default:
			KeyRecord.SetAlignmentV( ChangeRequest.m_nVertical );
			break;
		}

		switch ( ChangeRequest.m_nBold )
		{
		//NO CHANGE	
		case 999:
			break;

		//KEY DEFAULT
		case 998:
			KeyRecord.SetBoldFlag( TextRecord.GetBoldFlag() );
			break;

		default:
			KeyRecord.SetBoldFlag( ChangeRequest.m_nBold == 1 );
			break;
		}

		switch( ChangeRequest.m_nHide )
		{
		//NO CHANGE
		case 999:
			break;

		default:
			KeyRecord.SetHideTextFlag( ChangeRequest.m_nHide == 1 );
			break;
		}

		switch( ChangeRequest.m_nDefault )
		{
		//NO CHANGE
		case 999:
			break;

		default:
			KeyRecord.SetDefaultTextAlwaysFlag( ChangeRequest.m_nDefault == 1 );
			break;
		}

		switch ( ChangeRequest.m_nColourMode )
		{
		//NO CHANGE
		case 0:
			break;

		//KEY DEFAULT
		case 1:
			KeyRecord.SetForeground( TextRecord.GetForeground() );
			KeyRecord.SetBackground( TextRecord.GetBackground() );
			break;

		//SPECIFIED COLOURS
		case 2:
			KeyRecord.SetForeground( ChangeRequest.m_nForeground );
			KeyRecord.SetBackground( ChangeRequest.m_nBackground );
			break;
		}

		if ( ChangeRequest.m_bKeyText == TRUE )
		{
			CString strText;
			TextRecord.GetDefaultText( strText);
			KeyRecord.SetKeyText( strText );
		}

		if (TRUE == ChangeRequest.m_bRemoveImage)
		{
			KeyRecord.SetImageFilename("");
		}

		SetKeyRecord( nKeyIdx, KeyRecord );
	}
}

/**********************************************************************/

bool CSPOSKeyboardListManager::MovePages( __int64 nListNo, CWordArray& NewPages )
{
	int nNewPageCount = NewPages.GetSize();
	if ( nNewPageCount < 1 || nNewPageCount > MAX_LIST_PAGES )
		return FALSE;

	int nListIdx;
	if ( FindList( nListNo, nListIdx ) == FALSE )
		return FALSE;

	CSPOSKeyboardListRecord ListRecord;
	GetListRecord( nListIdx, ListRecord );
	
	int nPageCount = ListRecord.GetPageCount();
	int nKeysPerPage = ListRecord.GetKeysPerPage();

	int nListKeyStartIdx, nListKeyEndIdx;
	GetListKeyRange( nListNo, nListKeyStartIdx, nListKeyEndIdx );
	
	if ( nListKeyEndIdx - nListKeyStartIdx != nPageCount * nKeysPerPage )
		return FALSE;

	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> KeyBuffer;

	for ( int nNewPageNo = 1; nNewPageNo <= nNewPageCount; nNewPageNo++ )
	{
		int nNewPageType = NewPages.GetAt(nNewPageNo - 1);

		if ( nNewPageType >= 1 && nNewPageType <= MAX_LIST_PAGES )
		{
			//COPY EXISTING PAGE
			int nPageKeyStartIdx, nPageKeyEndIdx;
			GetPageKeyRange( nListNo, nNewPageType, nPageKeyStartIdx, nPageKeyEndIdx );

			if ( nPageKeyEndIdx - nPageKeyStartIdx != nKeysPerPage )
				return FALSE;

			int nKeyNo = 1;
			for ( int n = nPageKeyStartIdx; n < nPageKeyEndIdx; n++ )
			{
				CSPOSKeyboardKeyRecord KeyRecord;
				GetKeyRecord( n, KeyRecord );
				KeyRecord.SetListNo( nListNo );
				KeyRecord.SetPageNo( nNewPageNo );
				KeyRecord.SetKeyNo( nKeyNo++ );
				KeyBuffer.Add( KeyRecord );
			}
		}
		else if ( nNewPageType == 0 )
		{
			CSPOSKeyboardKeyRecord KeyRecord;
			KeyRecord.SetBoldFlag( SPOSKeyboardManager.GetLayoutOptions() -> GetDefaultBoldFlag() );
			for ( int nKeyNo = 1; nKeyNo <= nKeysPerPage; nKeyNo++ )
			{
				KeyRecord.SetListNo( nListNo );
				KeyRecord.SetPageNo( nNewPageNo );
				KeyRecord.SetKeyNo( nKeyNo );
				KeyBuffer.Add( KeyRecord );
			}
		}
		else
			return FALSE;
	}

	for ( int n = nListKeyEndIdx - 1; n >= nListKeyStartIdx; n-- )
		DeleteKey( n );

	int nNewKeyIdx = nListKeyStartIdx;
	for ( int n = 0; n < KeyBuffer.GetSize(); n++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord = KeyBuffer.GetAt( n );
		m_KeyArray.InsertAt( nNewKeyIdx++, KeyRecord );
	}
	
	ListRecord.SetPageCount( nNewPageCount );
	SetListRecord( nListIdx, ListRecord );

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardListManager::RememberKeysForList( __int64 nListNo )
{
	m_StoredPageKeyArray.RemoveAll();
	m_nStoredPageListNo = nListNo;
	m_nStoredPagePageNo = 0;

	int nKeyIdxStart, nKeyIdxEnd;
	FindKey( nListNo,		1, 0, nKeyIdxStart );
	FindKey( nListNo + 1,	1, 0, nKeyIdxEnd );

	for ( int nKeyIdx = nKeyIdxStart; nKeyIdx < nKeyIdxEnd; nKeyIdx++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord;
		GetKeyRecord( nKeyIdx, KeyRecord );
		m_StoredPageKeyArray.Add( KeyRecord );
	}
}

/**********************************************************************/

void CSPOSKeyboardListManager::RestoreKeysForList( __int64 nListNo )
{
	if ( nListNo == m_nStoredPageListNo )
	{
		int nKeyIdxStart, nKeyIdxEnd;
		FindKey( nListNo,		1, 0, nKeyIdxStart );
		FindKey( nListNo+ 1,	1, 0, nKeyIdxEnd );

		int nStoredKeyCount = m_StoredPageKeyArray.GetSize();
		if ( nKeyIdxEnd - nKeyIdxStart == nStoredKeyCount )
		{
			m_KeyArray.RemoveAt( nKeyIdxStart, nStoredKeyCount );

			for ( int n = 0; n < nStoredKeyCount; n++ )
			{
				CSPOSKeyboardKeyRecord KeyRecord = m_StoredPageKeyArray.GetAt( n );
				m_KeyArray.InsertAt( nKeyIdxStart + n, KeyRecord );
			}
		}
	}
}

/**********************************************************************/
