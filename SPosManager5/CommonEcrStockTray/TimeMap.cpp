/**********************************************************************/
 
/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
//#include "globalfunctions.h"
#include "TimeSliceCSVArray.h"
/**********************************************************************/
#include "TimeMap.h"
/**********************************************************************/

CTimeMap::CTimeMap(void)
{
	for ( int n = 0; n <= TimeSlice::Start.Max; n++ )
		m_arraySortOrder.Add ( "0000" );

	for ( int n = 0; n <= TimeSlice::SortOrder.Max; n++ )
		m_arraySharedNames.Add( "" );

	m_arrayUniqueNames.Add( "Other" );
}

/**********************************************************************/

CTimeMap::~CTimeMap(void)
{
}

/**********************************************************************/

void CTimeMap::BuildMap( int nTimeSliceFileNo )
{
	for ( int n = 0; n < m_arraySortOrder.GetSize(); n++ )
		m_arraySortOrder[n] = "0000";

	for ( int n = 1; n < m_arraySharedNames.GetSize(); n++ )
		m_arraySharedNames[n] = "";

	m_arrayUniqueNames.RemoveAll();
	m_arrayUniqueNames.Add( "Other" );

	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenTimeSliceDetail( nTimeSliceFileNo, DB_READONLY, info ) == FALSE )
		return;

	int nNextSortOrder = 1;

	for ( int n = 0; n < DataManagerNonDb.TimeSliceDetail.GetSize(); n++ )
	{
		CTimeSliceCSVRecord timeslice;
		DataManagerNonDb.TimeSliceDetail.GetAt( n, timeslice );

		int nStart = timeslice.GetStart() % ( TimeSlice::Start.Max + 1 );
		int nEnd = timeslice.GetEnd() % ( TimeSlice::End.Max + 1 );

		if ( nStart > nEnd )
			nEnd += ( TimeSlice::Start.Max + 1 );;

		CString strName = timeslice.GetName();
		::TrimSpaces( strName, FALSE );
			
		int nSortOrder;
		int nRequestSortOrder = timeslice.GetSortOrder();
		if ( ( nRequestSortOrder <= 0 ) || ( nRequestSortOrder > TimeSlice::SortOrder.Max ) )
		{
			nSortOrder = nNextSortOrder++;
			m_arrayUniqueNames.Add( strName );
		}
		else
		{
			nSortOrder = nRequestSortOrder + 1000;
			
			if ( strName != "" )
				m_arraySharedNames[nRequestSortOrder] = strName;
		}

		CString strSortOrder;
		strSortOrder.Format( "%4.4d", nSortOrder );

		for ( int x = nStart; x <= nEnd; x++ )
			m_arraySortOrder[x % ( TimeSlice::Start.Max + 1 ) ] = strSortOrder;
	}

	DataManagerNonDb.CloseTimeSliceDetail( info );
}

/**********************************************************************/

const char* CTimeMap::GetSortOrderName( const char* szSortOrder )
{
	m_strSortOrderName = "Other";
	
	int nSortOrder = atoi( szSortOrder );

	if ( ( nSortOrder >= 1 ) && ( nSortOrder <= m_arrayUniqueNames.GetSize() ) )
		m_strSortOrderName = m_arrayUniqueNames.GetAt(nSortOrder);
	else if ( ( nSortOrder >= 1001 ) && ( nSortOrder <= 1000 + TimeSlice::SortOrder.Max ) )
		m_strSortOrderName = m_arraySharedNames.GetAt(nSortOrder - 1000);

	return m_strSortOrderName;
}

/**********************************************************************/

const char* CTimeMap::GetSortOrder( int nHour, int nMinute )
{
	int nPos = ( nHour * 60 ) + nMinute;
	m_strSortOrderOut = m_arraySortOrder[nPos % ( TimeSlice::Start.Max + 1 ) ];
	return m_strSortOrderOut;
}

/**********************************************************************/

