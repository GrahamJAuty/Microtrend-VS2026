/*****************************************************************************/
 
/*****************************************************************************/
#include "DateRangeHelpers.h"
//#include "EcrmanOptionsIni.h"
/*****************************************************************************/
#include "PosTrayDateRange.h"
/*****************************************************************************/

CPosTrayDateRange::CPosTrayDateRange()
{
	int nFinancialYearDay;
	int nFinancialYearMonth;
	EcrmanOptions.GetFYearStart ( nFinancialYearDay, nFinancialYearMonth );
	int nStartOfWeek = EcrmanOptions.GetReportsWeekStartDay() + 1;
	int nTaxQuarterMonth = EcrmanOptions.GetReportsTaxQuarterStart() + 1;
	
	m_nStartOfWeek = ( nStartOfWeek >= 1 && nStartOfWeek <= 7 ) ? nStartOfWeek : 1;
	m_nTaxQuarterMonth = ( nTaxQuarterMonth >= 1 && nTaxQuarterMonth <= 12 ) ? nTaxQuarterMonth : 1;
	m_nFinancialYearDay = ( nFinancialYearDay >= 1 && nFinancialYearDay <= 31 ) ? nFinancialYearDay : 1;
	m_nFinancialYearMonth = ( nFinancialYearMonth >= 1 && nFinancialYearMonth <= 12 ) ? nFinancialYearMonth : 1;
	
	m_dateToday = COleDateTime::GetCurrentTime();
}

/*****************************************************************************/

void CPosTrayDateRange::SetDateToday( COleDateTime& date )
{
	m_dateToday = date;
}

/*****************************************************************************/

void CPosTrayDateRange::SetDateToday( COleDateTime& date, int nSODHour, int nSODMinute )
{
	m_dateToday = date;

	COleDateTime timeSOD = COleDateTime( 2018, 1, 1, nSODHour, nSODMinute, 0 );
	COleDateTime timeTSK = COleDateTime( 2018, 1, 1, date.GetHour(), date.GetMinute(), 0 );

	if ( timeSOD > timeTSK )
		AddDays( m_dateToday, -1 );		
}

/*****************************************************************************/

void CPosTrayDateRange::GetRange ( const char* szDateRange, COleDateTime& dateStart, COleDateTime& dateEnd, COleDateTime& dateFutureEnd )
{
	CString strDateRange = szDateRange;

	GetRange ( strDateRange );

	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.GetFutureDateTo( strDateRange, m_dateStart, m_dateEnd, m_dateFutureEnd );

	dateStart = m_dateStart;
	dateEnd = m_dateEnd;
	dateFutureEnd = m_dateFutureEnd;
}

/*****************************************************************************/

