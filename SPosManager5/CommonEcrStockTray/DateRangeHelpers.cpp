/**********************************************************************/
#include "DateRangeHelpers.h"
#include "PastAndFutureDateRange.h"
/**********************************************************************/

CDateRangeHelpers::CDateRangeHelpers()
{
}

/**********************************************************************/

int CDateRangeHelpers::GetWeekNo( COleDateTime dateToCheck )
{
	if ( EcrmanOptions.GetReportsWeekNumberingType() == 1 )
	{
		int nYearStartDay, nYearStartMonth;
		EcrmanOptions.GetFYearStart( nYearStartDay, nYearStartMonth );
		return GetWeekNo( dateToCheck, nYearStartMonth, nYearStartDay );
	}
	else
		return GetWeekNo( dateToCheck, 1, 1 );
}

/**********************************************************************/

int CDateRangeHelpers::GetWeekNo( COleDateTime dateToCheck, int nYearStartMonth, int nYearStartDay )
{
	if ( dateToCheck.GetStatus() != COleDateTime::valid )
		return 0;

	int nYear = dateToCheck.GetYear();

	COleDateTime dateWeek1LastYear, dateWeek1ThisYear, dateWeek1NextYear;
	
	GetWeek1StartDate ( nYear - 1,	dateWeek1LastYear, nYearStartMonth, nYearStartDay );
	GetWeek1StartDate ( nYear,		dateWeek1ThisYear, nYearStartMonth, nYearStartDay );
	GetWeek1StartDate ( nYear + 1,	dateWeek1NextYear, nYearStartMonth, nYearStartDay );

	COleDateTimeSpan timespan;

	if ( dateToCheck >= dateWeek1NextYear )
		timespan = dateToCheck - dateWeek1NextYear;
	else if ( dateToCheck >= dateWeek1ThisYear )
		timespan = dateToCheck - dateWeek1ThisYear;
	else
		timespan = dateToCheck - dateWeek1LastYear;
	
	return ( ( timespan.GetDays() / 7 ) + 1 );
}

/**********************************************************************/

void CDateRangeHelpers::GetWeek1StartDate( int nYear, COleDateTime& dateWeek1Start, int nYearStartMonth, int nYearStartDay )
{
	int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
	
	COleDateTime dateYearStart ( nYear, nYearStartMonth, nYearStartDay, 0, 0, 0 );
	int nYearStartWeekDay = dateYearStart.GetDayOfWeek() - 1;

	int nTrailingDays = nWeekStartDay - nYearStartWeekDay;
	if ( nTrailingDays < 0 ) nTrailingDays += 7;

	if ( nTrailingDays == 0 )
		dateWeek1Start = dateYearStart;
	else if ( nTrailingDays < 4 )
		dateWeek1Start = dateYearStart + COleDateTimeSpan( nTrailingDays, 0, 0, 0 );
	else
		dateWeek1Start = dateYearStart - COleDateTimeSpan( ( 7 - nTrailingDays ), 0, 0, 0 );
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
		int nFYearDay;
		int nFYearMonth;
		EcrmanOptions.GetFYearStart ( nFYearDay, nFYearMonth );
		int nWeekStart = EcrmanOptions.GetReportsWeekStartDay() + 1;
		int nTaxQuarter = EcrmanOptions.GetReportsTaxQuarterStart() + 1;

		CSSDateRange DateRange( nWeekStart, nTaxQuarter, nFYearDay, nFYearMonth );
		DateRange.GetRange ( strRangeText, timeFrom, timeTo );	
	}
}

/**********************************************************************/

void CDateRangeHelpers::GetPastAndFutureDateRange( const char* szRangeText, CTime& timeFrom, CTime& timeTo, bool bFromToday )
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
		int nFYearDay;
		int nFYearMonth;
		EcrmanOptions.GetFYearStart ( nFYearDay, nFYearMonth );
		int nWeekStart = EcrmanOptions.GetReportsWeekStartDay() + 1;
		int nTaxQuarter = EcrmanOptions.GetReportsTaxQuarterStart() + 1;

		CPastAndFutureDateRange DateRange( nWeekStart, nTaxQuarter, nFYearDay, nFYearMonth );
		DateRange.GetRange ( strRangeText, timeFrom, timeTo, bFromToday );	
	}
}

/**********************************************************************/

