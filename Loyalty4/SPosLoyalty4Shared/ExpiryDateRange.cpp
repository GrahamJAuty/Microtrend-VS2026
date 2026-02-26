/*****************************************************************************/
#include "ExpiryDateRange.h"
/*****************************************************************************/
const char* szRANGE_TOMORROW			= "Tomorrow";
const char* szRANGE_NEXT_WEEK			= "Next Week";
const char* szRANGE_NEXT_MONTH			= "Next Month";
const char* szRANGE_NEXT_YEAR			= "Next Year";
const char* szRANGE_NEXT_3_MONTHS		= "Next 3 Months";
const char* szRANGE_NEXT_6_MONTHS		= "Next 6 Months";
const char* szRANGE_NEXT_9_MONTHS		= "Next 9 Months";
const char* szRANGE_NEXT_12_MONTHS		= "Next 12 Months";
/*****************************************************************************/

CExpiryDateRange::CExpiryDateRange()
{
	m_nStartOfWeek = 1;			// sunday
}

/*****************************************************************************/

CExpiryDateRange::CExpiryDateRange ( int nStartOfWeek )
{
	m_nStartOfWeek = ( nStartOfWeek >= 1 && nStartOfWeek <= 7 ) ? nStartOfWeek : 1;
}

/*****************************************************************************/

void CExpiryDateRange::GetRange ( const char* szDateRange, CTime& dateStart, CTime& dateEnd, bool bExpand )
{
	GetRange ( szDateRange, bExpand );

	dateStart = m_timeStart;
	dateEnd = m_timeEnd;
}

/*****************************************************************************/

void CExpiryDateRange::GetRange ( const char* szDateRange, CSSDate& dateStart, CSSDate& dateEnd, bool bExpand )
{
	GetRange ( szDateRange, bExpand );

	dateStart.SetDate ( m_timeStart );
	dateEnd.SetDate ( m_timeEnd );
}

/*****************************************************************************/

void CExpiryDateRange::GetRange ( const char* szDateRange, bool bFromToday )
{
	CString strDateRange = szDateRange;

	if ( strDateRange == szRANGE_TODAY )					GetToday();
	if ( strDateRange == szRANGE_TOMORROW )					GetTomorrow();
	else if ( strDateRange == szRANGE_YESTERDAY )			GetYesterday();
	else if ( strDateRange == szRANGE_THIS_WEEK )			GetThisWeek( bFromToday );
	else if ( strDateRange == szRANGE_THIS_MONTH )			GetThisMonth( bFromToday );
	else if ( strDateRange == szRANGE_THIS_YEAR )			GetThisYear( bFromToday );
	else if ( strDateRange == szRANGE_NEXT_WEEK )			GetNextWeek();
	else if ( strDateRange == szRANGE_NEXT_MONTH )			GetNextMonth();
	else if ( strDateRange == szRANGE_NEXT_YEAR )			GetNextYear();
	else if ( strDateRange == szRANGE_NEXT_3_MONTHS )		GetNext3Months();
	else if ( strDateRange == szRANGE_NEXT_6_MONTHS )		GetNext6Months();
	else if ( strDateRange == szRANGE_NEXT_9_MONTHS )		GetNext9Months();
	else if ( strDateRange == szRANGE_NEXT_12_MONTHS )		GetNext12Months();
	else if ( strDateRange == szRANGE_LAST_WEEK )			GetLastWeek();
	else if ( strDateRange == szRANGE_LAST_MONTH )			GetLastMonth();
	else if ( strDateRange == szRANGE_LAST_YEAR )			GetLastYear();
	else if ( strDateRange == szRANGE_LAST_3_MONTHS )		GetLast3Months();
	else if ( strDateRange == szRANGE_LAST_6_MONTHS )		GetLast6Months();
	else if ( strDateRange == szRANGE_LAST_9_MONTHS )		GetLast9Months();
	else if ( strDateRange == szRANGE_LAST_12_MONTHS )		GetLast12Months();
	else													GetToday();
}

/*****************************************************************************/