void CPosTrayDateRange::GetRange ( const char* szDateRange )
{
	CString strDateRange = szDateRange;

	if ( strDateRange == szRANGE_TODAY )						GetToday();
	else if ( strDateRange == szRANGE_THIS_BUSDAY )				GetToday();
	else if ( strDateRange == szRANGE_YESTERDAY )				GetYesterday();
	else if ( strDateRange == szRANGE_LAST_BUSDAY )				GetYesterday();
	else if ( strDateRange == szRANGE_SUN_THIS_WEEK )			GetSunThisWeek();
	else if ( strDateRange == szRANGE_MON_THIS_WEEK )			GetMonThisWeek();
	else if ( strDateRange == szRANGE_TUE_THIS_WEEK )			GetTueThisWeek();
	else if ( strDateRange == szRANGE_WED_THIS_WEEK )			GetWedThisWeek();
	else if ( strDateRange == szRANGE_THU_THIS_WEEK )			GetThuThisWeek();
	else if ( strDateRange == szRANGE_FRI_THIS_WEEK )			GetFriThisWeek();
	else if ( strDateRange == szRANGE_SAT_THIS_WEEK )			GetSatThisWeek();
	else if ( strDateRange == szRANGE_SUN_LAST_WEEK )			GetSunLastWeek();
	else if ( strDateRange == szRANGE_MON_LAST_WEEK )			GetMonLastWeek();
	else if ( strDateRange == szRANGE_TUE_LAST_WEEK )			GetTueLastWeek();
	else if ( strDateRange == szRANGE_WED_LAST_WEEK )			GetWedLastWeek();
	else if ( strDateRange == szRANGE_THU_LAST_WEEK )			GetThuLastWeek();
	else if ( strDateRange == szRANGE_FRI_LAST_WEEK )			GetFriLastWeek();
	else if ( strDateRange == szRANGE_SAT_LAST_WEEK )			GetSatLastWeek();
	else if ( strDateRange == szRANGE_PREVIOUS_7DAYS )			GetPrevious7Days();
	else if ( strDateRange == szRANGE_PREVIOUS_14DAYS )			GetPrevious14Days();
	else if ( strDateRange == szRANGE_PREVIOUS_21DAYS )			GetPrevious21Days();
	else if ( strDateRange == szRANGE_PREVIOUS_28DAYS )			GetPrevious28Days();
	else if ( strDateRange == szRANGE_PREVIOUS_13WEEKS )		GetPrevious13Weeks();
	else if ( strDateRange == szRANGE_THIS_WEEK )				GetThisWeek();
	else if ( strDateRange == szRANGE_THIS_MONTH )				GetThisMonth();
	else if ( strDateRange == szRANGE_THIS_VAT_QUARTER )		GetThisTaxQuarter();
	else if ( strDateRange == szRANGE_THIS_GST_QUARTER )		GetThisTaxQuarter();
	else if ( strDateRange == szRANGE_THIS_YEAR )				GetThisYear();
	else if ( strDateRange == szRANGE_THIS_FINANCIAL_YEAR )		GetThisFinancialYear();
	else if ( strDateRange == szRANGE_LAST_WEEK )				GetLastWeek();
	else if ( strDateRange == szRANGE_LAST_MONTH )				GetLastMonth();
	else if ( strDateRange == szRANGE_LAST_VAT_QUARTER )		GetLastTaxQuarter();
	else if ( strDateRange == szRANGE_LAST_GST_QUARTER )		GetLastTaxQuarter();
	else if ( strDateRange == szRANGE_LAST_YEAR )				GetLastYear();
	else if ( strDateRange == szRANGE_LAST_FINANCIAL_YEAR )		GetLastFinancialYear();
	else if ( strDateRange == szRANGE_LAST_3_MONTHS )			GetLast3Months();
	else if ( strDateRange == szRANGE_LAST_6_MONTHS )			GetLast6Months();
	else if ( strDateRange == szRANGE_LAST_9_MONTHS )			GetLast9Months();
	else if ( strDateRange == szRANGE_LAST_12_MONTHS )			GetLast12Months();
	else														GetToday();
}

/*****************************************************************************/