void CDateRangeHelpers::GetCustomerSyncDateRange( const char* szRangeText, CTime& timeFrom, CTime& timeTo )
{
	CString strRangeText = szRangeText;

	if ( strRangeText == szRANGE_ALL )
	{
		CSSDateRange DateRange;
		DateRange.GetRange ( szRANGE_TODAY, timeFrom, timeTo );
		timeFrom = CTime( 2011, 1, 1, 0, 0, 0, -1 );		
	}
	else
	{
		int nFYearDay;
		int nFYearMonth;
		EcrmanOptions.GetFYearStart ( nFYearDay, nFYearMonth );
		int nWeekStart = EcrmanOptions.GetReportsWeekStartDay() + 1;
		int nTaxQuarter = EcrmanOptions.GetReportsTaxQuarterStart() + 1;

		CSSDateRange DateRange( nWeekStart, nTaxQuarter, nFYearDay, nFYearMonth );
		DateRange.GetRange ( strRangeText, timeFrom, timeTo );	
	}
}

/**********************************************************************/

void CDateRangeHelpers::GetOleDateRange( const char* szRangeText, COleDateTime& timeFrom, COleDateTime& timeTo )
{
	CTime legacyFrom, legacyTo;
	GetDateRange( szRangeText, legacyFrom, legacyTo );
	timeFrom = COleDateTime( legacyFrom.GetYear(), legacyFrom.GetMonth(), legacyFrom.GetDay(), 0, 0, 0 );
	timeTo = COleDateTime( legacyTo.GetYear(), legacyTo.GetMonth(), legacyTo.GetDay(), 0, 0, 0 );
}

/**********************************************************************/

void CDateRangeHelpers::GetPastAndFutureOleDateRange( const char* szRangeText, COleDateTime& timeFrom, COleDateTime& timeTo, bool bFromToday )
{
	CTime legacyFrom, legacyTo;
	GetPastAndFutureDateRange( szRangeText, legacyFrom, legacyTo, bFromToday );
	timeFrom = COleDateTime( legacyFrom.GetYear(), legacyFrom.GetMonth(), legacyFrom.GetDay(), 0, 0, 0 );
	timeTo = COleDateTime( legacyTo.GetYear(), legacyTo.GetMonth(), legacyTo.GetDay(), 0, 0, 0 );
}

/**********************************************************************/

void CDateRangeHelpers::GetCustomerSyncOleDateRange( const char* szRangeText, COleDateTime& timeFrom, COleDateTime& timeTo )
{
	CTime legacyFrom, legacyTo;
	GetCustomerSyncDateRange( szRangeText, legacyFrom, legacyTo );
	timeFrom = COleDateTime( legacyFrom.GetYear(), legacyFrom.GetMonth(), legacyFrom.GetDay(), 0, 0, 0 );
	timeTo = COleDateTime( legacyTo.GetYear(), legacyTo.GetMonth(), legacyTo.GetDay(), 0, 0, 0 );
}

/**********************************************************************/

void CDateRangeHelpers::GetLYDateRange( const char* szRangeText, CTime& timeFrom, CTime& timeTo )
{
	//USE LIBRARY FUNCTION TO GET ORIGINAL DATE RANGE
	CString strDateRange = szRangeText;
	GetDateRange( strDateRange, timeFrom, timeTo );

	if ( strDateRange == szRANGE_TODAY )					GetLYDay( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_YESTERDAY )			GetLYDay( timeFrom, timeTo );		
	else if ( strDateRange == szRANGE_THIS_WEEK )			GetLYWeek( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_WEEK )			GetLYWeek( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_THIS_MONTH )			GetLYMonth( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_MONTH )			GetLYMonth( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_THIS_VAT_QUARTER )	GetLYQuarter( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_THIS_GST_QUARTER )	GetLYQuarter( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_VAT_QUARTER )	GetLYQuarter( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_GST_QUARTER )	GetLYQuarter( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_THIS_YEAR )			GetLYYear( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_YEAR )			GetLYYear( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_THIS_FINANCIAL_YEAR )	GetLYYear( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_FINANCIAL_YEAR )	GetLYYear( timeFrom, timeTo );
	else if ( strDateRange == szRANGE_LAST_12_MONTHS )		GetLYYear( timeFrom, timeTo );
}

/**********************************************************************/

