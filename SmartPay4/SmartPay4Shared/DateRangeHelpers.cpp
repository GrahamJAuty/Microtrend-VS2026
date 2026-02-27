/**********************************************************************/
#include "ExpiryDateRange.h"
/**********************************************************************/
#include "DateRangeHelpers.h"
/**********************************************************************/

CDateRangeHelpers::CDateRangeHelpers()
{
}

/**********************************************************************/

void CDateRangeHelpers::GetDateRange( const char* szRangeText, CTime& timeFrom, CTime& timeTo )
{
	CString strRangeText = szRangeText;

	if ( strRangeText == szRANGE_ALL )
	{
		CSSDateRange DateRange;
		DateRange.GetRange ( szRANGE_TODAY, timeFrom, timeTo );
		timeFrom = CTime ( 1998, 1, 1, 0, 0, 0, -1 );
	}
	else
	{
		int nFYearDay = 1;
		int nFYearMonth = 1;
		int nWeekStart = System.GetStartOfWeek();
		int nTaxQuarter = 1;

		CSSDateRange DateRange( nWeekStart, nTaxQuarter, nFYearDay, nFYearMonth );
		DateRange.GetRange ( strRangeText, timeFrom, timeTo );	
	}
}

/**********************************************************************/

void CDateRangeHelpers::GetExpiryDateRange( const char* szRangeText, CTime& timeFrom, CTime& timeTo, bool bFromToday )
{
	CString strRangeText = szRangeText;

	if ( strRangeText == szRANGE_ALL )
	{
		CSSDateRange DateRange;
		DateRange.GetRange ( szRANGE_TODAY, timeFrom, timeTo );
		
		if ( TRUE == bFromToday )
			timeTo = CTime( 2050, 1, 1, 0, 0, 0, -1 );
		else
			timeFrom = CTime( 2010, 1, 1, 0, 0, 0, -1 );		
	}
	else
	{
		CExpiryDateRange DateRange( System.GetStartOfWeek() );
		DateRange.GetRange ( strRangeText, timeFrom, timeTo, bFromToday );	
	}
}

/**********************************************************************/

void CDateRangeHelpers::AddDaysToTime(CTime& time, int nDays)
{
	int nOldHour = time.GetHour();
	time += CTimeSpan(nDays, 0, 0, 0);
	int nNewHour = time.GetHour();

	if ((nNewHour == nOldHour - 1) || (nNewHour == nOldHour + 23))
	{
		time += CTimeSpan(0, 1, 0, 0);
	}

	if ((nNewHour == nOldHour + 1) || (nNewHour == nOldHour - 23))
	{
		time -= CTimeSpan(0, 1, 0, 0);
	}
}

/**********************************************************************/

void CDateRangeHelpers::AddDaysToTime(COleDateTime& time, int nDays)
{
	int nOldHour = time.GetHour();
	time += COleDateTimeSpan(nDays, 0, 0, 0);
	int nNewHour = time.GetHour();

	if ((nNewHour == nOldHour - 1) || (nNewHour == nOldHour + 23))
	{
		time += COleDateTimeSpan(0, 1, 0, 0);
	}

	if ((nNewHour == nOldHour + 1) || (nNewHour == nOldHour - 23))
	{
		time -= COleDateTimeSpan(0, 1, 0, 0);
	}
}

/**********************************************************************/