void CExpiryDateRange::GetTimeToday ( CTime& time )
{
	time = CTime::GetCurrentTime();
	time = CTime ( time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::AddDays ( CTime& time, int nDays )
{
	int nOldHour = time.GetHour();
	time += CTimeSpan ( nDays, 0, 0, 0 );
	int nNewHour = time.GetHour();

	if ( ( nNewHour == nOldHour-1 ) || ( nNewHour == nOldHour+23 ) )
		time += CTimeSpan ( 0, 1, 0, 0 );

	if ( ( nNewHour == nOldHour+1 ) || ( nNewHour == nOldHour-23 ) )
		time -= CTimeSpan ( 0, 1, 0, 0 );
}

/*****************************************************************************/

void CExpiryDateRange::AddMonths ( CTime& time, int nMonths )
{
	int nYear = time.GetYear();
	int nMonth = time.GetMonth() + nMonths;

	while( nMonth <= 0 )
	{
		nMonth += 12;
		nYear -= 1;
	}

	while( nMonth >= 13 )
	{
		nMonth -= 12;
		nYear += 1;
	}

	time = CTime ( nYear, nMonth, 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetToday()
{
	GetTimeToday ( m_timeStart );
	m_timeEnd = m_timeStart;
}

/*****************************************************************************/

void CExpiryDateRange::GetTomorrow()
{
	GetTimeToday ( m_timeStart );
	AddDays ( m_timeStart, 1 );
	m_timeEnd = m_timeStart;
}

/*****************************************************************************/

void CExpiryDateRange::GetYesterday()
{
	GetTimeToday ( m_timeStart );
	AddDays ( m_timeStart, -1 );
	m_timeEnd = m_timeStart;
}

/*****************************************************************************/

void CExpiryDateRange::GetThisWeek( bool bFromToday, bool bFullWeek )
{
	CTime timeToday;
	GetTimeToday ( timeToday );

	int nWeekday = timeToday.GetDayOfWeek();

	if ( nWeekday < m_nStartOfWeek )
		nWeekday += 7;

	m_timeStart = timeToday;
	AddDays ( m_timeStart, m_nStartOfWeek - nWeekday );

	m_timeEnd = m_timeStart;
	AddDays ( m_timeEnd, 6 );

	if ( FALSE == bFullWeek )
	{
		if ( FALSE == bFromToday )
			m_timeEnd = timeToday;
		else
			m_timeStart = timeToday;
	}
}

/*****************************************************************************/

void CExpiryDateRange::GetThisMonth( bool bFromToday )
{
	CTime timeToday;
	GetTimeToday ( timeToday );
	m_timeStart = CTime ( timeToday.GetYear(), timeToday.GetMonth(), 1, 0, 0, 0, -1 );
	
	m_timeEnd = m_timeStart;
	AddMonths( m_timeEnd, 1 );
	AddDays( m_timeEnd, -1 );

	if ( FALSE == bFromToday )
		m_timeEnd = timeToday;
	else
		m_timeStart = timeToday;
}

/*****************************************************************************/

void CExpiryDateRange::GetThisYear( bool bFromToday )
{
	CTime timeToday;
	GetTimeToday ( timeToday );
	m_timeStart = CTime ( timeToday.GetYear(), 1, 1, 0, 0, 0, -1 );

	m_timeEnd = m_timeStart;
	AddMonths( m_timeEnd, 12 );
	AddDays( m_timeEnd, -1 );

	if ( FALSE == bFromToday )
		m_timeEnd = timeToday;
	else
		m_timeStart = timeToday;
}

/*****************************************************************************/

void CExpiryDateRange::GetNextWeek()
{
	GetThisWeek( TRUE, TRUE );
	AddDays( m_timeStart, 7 );
	AddDays( m_timeEnd, 7 );
}

/*****************************************************************************/

void CExpiryDateRange::GetNextMonth()
{
	GetThisMonth( TRUE );
	AddMonths( m_timeStart, 1 );
	AddMonths( m_timeEnd, 2 );
	AddDays( m_timeEnd, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetNextYear()
{
	CTime time;
	GetTimeToday(time);
	m_timeStart = CTime( time.GetYear() + 1, 1, 1, 0, 0, 0 );
	m_timeEnd = CTime( time.GetYear() + 1, 12, 31, 0, 0, 0 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLastWeek()
{
	GetThisWeek( TRUE, TRUE );
	AddDays( m_timeStart, -7 );
	AddDays( m_timeEnd, -7 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLastMonth()
{
	GetTimeToday ( m_timeEnd );
	m_timeEnd = CTime ( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), 1, 0, 0, 0, -1 );
	AddDays ( m_timeEnd, -1 );
	m_timeStart = CTime ( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLastYear()
{
	GetTimeToday ( m_timeEnd );
	m_timeEnd = CTime ( m_timeEnd.GetYear(), 1, 1, 0, 0, 0, -1 );
	AddDays ( m_timeEnd, -1 );
	m_timeStart = CTime ( m_timeEnd.GetYear(), 1, 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLast3Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	AddMonths ( m_timeStart, -3 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLast6Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	AddMonths ( m_timeStart, -6 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLast9Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	AddMonths ( m_timeStart, -9 );
}

/*****************************************************************************/

void CExpiryDateRange::GetLast12Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	AddMonths ( m_timeStart, -12 );
}

/*****************************************************************************/

void CExpiryDateRange::GetNext3Months()
{
	GetTimeToday ( m_timeStart );
	m_timeEnd = m_timeStart;
	AddMonths ( m_timeEnd, 4 );
	AddDays ( m_timeEnd, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetNext6Months()
{
	GetTimeToday ( m_timeStart );
	m_timeEnd = m_timeStart;
	AddMonths ( m_timeEnd, 7 );
	AddDays ( m_timeEnd, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetNext9Months()
{
	GetTimeToday ( m_timeStart );
	m_timeEnd = m_timeStart;
	AddMonths ( m_timeEnd, 10 );
	AddDays ( m_timeEnd, -1 );
}

/*****************************************************************************/

void CExpiryDateRange::GetNext12Months()
{
	GetTimeToday ( m_timeStart );
	m_timeEnd = m_timeStart;
	AddMonths ( m_timeEnd, 13 );
	AddDays ( m_timeEnd, -1 );
}

/*****************************************************************************/