void CDateRangeHelpers::GetLYOleDateRange( const char* szRangeText, COleDateTime& timeFrom, COleDateTime& timeTo )
{
	CTime legacyFrom, legacyTo;
	GetLYDateRange( szRangeText, legacyFrom, legacyTo );
	timeFrom = COleDateTime( legacyFrom.GetYear(), legacyFrom.GetMonth(), legacyFrom.GetDay(), 0, 0, 0 );
	timeTo = COleDateTime( legacyTo.GetYear(), legacyTo.GetMonth(), legacyTo.GetDay(), 0, 0, 0 );
}

/**********************************************************************/

void CDateRangeHelpers::GetLYDay( CTime& timeFrom, CTime& timeTo )
{
	//SAME WEEKDAY 52 WEEKS AGO
	AddDaysToTime( timeFrom, -364 );
	timeTo = timeFrom;
}

/**********************************************************************/

void CDateRangeHelpers::GetLYWeek( CTime& timeFrom, CTime& timeTo )
{
	//FULL WEEK FROM 52 WEEKS AGO
	AddDaysToTime( timeFrom, -364 );
	timeTo = timeFrom;
	AddDaysToTime( timeTo, 6 );
}

/**********************************************************************/

void CDateRangeHelpers::GetLYMonth( CTime& timeFrom, CTime& timeTo )
{
	//FULL MONTH FROM PREVIOUS YEAR
	int nYear = timeFrom.GetYear();
	int nMonth = timeFrom.GetMonth();
	int nDay = timeFrom.GetDay();

	timeFrom = CTime ( nYear - 1, nMonth, nDay, 0, 0, 0, -1 );
		
	if ( nMonth < 12 )
		timeTo = CTime ( nYear - 1, nMonth + 1, 1, 0, 0, 0, -1 );
	else
		timeTo = CTime ( nYear, 1, 1, 0, 0, 0, -1 );

	AddDaysToTime( timeTo, -1 );
}

/**********************************************************************/

void CDateRangeHelpers::GetLYQuarter( CTime& timeFrom, CTime& timeTo )
{
	//FULL MONTH FROM PREVIOUS YEAR
	int nYear = timeFrom.GetYear();
	int nMonth = timeFrom.GetMonth();
	int nDay = timeFrom.GetDay();

	timeFrom = CTime ( nYear - 1, nMonth, nDay, 0, 0, 0, -1 );
		
	if ( nMonth < 10 )
		timeTo = CTime ( nYear - 1, nMonth + 3, 1, 0, 0, 0, -1 );
	else
		timeTo = CTime ( nYear, nMonth - 9, 1, 0, 0, 0, -1 );

	AddDaysToTime( timeTo, -1 );
}

/**********************************************************************/

void CDateRangeHelpers::GetLYYear( CTime& timeFrom, CTime& timeTo )
{
	//FULL MONTH FROM PREVIOUS YEAR
	int nYear = timeFrom.GetYear();
	int nMonth = timeFrom.GetMonth();
	int nDay = timeFrom.GetDay();

	timeTo = timeFrom;
	AddDaysToTime ( timeTo, -1 );

	if ( (nMonth == 2) && (nDay == 29) )
	{
		nMonth = 3;
		nDay = 1;
	}

	timeFrom = CTime( nYear - 1, nMonth, nDay, 0, 0, 0, -1 );
}

/**********************************************************************/

void GetEarliestReportDateString ( CString& strType )
{
	int nFYearDay;
	int nFYearMonth;
	EcrmanOptions.GetFYearStart ( nFYearDay, nFYearMonth );
	int nWeekStart = EcrmanOptions.GetReportsWeekStartDay() + 1;
	int nTaxQuarter = EcrmanOptions.GetReportsTaxQuarterStart() + 1;
	CSSDateRange DateRange( nWeekStart, nTaxQuarter, nFYearDay, nFYearMonth );
	
	CTime timeTo, timeFrom1, timeFrom2;
	DateRange.GetRange ( szRANGE_LAST_FINANCIAL_YEAR, timeFrom1, timeTo );
	DateRange.GetRange ( szRANGE_LAST_YEAR, timeFrom2, timeTo );
	strType = ( timeFrom1 < timeFrom2 ) ? szRANGE_LAST_FINANCIAL_YEAR : szRANGE_LAST_YEAR;
}

