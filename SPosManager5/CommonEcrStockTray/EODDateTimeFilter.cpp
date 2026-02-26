/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "EODDateTimeFilter.h"
/**********************************************************************/

CEODDateTimeFilterArray::CEODDateTimeFilterArray()
{
}

/**********************************************************************/

void CEODDateTimeFilterArray::PrepareForUse()
{
	ClearAfterUse();
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		CEODDateTimeFilter* pFilter = new CEODDateTimeFilter( nLocIdx );
		m_arrayLocs.Add( pFilter );
	}
}

/**********************************************************************/

CEODDateTimeFilterArray::~CEODDateTimeFilterArray()
{
	ClearAfterUse();
}

/**********************************************************************/

void CEODDateTimeFilterArray::ClearAfterUse()
{
	for ( int nLocIdx = 0; nLocIdx < m_arrayLocs.GetSize(); nLocIdx++ )
	{
		CEODDateTimeFilter* pFilter = m_arrayLocs.GetAt( nLocIdx );
		
		if ( pFilter != NULL )
		{
			pFilter -> Clear();
			delete( pFilter );
		}
	}

	m_arrayLocs.RemoveAll();
}

/**********************************************************************/

bool CEODDateTimeFilterArray::CheckFilter( int nLocIdx )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < m_arrayLocs.GetSize() ) )
		return ( m_arrayLocs[nLocIdx] != NULL );
	else
		return FALSE;
}

/**********************************************************************/

void CEODDateTimeFilterArray::CheckFile( int nLocIdx )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		m_arrayLocs[nLocIdx] -> CheckFile();
}

/**********************************************************************/

bool CEODDateTimeFilterArray::GetFilterTimes( int nLocIdx, CString& strDateFrom, CString& strDateTo )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		return m_arrayLocs[nLocIdx] -> GetFilterTimes( strDateFrom, strDateTo );
	else
		return FALSE;
}

/**********************************************************************/

bool CEODDateTimeFilterArray::CheckTime( int nLocIdx, CString& strDateTime )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		return m_arrayLocs[nLocIdx] -> CheckTime( strDateTime );
	else
		return FALSE;
}

/**********************************************************************/

bool CEODDateTimeFilterArray::CheckTime( int nLocIdx, COleDateTime& dateTime )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		return m_arrayLocs[nLocIdx] -> CheckTime( dateTime );
	else
		return FALSE;
}

/**********************************************************************/

bool CEODDateTimeFilterArray::GetBusinessDay( int nLocIdx, CString& strDateTime, CSortedDateTimeItem& itemDay )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		return m_arrayLocs[nLocIdx] -> GetBusinessDay( strDateTime, itemDay );
	else
		return FALSE;
}

/**********************************************************************/

bool CEODDateTimeFilterArray::GetBusinessDay( int nLocIdx, COleDateTime& oleDateTime, CSortedDateTimeItem& itemDay  )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		return m_arrayLocs[nLocIdx] -> GetBusinessDay( oleDateTime, itemDay );
	else
		return FALSE;
}

/**********************************************************************/

bool CEODDateTimeFilterArray::GetLastResult( int nLocIdx )
{
	if ( CheckFilter( nLocIdx ) == TRUE )
		return m_arrayLocs[nLocIdx] -> GetLastResult();
	else
		return FALSE;
}

/**********************************************************************/

CSortedDateTimeItem CEODDateTimeFilterArray::GetCheckDateFrom( int nLocIdx )
{
	CSortedDateTimeItem item;

	if ( CheckFilter( nLocIdx ) == TRUE )
		item = m_arrayLocs[nLocIdx] -> GetCheckDateFrom();
	else
		item.m_strDateTime = "";

	return item;
}

/**********************************************************************/

CSortedDateTimeItem CEODDateTimeFilterArray::GetCheckDateTo( int nLocIdx )
{
	CSortedDateTimeItem item;

	if ( CheckFilter( nLocIdx ) == TRUE )
		item = m_arrayLocs[nLocIdx] -> GetCheckDateTo();
	else
		item.m_strDateTime = "";

	return item;
}

