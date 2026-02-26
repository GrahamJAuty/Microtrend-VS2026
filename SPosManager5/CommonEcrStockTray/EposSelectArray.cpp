/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "EposSelectArray.h"
/**********************************************************************/

CEposSelectArrayLocSort::CEposSelectArrayLocSort()
{
	Reset();
}

/**********************************************************************/

void CEposSelectArrayLocSort::Reset()
{
	m_nSortIdx = 0;
	m_nLocIdx = 0;
	m_nTermIdx = 0;
	m_nSelArrayIdx = 0;
}

/**********************************************************************/

void CEposSelectArrayLocSort::Add (CEposSelectArrayLocSort& source )
{
}

/**********************************************************************/

int CEposSelectArrayLocSort::Compare( CEposSelectArrayLocSort& source, int nHint )
{
	if ( m_nSortIdx != source.m_nSortIdx )
		return ( ( m_nSortIdx > source.m_nSortIdx ) ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( m_nTermIdx != source.m_nTermIdx )
		return ( ( m_nTermIdx > source.m_nTermIdx ) ? 1 : -1 );

	if ( m_nSelArrayIdx != source.m_nSelArrayIdx )
		return ( ( m_nSelArrayIdx > source.m_nSelArrayIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposSelectArrayListItem::CEposSelectArrayListItem()
{
	Reset();
}

/**********************************************************************/

void CEposSelectArrayListItem::Reset()
{
	m_nLineType = EPOSSELECT_TERMINAL;
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_nTermIdx = 0;
	m_nLineCount = 0;
	m_dateStart.SetStatus( COleDateTime::null );
	m_dateEnd.SetStatus( COleDateTime::null );
}

/**********************************************************************/

CEposSelectArray::CEposSelectArray()
{
}

/**********************************************************************/

void CEposSelectArray::RemoveAll()
{
	m_Feeder.RemoveAll();
	m_arrayListItems.RemoveAll();
}

/**********************************************************************/

void CEposSelectArray::MakeList( bool bAllowSort, bool bOneTerminalPerLocation )
{
	m_arrayListItems.RemoveAll();

	if ( ( bAllowSort == FALSE ) || ( EcrmanOptions.GetReportsLocationSortType() == 0 ) )
	{
		for ( int nLocIdx = 0; nLocIdx < m_Feeder.GetLocCount(); nLocIdx++ )
			MakeListLocation( nLocIdx, bOneTerminalPerLocation );
	}
	else
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt( n );
			MakeListLocation( nLocIdx, bOneTerminalPerLocation );
		}
	}

#ifdef STOCKMAN_SYSTEM
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		if ( m_Feeder.GetDbManualFlag( nDbIdx ) == TRUE  )
		{
			CEposSelectArrayListItem ListItem;
			ListItem.m_nLineType = EPOSSELECT_DB_MANUAL;
			ListItem.m_nDbIdx = nDbIdx;
			ListItem.m_nLocIdx = 0;
			ListItem.m_nTermIdx = 0;
			ListItem.m_nLineCount = 0;
			m_arrayListItems.Add( ListItem );
		}
	}

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		if ( m_Feeder.GetDbImportFlag( nDbIdx ) == TRUE  )
		{
			CEposSelectArrayListItem ListItem;
			ListItem.m_nLineType = EPOSSELECT_DB_IMPORT;
			ListItem.m_nDbIdx = nDbIdx;
			ListItem.m_nLocIdx = 0;
			ListItem.m_nTermIdx = 0;
			ListItem.m_nLineCount = 0;
			m_arrayListItems.Add( ListItem );
		}
	}	
#endif
}

/**********************************************************************/

void CEposSelectArray::MakeListLocation( int nLocIdx, bool bOneTerminalPerLocation )
{	
	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo( nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		nDbIdx = 0;

	CArray<int,int> arrayTermIdx;
	m_Feeder.GetTermIdxArray( nLocIdx, arrayTermIdx );

	for ( int nPos = 0; nPos < arrayTermIdx.GetSize(); nPos++ )
	{
		CEposSelectArrayListItem ListItem;
		ListItem.m_nLineType = EPOSSELECT_TERMINAL;
		ListItem.m_nDbIdx = nDbIdx;
		ListItem.m_nLocIdx = nLocIdx;
		ListItem.m_nTermIdx = arrayTermIdx.GetAt ( nPos );
		ListItem.m_nLineCount = 0;
		m_arrayListItems.Add( ListItem );

		if ( TRUE == bOneTerminalPerLocation )
			break;
	}
}

/**********************************************************************/

void CEposSelectArray::GetListItem( int nIndex, CEposSelectArrayListItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayListItems.GetSize() ) )
		item = m_arrayListItems.GetAt( nIndex );
	else
		item.Reset();
}

/**********************************************************************/

void CEposSelectArray::SetListItem( int nIndex, CEposSelectArrayListItem& item )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayListItems.GetSize() ) )
		m_arrayListItems.SetAt( nIndex, item );
}

/**********************************************************************/