void CPosTrayDateRange::GetTimeToday ( COleDateTime& date )
{
	date = COleDateTime( m_dateToday.GetYear(), m_dateToday.GetMonth(), m_dateToday.GetDay(), 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::AddDays ( COleDateTime& time, int nDays )
{
	int nOldHour = time.GetHour();
	time += COleDateTimeSpan ( nDays, 0, 0, 0 );
	int nNewHour = time.GetHour();

	if ( ( nNewHour == nOldHour-1 ) || ( nNewHour == nOldHour+23 ) )
		time += COleDateTimeSpan ( 0, 1, 0, 0 );

	if ( ( nNewHour == nOldHour+1 ) || ( nNewHour == nOldHour-23 ) )
		time -= COleDateTimeSpan ( 0, 1, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::SubtractMonths ( COleDateTime& time, int nMonths )
{
	int nYear = time.GetYear();
	int nMonth = time.GetMonth() - nMonths;

	if ( nMonth <= 0 )
	{
		nMonth += 12;
		nYear -= 1;
	}
	time = COleDateTime ( nYear, nMonth, 1, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetToday()
{
	GetTimeToday ( m_dateStart );
	m_dateEnd = m_dateStart;
}

/*****************************************************************************/

void CPosTrayDateRange::GetYesterday()
{
	GetTimeToday ( m_dateStart );
	AddDays ( m_dateStart, -1 );
	m_dateEnd = m_dateStart;
}

/*****************************************************************************/

void CPosTrayDateRange::GetPrevious7Days()
{
	GetYesterday();
	AddDays( m_dateStart, -6 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetPrevious14Days()
{
	GetYesterday();
	AddDays( m_dateStart, -13 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetPrevious21Days()
{
	GetYesterday();
	AddDays( m_dateStart, -20 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetPrevious28Days()
{
	GetYesterday();
	AddDays( m_dateStart, -27 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetPrevious13Weeks()
{
	GetYesterday();
	AddDays( m_dateStart, -90 );
}

/*****************************************************************************/
void CPosTrayDateRange::GetSunThisWeek(){ GetWeekday( FALSE, 1 ); }
void CPosTrayDateRange::GetMonThisWeek(){ GetWeekday( FALSE, 2 ); }
void CPosTrayDateRange::GetTueThisWeek(){ GetWeekday( FALSE, 3 ); }
void CPosTrayDateRange::GetWedThisWeek(){ GetWeekday( FALSE, 4 ); }
void CPosTrayDateRange::GetThuThisWeek(){ GetWeekday( FALSE, 5 ); }
void CPosTrayDateRange::GetFriThisWeek(){ GetWeekday( FALSE, 6 ); }
void CPosTrayDateRange::GetSatThisWeek(){ GetWeekday( FALSE, 7 ); }
/*****************************************************************************/
void CPosTrayDateRange::GetSunLastWeek(){ GetWeekday( TRUE, 1 ); }
void CPosTrayDateRange::GetMonLastWeek(){ GetWeekday( TRUE, 2 ); }
void CPosTrayDateRange::GetTueLastWeek(){ GetWeekday( TRUE, 3 ); }
void CPosTrayDateRange::GetWedLastWeek(){ GetWeekday( TRUE, 4 ); }
void CPosTrayDateRange::GetThuLastWeek(){ GetWeekday( TRUE, 5 ); }
void CPosTrayDateRange::GetFriLastWeek(){ GetWeekday( TRUE, 6 ); }
void CPosTrayDateRange::GetSatLastWeek(){ GetWeekday( TRUE, 7 ); }
/*****************************************************************************/

void CPosTrayDateRange::GetWeekday( bool bLastWeek, int nWeekday )
{
	if ( FALSE == bLastWeek )
		GetThisWeek();
	else
		GetLastWeek();

	if ( nWeekday < m_nStartOfWeek )
		nWeekday += 7;

	if ( nWeekday != m_nStartOfWeek )
		AddDays( m_dateStart, nWeekday - m_nStartOfWeek );

	m_dateEnd = m_dateStart;
}

/*****************************************************************************/

void CPosTrayDateRange::GetThisWeek()
{
	GetTimeToday ( m_dateEnd );

	int nWeekday = m_dateEnd.GetDayOfWeek();

	if ( nWeekday < m_nStartOfWeek )
		nWeekday += 7;

	m_dateStart = m_dateEnd;
	AddDays ( m_dateStart, m_nStartOfWeek - nWeekday );
}

/*****************************************************************************/

void CPosTrayDateRange::GetThisMonth()
{
	GetTimeToday ( m_dateEnd );
	m_dateStart = COleDateTime ( m_dateEnd.GetYear(), m_dateEnd.GetMonth(), 1, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetThisTaxQuarter()
{
	GetTimeToday ( m_dateEnd );

	// adjust microsoft month now to start from zero
	int nMonthNow = m_dateEnd.GetMonth() - 1;
	int nYearNow = m_dateEnd.GetYear();

	// start from the last tax quarter and work backwards
	int nTaxQuarterMonth = 9 + ( m_nTaxQuarterMonth % 3 ) - 1;
	while ( nTaxQuarterMonth > nMonthNow )
		nTaxQuarterMonth -= 3;

	// add one to tax quarter month to make microsoft month
	if ( nTaxQuarterMonth >= 0 )
		m_dateStart = COleDateTime ( m_dateEnd.GetYear(), nTaxQuarterMonth+1, 1, 0, 0, 0 );
	else
		m_dateStart = COleDateTime ( m_dateEnd.GetYear()-1, nTaxQuarterMonth+13, 1, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetThisYear()
{
	GetTimeToday ( m_dateEnd );
	m_dateStart = COleDateTime ( m_dateEnd.GetYear(), 1, 1, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetThisFinancialYear()
{
	GetTimeToday ( m_dateEnd );
	
	m_dateStart = COleDateTime ( m_dateEnd.GetYear(), m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0 );

	if ( m_dateStart > m_dateEnd )
		m_dateStart = COleDateTime ( m_dateEnd.GetYear()-1, m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLastWeek()
{
	GetTimeToday ( m_dateEnd );

	int nWeekday = m_dateEnd.GetDayOfWeek();

	if ( nWeekday < m_nStartOfWeek ) nWeekday += 7;
		AddDays ( m_dateEnd, m_nStartOfWeek - nWeekday - 1 );

	m_dateStart = m_dateEnd;

	AddDays ( m_dateStart, -6 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLastMonth()
{
	GetTimeToday ( m_dateEnd );
	m_dateEnd = COleDateTime ( m_dateEnd.GetYear(), m_dateEnd.GetMonth(), 1, 0, 0, 0 );
	AddDays ( m_dateEnd, -1 );
	m_dateStart = COleDateTime ( m_dateEnd.GetYear(), m_dateEnd.GetMonth(), 1, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLastTaxQuarter()
{
	GetThisTaxQuarter();
	m_dateEnd = m_dateStart;
	AddDays ( m_dateEnd, -1 );
	SubtractMonths ( m_dateStart, 3 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLastYear()
{
	GetTimeToday ( m_dateEnd );
	m_dateEnd = COleDateTime ( m_dateEnd.GetYear(), 1, 1, 0, 0, 0 );
	AddDays ( m_dateEnd, -1 );
	m_dateStart = COleDateTime ( m_dateEnd.GetYear(), 1, 1, 0, 0, 0 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLastFinancialYear()
{
	COleDateTime dateNow;
	GetTimeToday ( dateNow );

	m_dateEnd = COleDateTime ( dateNow.GetYear(), m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0 );
	
	if ( m_dateEnd > dateNow )
		m_dateEnd = COleDateTime ( dateNow.GetYear()-1, m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0 );

	m_dateStart = COleDateTime ( m_dateEnd.GetYear()-1, m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0 );
	AddDays ( m_dateEnd, -1 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLast3Months()
{
	GetTimeToday ( m_dateEnd );
	m_dateStart = m_dateEnd;
	SubtractMonths ( m_dateStart, 3 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLast6Months()
{
	GetTimeToday ( m_dateEnd );
	m_dateStart = m_dateEnd;
	SubtractMonths ( m_dateStart, 6 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLast9Months()
{
	GetTimeToday ( m_dateEnd );
	m_dateStart = m_dateEnd;
	SubtractMonths ( m_dateStart, 9 );
}

/*****************************************************************************/

void CPosTrayDateRange::GetLast12Months()
{
	GetTimeToday ( m_dateEnd );
	m_dateStart = m_dateEnd;
	SubtractMonths ( m_dateStart, 12 );
}

/*****************************************************************************/