/**********************************************************************/

void CDateRangeHelpers::AddDaysToTime ( CTime& time, int nDays )
{
	int nOldHour = time.GetHour();
	time += CTimeSpan ( nDays, 0, 0, 0 );
	int nNewHour = time.GetHour();

	if ( ( nNewHour == nOldHour - 1 ) || ( nNewHour == nOldHour + 23 ) )
		time += CTimeSpan ( 0, 1, 0, 0 );

	if ( ( nNewHour == nOldHour + 1 ) || ( nNewHour == nOldHour - 23 ) )
		time -= CTimeSpan ( 0, 1, 0, 0 );
}

/**********************************************************************/

void CDateRangeHelpers::AddDaysToTime ( COleDateTime& time, int nDays )
{
	int nOldHour = time.GetHour();
	time += COleDateTimeSpan ( nDays, 0, 0, 0 );
	int nNewHour = time.GetHour();

	if ( ( nNewHour == nOldHour - 1 ) || ( nNewHour == nOldHour + 23 ) )
		time += COleDateTimeSpan ( 0, 1, 0, 0 );

	if ( ( nNewHour == nOldHour + 1 ) || ( nNewHour == nOldHour - 23 ) )
		time -= COleDateTimeSpan ( 0, 1, 0, 0 );
}

/**********************************************************************/

void CDateRangeHelpers::GetEarliestReportDateString ( CString& strType )
{
	int nFYearDay;
	int nFYearMonth;
	EcrmanOptions.GetFYearStart ( nFYearDay, nFYearMonth );
	int nWeekStart = EcrmanOptions.GetReportsWeekStartDay() + 1;
	int nTaxQuarter = EcrmanOptions.GetReportsTaxQuarterStart() + 1;
	CSSDateRange DateRange( nWeekStart, nTaxQuarter, nFYearDay, nFYearMonth );
	
	CTime timeTo, timeFrom1, timeFrom2;
	DateRange.GetRange ( szRANGE_LAST_FINANCIAL_YEAR, timeFrom1, timeTo );
	DateRange.GetRange ( szRANGE_LAST_YEAR, timeFrom2, timeTo );
	strType = ( timeFrom1 < timeFrom2 ) ? szRANGE_LAST_FINANCIAL_YEAR : szRANGE_LAST_YEAR;
}

/**********************************************************************/

void CDateRangeHelpers::GetFutureDateTo( CString& strDateRange, COleDateTime& oleDateFrom, COleDateTime& oleDateTo, COleDateTime& oleFutureDateTo )
{
	oleFutureDateTo = oleDateTo;

	if ( strDateRange == szRANGE_THIS_WEEK )
	{
		oleFutureDateTo = oleDateFrom;
		AddDaysToTime( oleFutureDateTo, 6 );
	}
	else 
	{
		int nYear = oleDateFrom.GetYear();
		int nMonth = oleDateFrom.GetMonth();
		int nDay = oleDateFrom.GetDay();
		bool bSetDate = FALSE;
		
		if ( strDateRange == szRANGE_THIS_MONTH )
		{
			nMonth++;
			nDay = 1;
			bSetDate = TRUE;
		}
		else if ( ( strDateRange == szRANGE_THIS_VAT_QUARTER ) || ( strDateRange == szRANGE_THIS_GST_QUARTER ) )
		{
			nMonth += 3;
			nDay = 1;
			bSetDate = TRUE;
		}
		else if ( strDateRange == szRANGE_THIS_YEAR )
		{
			nYear++;
			nMonth = 1;
			nDay = 1;
			bSetDate = TRUE;
		}
		else if ( strDateRange == szRANGE_THIS_FINANCIAL_YEAR )
		{
			nYear++;
			bSetDate = TRUE;
		}
		else if ( strDateRange == szRANGE_ALL )
		{
			nYear = oleDateTo.GetYear() + 2;
			nMonth = 1;
			nDay = 1;
			bSetDate = TRUE;
		}

		if ( TRUE == bSetDate )
		{
			if ( nMonth > 12 )
			{
				nYear++;
				nMonth -= 12;
			}

			oleFutureDateTo = COleDateTime( nYear, nMonth, nDay, 0, 0, 0 );
			AddDaysToTime( oleFutureDateTo, -1 );
		}
	}
}

/**********************************************************************/







