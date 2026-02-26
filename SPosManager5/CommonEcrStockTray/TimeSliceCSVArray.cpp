/**********************************************************************/
//#include "DataManagerNonDb.h"
#include "SessionCSVArray.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/
#include "TimeSliceCSVArray.h"
/**********************************************************************/

CTimeSliceCSVRecord::CTimeSliceCSVRecord()
{
	ClearRecord();	
}

/**********************************************************************/

void CTimeSliceCSVRecord::ClearRecord()
{
	m_strName = "";
	m_nStart = 0;
	m_nEnd = 0;
	m_nSortOrder = 0;
}

/**********************************************************************/

void CTimeSliceCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	SetName ( csv.GetString (0) );
	SetStart ( csv.GetInt(1) ); 
	SetEnd ( csv.GetInt(2) );
	SetSortOrder ( csv.GetInt(3) );
}

/**********************************************************************/

void CTimeSliceCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( m_nStart );
	csv.Add ( m_nEnd );
	csv.Add ( m_nSortOrder );
	csv.Add ( TIMESLICE_VERSION );				
}

/**********************************************************************/

void CTimeSliceCSVRecord::SetStart( int n )
{
	Set( m_nStart, n, TimeSlice::Start );
}

/**********************************************************************/

void CTimeSliceCSVRecord::SetEnd( int n )
{
	Set( m_nEnd, n, TimeSlice::End );
}

/**********************************************************************/

void CTimeSliceCSVRecord::SetSortOrder( int n )
{
	Set( m_nSortOrder, n, TimeSlice::SortOrder );
}

/**********************************************************************/

void CTimeSliceCSVRecord::SetName( const char* sz )
{
	Set( m_strName, sz, TimeSlice::Name );
}

/**********************************************************************/

CTimeSliceCSVArray::CTimeSliceCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CTimeSliceCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strName = csv.GetString ( 0 );
	}

	return m_strName;
}

/**********************************************************************/

int CTimeSliceCSVArray::GetStart ( int nIndex )
{
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		return csv.GetInt ( 1 );
	}

	return 0;
}

/**********************************************************************/

int CTimeSliceCSVArray::GetEnd ( int nIndex )
{
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 3 );
		return csv.GetInt ( 2 );
	}

	return 0;
}

/**********************************************************************/

int CTimeSliceCSVArray::GetSortOrder ( int nIndex )
{
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 4 );
		return csv.GetInt ( 3 );
	}

	return 0;
}

/**********************************************************************/

void CTimeSliceCSVArray::CreateHourlyMap()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < 24; n++ )
	{
		CTimeSliceCSVRecord record;
		record.SetStart( n * 60 );
		record.SetEnd( ( n * 60 ) + 59 );
		record.SetSortOrder( 0 );

		CString strName;
		strName.Format( "%2.2d:00 - %2.2d:59",
			n, n );

		record.SetName( strName );
		Add( record );
	}
}

/**********************************************************************/

void CTimeSliceCSVArray::CreateHalfHourlyMap()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < 48; n++ )
	{
		CTimeSliceCSVRecord record;
		record.SetStart( n * 30 );
		record.SetEnd( ( n * 30 ) + 29 );
		record.SetSortOrder( 0 );

		CString strName;
		strName.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			n / 2,
			( n % 2 ) * 30,
			n / 2,
			( ( n % 2 ) * 30 ) + 29 );

		record.SetName( strName );
		Add( record );
	}
}

/**********************************************************************/

void CTimeSliceCSVArray::CreateQuarterHourlyMap()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < 96; n++ )
	{
		CTimeSliceCSVRecord record;
		record.SetStart( n * 15 );
		record.SetEnd( ( n * 15 ) + 14 );
		record.SetSortOrder( 0 );

		CString strName;
		strName.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			n / 4,
			( n % 4 ) * 15,
			n / 4,
			( ( n % 4 ) * 15 ) + 14 );

		record.SetName( strName );
		Add( record );
	}
}

/**********************************************************************/

void CTimeSliceCSVArray::CreateReportSessionMap()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
	{
		CSessionCSVRecord session;
		DataManagerNonDb.ReportSession.GetAt( n, session );

		if ( session.GetCombinedSessionFlag() == FALSE )
		{
			CTimeSliceCSVRecord timeslice;
			timeslice.SetStart( ( session.GetSessionStartHour() * 60 ) + session.GetSessionStartMinute() );
			timeslice.SetEnd( ( session.GetSessionEndHour() * 60 ) + session.GetSessionEndMinute() );
			timeslice.SetName( session.GetName() );
			timeslice.SetSortOrder(0);

			Add( timeslice );
		}
	}
}

/**********************************************************************/

void CTimeSliceCSVArray::SortTimeSlices()
{
	for ( int n = 1; n < GetSize(); n++ )
	{
		int nStart = GetStart(n);

		for ( int x = 0; x < n; x++ )
		{
			if ( GetStart(x) > nStart )
			{
				CTimeSliceCSVRecord timeslice;
				GetAt( n, timeslice );
				RemoveAt(n);
				InsertAt( x, timeslice );
				break;
			}
		}
	}
}

/**********************************************************************/