void CEposSelectArray::ResetDateInfo()
{
	for( int nIndex = 0; nIndex < m_arrayListItems.GetSize(); nIndex++)
	{ 
		CEposSelectArrayListItem ListItem;
		GetListItem(nIndex, ListItem);
		ListItem.m_dateStart.SetStatus(COleDateTime::null);
		ListItem.m_dateEnd.SetStatus(COleDateTime::null);
		SetListItem(nIndex, ListItem);
	}
}

/**********************************************************************/

void CEposSelectArray::ProcessStartDate( int nIndex, COleDateTime date )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayListItems.GetSize() ) )
	{
		CEposSelectArrayListItem ListItem;
		GetListItem( nIndex, ListItem );

		if ( ( ListItem.m_dateStart.GetStatus() == COleDateTime::null ) || ( ListItem.m_dateStart > date ) )
		{
			ListItem.m_dateStart = date;
			SetListItem( nIndex, ListItem );
		}
	}
}

/**********************************************************************/

void CEposSelectArray::ProcessEndDate( int nIndex, COleDateTime date )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayListItems.GetSize() ) )
	{
		CEposSelectArrayListItem ListItem;
		GetListItem( nIndex, ListItem );

		if ( ( ListItem.m_dateEnd.GetStatus() == COleDateTime::null ) || ( ListItem.m_dateEnd < date ) )
		{
			ListItem.m_dateEnd = date;
			SetListItem( nIndex, ListItem );
		}
	}
}

/**********************************************************************/

void CEposSelectArray::ProcessLineCount( int nIndex, int nCount )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayListItems.GetSize() ) )
	{
		CEposSelectArrayListItem ListItem;
		GetListItem( nIndex, ListItem );
		ListItem.m_nLineCount += nCount;
		SetListItem( nIndex, ListItem );
	}
}

/**********************************************************************/

const char* CEposSelectArray::GetDateInfo( CEposSelectArrayListItem& ListItem, bool& bHideNoData )
{
	m_strDateInfo = "(NO DATA)";
	bHideNoData = DealerFlags.GetHideNoDataFlag();

	if ( ( ListItem.m_dateStart.GetStatus() == COleDateTime::valid ) && ( ListItem.m_dateEnd.GetStatus() == COleDateTime::valid ) )
	{
		m_strDateInfo.Format ( "(%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d)",
			ListItem.m_dateStart.GetDay(),
			ListItem.m_dateStart.GetMonth(),
			ListItem.m_dateStart.GetYear(),
			ListItem.m_dateEnd.GetDay(),
			ListItem.m_dateEnd.GetMonth(),
			ListItem.m_dateEnd.GetYear() );

		bHideNoData = FALSE;
	}

	return m_strDateInfo;
}

/**********************************************************************/

void CEposSelectArray::MergeDateInfo(CEposSelectArrayListItem& ListItemSource, CEposSelectArrayListItem& ListItemDest, int& nTerminals, int& nWithData)
{
	nTerminals++;

	if ((ListItemSource.m_dateStart.GetStatus() == COleDateTime::valid) && (ListItemSource.m_dateEnd.GetStatus() == COleDateTime::valid))
	{
		nWithData++;

		{
			bool bUpdateStart = TRUE;
			if (ListItemDest.m_dateStart.GetStatus() == COleDateTime::valid)
			{
				bUpdateStart = (ListItemDest.m_dateStart > ListItemSource.m_dateStart);
			}

			if (TRUE == bUpdateStart)
			{
				ListItemDest.m_dateStart = ListItemSource.m_dateStart;
			}
		}

		{
			bool bUpdateEnd = TRUE;
			if (ListItemDest.m_dateEnd.GetStatus() == COleDateTime::valid)
			{
				bUpdateEnd = (ListItemDest.m_dateEnd > ListItemSource.m_dateEnd);
			}

			if (TRUE == bUpdateEnd)
			{
				ListItemDest.m_dateEnd = ListItemSource.m_dateEnd;
			}
		}
	}
}

/**********************************************************************/

void CEposSelectArray::BuildLocSortArray()
{
	m_arrayLocSort.RemoveAll();
	for ( int nIndex = 0; nIndex <GetListSize(); nIndex++ )
	{
		CEposSelectArrayListItem ListItem;
		GetListItem( nIndex, ListItem );

		CEposSelectArrayLocSort LocSort;
		LocSort.m_nLocIdx = ListItem.m_nLocIdx;
		LocSort.m_nSortIdx = dbLocation.GetSortIdxFromLocIdx( ListItem.m_nLocIdx );
		LocSort.m_nTermIdx = ListItem.m_nTermIdx;
		LocSort.m_nSelArrayIdx = nIndex;
		m_arrayLocSort.Consolidate( LocSort );
	}
}

/**********************************************************************/

int CEposSelectArray::GetLocSortIdx( int nSortIdx )
{
	int nResult = 0;
	if ( ( nSortIdx >= 0 ) && ( nSortIdx < m_arrayLocSort.GetSize() ) )
	{
		CEposSelectArrayLocSort LocSort;
		m_arrayLocSort.GetAt( nSortIdx, LocSort );
		nResult = LocSort.m_nSelArrayIdx;
	}
	return nResult;
}

/**********************************************************************/