/**********************************************************************/

CSortedDateTimeItem CEODDateTimeFilterArray::GetCheckTimeFrom( int nLocIdx )
{
	CSortedDateTimeItem item;

	if ( CheckFilter( nLocIdx ) == TRUE )
		item = m_arrayLocs[nLocIdx] -> GetCheckTimeFrom();
	else
		item.m_strDateTime = "";

	return item;
}

/**********************************************************************/

CSortedDateTimeItem CEODDateTimeFilterArray::GetCheckTimeTo( int nLocIdx )
{
	CSortedDateTimeItem item;

	if ( CheckFilter( nLocIdx ) == TRUE )
		item = m_arrayLocs[nLocIdx] -> GetCheckTimeTo();
	else
		item.m_strDateTime = "";

	return item;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEODDateTimeFilter::CEODDateTimeFilter( int nLocIdx )
{
	m_nLocIdx = nLocIdx;
	Clear();
}

/**********************************************************************/

void CEODDateTimeFilter::Clear()
{
	m_arrayEODTimes.RemoveAll();
	
	m_nFileSize = 0;
	m_nLineCount = 0;
	m_bLastResult = FALSE;
	m_bLocked = FALSE;
	m_bGotBusinessDay = FALSE;
}

/**********************************************************************/

void CEODDateTimeFilter::CheckFile()
{
	if ( TRUE == m_bLocked )
		return;

	CFileStatus FileStatus;
	CFilenameUpdater FnUp( SysFiles::EODDates, m_nLocIdx );
	CString strFilename = FnUp.GetFilenameToUse();

	if ( CFile::GetStatus( strFilename, FileStatus ) == FALSE )
	{
		m_bLocked = TRUE;
		return;
	}

	if ( FileStatus.m_size == m_nFileSize )
		return;

	CFileStringArray arrayUnsorted;		
	if ( arrayUnsorted.Read( strFilename ) == FALSE )
	{
		m_bLocked = TRUE;
		return;
	}

	for ( int n = m_nLineCount; n < arrayUnsorted.GetSize(); n++ )
	{
		CString strData = arrayUnsorted.GetAt(n);
		
		switch( strData.GetLength() )
		{
		case 15:
		case 24:
		case 35:
			{
				COleDateTime oleDateTime;
				CString strDateTime = strData.Left(8) + strData.Mid(9,6);
				if ( ::GetOleDateTimeFromString( strDateTime, oleDateTime ) == TRUE )
				{
					CSortedDateTimeItem item;
					item.m_strDateTime = strDateTime;
					item.m_oleDateTime = oleDateTime;
					m_arrayEODTimes.Consolidate( item );
				}
			}
			break;
		}
	}

	m_bGotBusinessDay = FALSE;
	m_nFileSize = FileStatus.m_size;
	m_nLineCount = arrayUnsorted.GetSize();
}

/**********************************************************************/

bool CEODDateTimeFilter::GetFilterTimes( CString& strDateFrom, CString& strDateTo )
{
	m_bLastResult = FALSE;
	
	m_CheckDateFrom.Reset();
	m_CheckDateTo.Reset();
	m_CheckTimeFrom.Reset();
	m_CheckTimeTo.Reset();

	CSortedDateTimeItem CheckTimeFrom;
	CSortedDateTimeItem CheckTimeTo;

	int nEODCount = m_arrayEODTimes.GetSize();

	if ( nEODCount < 2 )
		return FALSE;

	CString strStartDateTime;
	if ( FormatEODSearchTime( strDateFrom, strStartDateTime, FALSE ) == FALSE )
		return FALSE;

	CString strEndDateTime;
	if ( FormatEODSearchTime( strDateTo, strEndDateTime, TRUE ) == FALSE )
		return FALSE;

	//THE START TIME FOR THE FILTER IS THE MOST RECENT EOD THAT PRECEDES THE
	//SOD TIME FOR THE START DATE. IF NO SUCH EOD EXISTS, THEN USE THE FIRST
	//KNOWN EOD INSTEAD
	{
		int nPos;
		CSortedDateTimeItem item;
		item.m_strDateTime = strStartDateTime;
		m_arrayEODTimes.Find( item, nPos );

		if ( nPos >= nEODCount )
			return FALSE;

		if ( nPos > 0 )
			nPos--;

		m_arrayEODTimes.GetAt( nPos, CheckTimeFrom );
	}

	//THE END TIME FOR THE FILTER IS THE MOST RECENT EOD THAT PRECEDES THE
	//SOD TIME FOR THE DAY AFTER THE END DATE. THIS CANNOT BE THE FIRST
	//KNOWN EOD, AS IT MUST BE LATER THAN THE START TIME OF THE FILTER
	{
		int nPos;
		CSortedDateTimeItem item;
		item.m_strDateTime = strEndDateTime;
		m_arrayEODTimes.Find( item, nPos );

		if ( nPos <= 1 )
			return FALSE;

		nPos--;

		m_arrayEODTimes.GetAt( nPos, CheckTimeTo );
	}

	//IF THE START TIME IS NOT PRIOR TO THE END TIME THEN WE DO NOT 
	//HAVE A RANGE OF TRANSACTIONS TO SEARCH
	if ( CheckTimeFrom.m_strDateTime >= CheckTimeTo.m_strDateTime )
		return FALSE;

	m_bLastResult = TRUE;

	m_CheckTimeFrom = CheckTimeFrom;
	m_CheckTimeTo = CheckTimeTo;

	m_CheckDateFrom.m_strDateTime = m_CheckTimeFrom.m_strDateTime.Left(8);
	m_CheckDateFrom.m_oleDateTime = COleDateTime(
		m_CheckTimeFrom.m_oleDateTime.GetYear(),
		m_CheckTimeFrom.m_oleDateTime.GetMonth(),
		m_CheckTimeFrom.m_oleDateTime.GetDay(),
		0, 0, 0 );

	m_CheckDateTo.m_strDateTime = m_CheckTimeTo.m_strDateTime.Left(8);
	m_CheckDateTo.m_oleDateTime = COleDateTime(
		m_CheckTimeTo.m_oleDateTime.GetYear(),
		m_CheckTimeTo.m_oleDateTime.GetMonth(),
		m_CheckTimeTo.m_oleDateTime.GetDay(),
		0, 0, 0 );

	return TRUE;
}

/**********************************************************************/

bool CEODDateTimeFilter::CheckTime( CString& strDateTime )
{
	return ( ( strDateTime > m_CheckTimeFrom.m_strDateTime ) && ( strDateTime <= m_CheckTimeTo.m_strDateTime ) );
}

/**********************************************************************/

bool CEODDateTimeFilter::CheckTime( COleDateTime& dateTime )
{
	return ( ( dateTime >= m_CheckTimeFrom.m_oleDateTime ) && ( dateTime <= m_CheckTimeTo.m_oleDateTime ) );
}

/**********************************************************************/

bool CEODDateTimeFilter::GetBusinessDay( CString& strDateTime, CSortedDateTimeItem& itemDay )
{
	bool bResult = FALSE;
	
	COleDateTime oleDateTime;
	if ( ::GetOleDateTimeFromString( strDateTime, oleDateTime ) == TRUE )
	{
		if ( ( oleDateTime >= m_CheckTimeFrom.m_oleDateTime ) && ( oleDateTime <= m_CheckTimeTo.m_oleDateTime ) ) 
		{
			CSortedDateTimeItem itemSearch;
			itemSearch.m_strDateTime = strDateTime;
			itemSearch.m_oleDateTime = oleDateTime;

			bResult = GetBusinessDay( itemSearch, itemDay );

			if ( FALSE == bResult )
			{
				m_bGotBusinessDay = FALSE;
				itemDay.Reset();
			}
		}
	}

	return bResult;
}

/**********************************************************************/

bool CEODDateTimeFilter::GetBusinessDay( COleDateTime& oleDateTime, CSortedDateTimeItem& itemDay )
{
	bool bResult = FALSE;

	CString strDateTime;
	if ( ::GetStringFromOleDateTime( oleDateTime, strDateTime ) == TRUE )
	{
		if ( ( oleDateTime >= m_CheckTimeFrom.m_oleDateTime ) && ( oleDateTime <= m_CheckTimeTo.m_oleDateTime ) ) 
		{
			CSortedDateTimeItem itemSearch;
			itemSearch.m_strDateTime = strDateTime;
			itemSearch.m_oleDateTime = oleDateTime;

			bResult = GetBusinessDay( itemSearch, itemDay );

			if ( FALSE == bResult )
			{
				m_bGotBusinessDay = FALSE;
				itemDay.Reset();
			}
		}
	}

	return bResult;
}

/**********************************************************************/

bool CEODDateTimeFilter::GetBusinessDay( CSortedDateTimeItem& itemSearch, CSortedDateTimeItem& itemDay  )
{
	if ( TRUE == m_bGotBusinessDay )
	{
		if ( ( itemSearch.m_oleDateTime > m_BusinessDayStart.m_oleDateTime ) && ( itemSearch.m_oleDateTime <= m_BusinessDayEnd.m_oleDateTime ) )
		{
			itemDay = m_BusinessDayCurrent;
			return TRUE;
		}
	}

	int nPos;
	m_arrayEODTimes.Find( itemSearch, nPos );

	if ( ( nPos < 1 ) || ( nPos >= m_arrayEODTimes.GetSize() ) )
		return FALSE;

	m_arrayEODTimes.GetAt( nPos - 1, m_BusinessDayStart );
	m_arrayEODTimes.GetAt( nPos, m_BusinessDayEnd );
	m_bGotBusinessDay = TRUE;

	itemDay.m_oleDateTime = COleDateTime(
		m_BusinessDayEnd.m_oleDateTime.GetYear(),
		m_BusinessDayEnd.m_oleDateTime.GetMonth(),
		m_BusinessDayEnd.m_oleDateTime.GetDay(),
		0, 0, 0 );

	bool bPreviousDay = FALSE;

	int nSODHour = EcrmanOptions.GetReportsDayStartHour();
	int nEODHour = m_BusinessDayEnd.m_oleDateTime.GetHour();

	if ( nSODHour > nEODHour  )
		bPreviousDay = TRUE;
	else
	{
		if ( nSODHour == nEODHour )
		{
			int nSODMinute = EcrmanOptions.GetReportsDayStartMinute();
			int nEODMinute = m_BusinessDayEnd.m_oleDateTime.GetMinute();
			bPreviousDay = ( nSODMinute > nEODMinute );
		}
	}

	if ( TRUE == bPreviousDay )
		itemDay.m_oleDateTime -= COleDateTimeSpan( 1, 0, 0, 0 );

	::GetStringFromOleDate( itemDay.m_oleDateTime, itemDay.m_strDateTime );
	m_BusinessDayCurrent = itemDay;
	
	return TRUE;
}

/**********************************************************************/

bool CEODDateTimeFilter::FormatEODSearchTime( CString& strDate, CString& strDateTime, bool bEnd )
{
	strDateTime = "";

	COleDateTime oleDate;
	if ( ::GetOleDateFromString( strDate, oleDate ) == FALSE )
		return FALSE;

	if ( TRUE == bEnd )
		oleDate += COleDateTimeSpan( 1, 0, 0, 0 );

	strDateTime.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d00",
		oleDate.GetYear(),
		oleDate.GetMonth(),
		oleDate.GetDay(),
		EcrmanOptions.GetReportsDayStartHour(),
		EcrmanOptions.GetReportsDayStartMinute() );

	return TRUE;
}

/**********************************************************************/
