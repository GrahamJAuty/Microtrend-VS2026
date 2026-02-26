/*****************************************************************************/

const char* szRANGE_TODAY				= "Today";
const char* szRANGE_YESTERDAY			= "Yesterday";
const char* szRANGE_THIS_BUSDAY			= "Current Business Day";
const char* szRANGE_LAST_BUSDAY			= "Previous Business Day";
const char* szRANGE_SUN_THIS_WEEK		= "This Week: Sunday";
const char* szRANGE_MON_THIS_WEEK		= "This Week: Monday ";
const char* szRANGE_TUE_THIS_WEEK		= "This Week: Tuesday";
const char* szRANGE_WED_THIS_WEEK		= "This Week: Wednesday";
const char* szRANGE_THU_THIS_WEEK		= "This Week: Thursday";
const char* szRANGE_FRI_THIS_WEEK		= "This Week: Friday";
const char* szRANGE_SAT_THIS_WEEK		= "This Week: Saturday";
const char* szRANGE_SUN_LAST_WEEK		= "Last Week: Sunday";
const char* szRANGE_MON_LAST_WEEK		= "Last Week: Monday";
const char* szRANGE_TUE_LAST_WEEK		= "Last Week: Tuesday";
const char* szRANGE_WED_LAST_WEEK		= "Last Week: Wednesday";
const char* szRANGE_THU_LAST_WEEK		= "Last Week: Thursday";
const char* szRANGE_FRI_LAST_WEEK		= "Last Week: Friday";
const char* szRANGE_SAT_LAST_WEEK		= "Last Week: Saturday";
const char* szRANGE_PREVIOUS_7DAYS		= "Previous 7 days";
const char* szRANGE_PREVIOUS_14DAYS		= "Previous 14 days";
const char* szRANGE_PREVIOUS_21DAYS		= "Previous 21 days";
const char* szRANGE_PREVIOUS_28DAYS		= "Previous 28 days";
const char* szRANGE_PREVIOUS_13WEEKS	= "Previous 13 weeks";
const char* szRANGE_THIS_WEEK			= "This Week";
const char* szRANGE_THIS_MONTH			= "This Month";
const char* szRANGE_THIS_VAT_QUARTER	= "This VAT Quarter";
const char* szRANGE_THIS_GST_QUARTER	= "This GST Quarter";
const char* szRANGE_THIS_YEAR			= "This Year";
const char* szRANGE_THIS_FINANCIAL_YEAR	= "This Financial Year";
const char* szRANGE_LAST_WEEK			= "Last Week";
const char* szRANGE_LAST_MONTH			= "Last Month";	
const char* szRANGE_LAST_VAT_QUARTER	= "Last VAT Quarter";
const char* szRANGE_LAST_GST_QUARTER	= "Last GST Quarter";
const char* szRANGE_LAST_YEAR			= "Last Year";
const char* szRANGE_LAST_FINANCIAL_YEAR	= "Last Financial Year";
const char* szRANGE_LAST_3_MONTHS		= "Last 3 Months";
const char* szRANGE_LAST_6_MONTHS		= "Last 6 Months";
const char* szRANGE_LAST_9_MONTHS		= "Last 9 Months";
const char* szRANGE_LAST_12_MONTHS		= "Last 12 Months";
const char* szRANGE_CUSTOM_DATE			= "Custom Date";	// not implemented by CSSDateRange
const char* szRANGE_CUSTOM_RANGE		= "Custom Range";	// not implemented by CSSDateRange
const char* szRANGE_ALL					= "All";			// not implemented by CSSDateRange

/*****************************************************************************/

CSSDateRange::CSSDateRange() 
{
	Init( 1, 1, 1, 1, 0, 0 );
}

/*****************************************************************************/

CSSDateRange::CSSDateRange( int nSODHour, int nSODMinute )
{
	Init( 1, 1, 1, 1, nSODHour, nSODMinute );
}

/*****************************************************************************/

CSSDateRange::CSSDateRange( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth )
{
	Init( nStartOfWeek, nTaxQuarterMonth, nFinancialYearDay, nFinancialYearMonth, 0, 0 );
}

/*****************************************************************************/

CSSDateRange::CSSDateRange( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth, int nSODHour, int nSODMinute )
{
	Init( nStartOfWeek, nTaxQuarterMonth, nFinancialYearDay, nFinancialYearMonth, nSODHour, nSODMinute );
}

/*****************************************************************************/

void CSSDateRange::Init ( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth, int nSODHour, int nSODMinute )
{
	m_nStartOfWeek = ( nStartOfWeek >= 1 && nStartOfWeek <= 7 ) ? nStartOfWeek : 1;
	m_nTaxQuarterMonth = ( nTaxQuarterMonth >= 1 && nTaxQuarterMonth <= 12 ) ? nTaxQuarterMonth : 1;
	m_nFinancialYearDay = ( nFinancialYearDay >= 1 && nFinancialYearDay <= 31 ) ? nFinancialYearDay : 1;
	m_nFinancialYearMonth = ( nFinancialYearMonth >= 1 && nFinancialYearMonth <= 12 ) ? nFinancialYearMonth : 1;
	m_nSODHour = ( nSODHour >= 0 ) && ( nSODHour <= 23 ) ? nSODHour : 0;
	m_nSODMinute = ( nSODMinute >= 0 ) && ( nSODMinute <= 23 ) ? nSODMinute : 0;
}

/*****************************************************************************/

void CSSDateRange::GetRange ( const char* szDateRange, CTime& dateStart, CTime& dateEnd )
{
	GetRange ( szDateRange );

	dateStart = m_timeStart;
	dateEnd = m_timeEnd;
}

/*****************************************************************************/

void CSSDateRange::GetRange ( const char* szDateRange, CSSDate& dateStart, CSSDate& dateEnd )
{
	GetRange ( szDateRange );

	dateStart.SetDate ( m_timeStart );
	dateEnd.SetDate ( m_timeEnd );
}

/*****************************************************************************/

void CSSDateRange::GetRange ( const char* szDateRange )
{
	CString strDateRange = szDateRange;

	if ( strDateRange == szRANGE_TODAY )					GetToday();
	else if ( strDateRange == szRANGE_YESTERDAY )			GetYesterday();
	else if ( strDateRange == szRANGE_THIS_BUSDAY )			GetToday();
	else if ( strDateRange == szRANGE_LAST_BUSDAY )			GetYesterday();
	else if ( strDateRange == szRANGE_SUN_THIS_WEEK )		GetSunThisWeek();
	else if ( strDateRange == szRANGE_MON_THIS_WEEK )		GetMonThisWeek();
	else if ( strDateRange == szRANGE_TUE_THIS_WEEK )		GetTueThisWeek();
	else if ( strDateRange == szRANGE_WED_THIS_WEEK )		GetWedThisWeek();
	else if ( strDateRange == szRANGE_THU_THIS_WEEK )		GetThuThisWeek();
	else if ( strDateRange == szRANGE_FRI_THIS_WEEK )		GetFriThisWeek();
	else if ( strDateRange == szRANGE_SAT_THIS_WEEK )		GetSatThisWeek();
	else if ( strDateRange == szRANGE_SUN_LAST_WEEK )		GetSunLastWeek();
	else if ( strDateRange == szRANGE_MON_LAST_WEEK )		GetMonLastWeek();
	else if ( strDateRange == szRANGE_TUE_LAST_WEEK )		GetTueLastWeek();
	else if ( strDateRange == szRANGE_WED_LAST_WEEK )		GetWedLastWeek();
	else if ( strDateRange == szRANGE_THU_LAST_WEEK )		GetThuLastWeek();
	else if ( strDateRange == szRANGE_FRI_LAST_WEEK )		GetFriLastWeek();
	else if ( strDateRange == szRANGE_SAT_LAST_WEEK )		GetSatLastWeek();
	else if ( strDateRange == szRANGE_PREVIOUS_7DAYS )		GetPrevious7Days();
	else if ( strDateRange == szRANGE_PREVIOUS_14DAYS )		GetPrevious14Days();
	else if ( strDateRange == szRANGE_PREVIOUS_21DAYS )		GetPrevious21Days();
	else if ( strDateRange == szRANGE_PREVIOUS_28DAYS )		GetPrevious28Days();
	else if ( strDateRange == szRANGE_PREVIOUS_13WEEKS )	GetPrevious13Weeks();
	else if ( strDateRange == szRANGE_THIS_WEEK )			GetThisWeek();
	else if ( strDateRange == szRANGE_THIS_MONTH )			GetThisMonth();
	else if ( strDateRange == szRANGE_THIS_VAT_QUARTER )	GetThisTaxQuarter();
	else if ( strDateRange == szRANGE_THIS_GST_QUARTER )	GetThisTaxQuarter();
	else if ( strDateRange == szRANGE_THIS_YEAR )			GetThisYear();
	else if ( strDateRange == szRANGE_THIS_FINANCIAL_YEAR )	GetThisFinancialYear();
	else if ( strDateRange == szRANGE_LAST_WEEK )			GetLastWeek();
	else if ( strDateRange == szRANGE_LAST_MONTH )			GetLastMonth();
	else if ( strDateRange == szRANGE_LAST_VAT_QUARTER )	GetLastTaxQuarter();
	else if ( strDateRange == szRANGE_LAST_GST_QUARTER )	GetLastTaxQuarter();
	else if ( strDateRange == szRANGE_LAST_YEAR )			GetLastYear();
	else if ( strDateRange == szRANGE_LAST_FINANCIAL_YEAR )	GetLastFinancialYear();
	else if ( strDateRange == szRANGE_LAST_3_MONTHS )		GetLast3Months();
	else if ( strDateRange == szRANGE_LAST_6_MONTHS )		GetLast6Months();
	else if ( strDateRange == szRANGE_LAST_9_MONTHS )		GetLast9Months();
	else if ( strDateRange == szRANGE_LAST_12_MONTHS )		GetLast12Months();
	else													GetToday();
}

/*****************************************************************************/

void CSSDateRange::GetTimeToday ( CTime& time )
{
	time = CTime::GetCurrentTime();
	time = CTime ( time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0, -1 );

	int nMinutesNow = ( time.GetHour() * 60 ) + time.GetMinute();
	int nMinutesSOD = ( m_nSODHour * 60 ) + m_nSODMinute;
	
	if ( nMinutesNow < nMinutesSOD )
		AddDays( time, -1 );
}

/*****************************************************************************/

void CSSDateRange::AddDays ( CTime& time, int nDays )
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

void CSSDateRange::SubtractMonths ( CTime& time, int nMonths )
{
	int nYear = time.GetYear();
	int nMonth = time.GetMonth() - nMonths;

	if ( nMonth <= 0 )
	{
		nMonth += 12;
		nYear -= 1;
	}
	time = CTime ( nYear, nMonth, 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetToday()
{
	GetTimeToday ( m_timeStart );
	m_timeEnd = m_timeStart;
}

/*****************************************************************************/

void CSSDateRange::GetYesterday()
{
	GetTimeToday ( m_timeStart );
	AddDays ( m_timeStart, -1 );
	m_timeEnd = m_timeStart;
}

/*****************************************************************************/

void CSSDateRange::GetPrevious7Days()
{
	GetYesterday();
	AddDays ( m_timeStart, -6 );
}

/*****************************************************************************/

void CSSDateRange::GetPrevious14Days()
{
	GetYesterday();
	AddDays ( m_timeStart, -13 );
}

/*****************************************************************************/

void CSSDateRange::GetPrevious21Days()
{
	GetYesterday();
	AddDays ( m_timeStart, -20 );
}

/*****************************************************************************/

void CSSDateRange::GetPrevious28Days()
{
	GetYesterday();
	AddDays ( m_timeStart, -27 );
}

/*****************************************************************************/

void CSSDateRange::GetPrevious13Weeks()
{
	GetYesterday();
	AddDays ( m_timeStart, -90 );
}

/*****************************************************************************/
void CSSDateRange::GetSunThisWeek(){ GetWeekday( FALSE, 1 ); }
void CSSDateRange::GetMonThisWeek(){ GetWeekday( FALSE, 2 ); }
void CSSDateRange::GetTueThisWeek(){ GetWeekday( FALSE, 3 ); }
void CSSDateRange::GetWedThisWeek(){ GetWeekday( FALSE, 4 ); }
void CSSDateRange::GetThuThisWeek(){ GetWeekday( FALSE, 5 ); }
void CSSDateRange::GetFriThisWeek(){ GetWeekday( FALSE, 6 ); }
void CSSDateRange::GetSatThisWeek(){ GetWeekday( FALSE, 7 ); }
/*****************************************************************************/
void CSSDateRange::GetSunLastWeek(){ GetWeekday( TRUE, 1 ); }
void CSSDateRange::GetMonLastWeek(){ GetWeekday( TRUE, 2 ); }
void CSSDateRange::GetTueLastWeek(){ GetWeekday( TRUE, 3 ); }
void CSSDateRange::GetWedLastWeek(){ GetWeekday( TRUE, 4 ); }
void CSSDateRange::GetThuLastWeek(){ GetWeekday( TRUE, 5 ); }
void CSSDateRange::GetFriLastWeek(){ GetWeekday( TRUE, 6 ); }
void CSSDateRange::GetSatLastWeek(){ GetWeekday( TRUE, 7 ); }
/*****************************************************************************/

void CSSDateRange::GetWeekday( bool bLastWeek, int nWeekday )
{
	if ( FALSE == bLastWeek )
		GetThisWeek();
	else
		GetLastWeek();

	if ( nWeekday < m_nStartOfWeek )
		nWeekday += 7;

	if ( nWeekday != m_nStartOfWeek )
		AddDays( m_timeStart, nWeekday - m_nStartOfWeek );

	m_timeEnd = m_timeStart;
}

/*****************************************************************************/

void CSSDateRange::GetThisWeek()
{
	GetTimeToday ( m_timeEnd );

	int nWeekday = m_timeEnd.GetDayOfWeek();

	if ( nWeekday < m_nStartOfWeek )
		nWeekday += 7;

	m_timeStart = m_timeEnd;

	AddDays ( m_timeStart, m_nStartOfWeek - nWeekday );
}

/*****************************************************************************/

void CSSDateRange::GetThisMonth()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = CTime ( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetThisTaxQuarter()
{
	GetTimeToday ( m_timeEnd );

	// adjust microsoft month now to start from zero
	int nMonthNow = m_timeEnd.GetMonth() - 1;
	int nYearNow = m_timeEnd.GetYear();

	// start from the last tax quarter and work backwards
	int nTaxQuarterMonth = 9 + ( m_nTaxQuarterMonth % 3 ) - 1;
	while ( nTaxQuarterMonth > nMonthNow )
		nTaxQuarterMonth -= 3;

	// add one to tax quarter month to make microsoft month
	if ( nTaxQuarterMonth >= 0 )
		m_timeStart = CTime ( m_timeEnd.GetYear(), nTaxQuarterMonth+1, 1, 0, 0, 0, -1 );
	else
		m_timeStart = CTime ( m_timeEnd.GetYear()-1, nTaxQuarterMonth+13, 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetThisYear()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = CTime ( m_timeEnd.GetYear(), 1, 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetThisFinancialYear()
{
	GetTimeToday ( m_timeEnd );
	
	m_timeStart = CTime ( m_timeEnd.GetYear(), m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0, -1 );

	if ( m_timeStart > m_timeEnd )
		m_timeStart = CTime ( m_timeEnd.GetYear()-1, m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetLastWeek()
{
	GetTimeToday ( m_timeEnd );

	int nWeekday = m_timeEnd.GetDayOfWeek();

	if ( nWeekday < m_nStartOfWeek ) nWeekday += 7;
		AddDays ( m_timeEnd, m_nStartOfWeek - nWeekday - 1 );

	m_timeStart = m_timeEnd;

	AddDays ( m_timeStart, -6 );
}

/*****************************************************************************/

void CSSDateRange::GetLastMonth()
{
	GetTimeToday ( m_timeEnd );
	m_timeEnd = CTime ( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), 1, 0, 0, 0, -1 );
	AddDays ( m_timeEnd, -1 );
	m_timeStart = CTime ( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetLastTaxQuarter()
{
	GetThisTaxQuarter();
	m_timeEnd = m_timeStart;
	AddDays ( m_timeEnd, -1 );
	SubtractMonths ( m_timeStart, 3 );
}

/*****************************************************************************/

void CSSDateRange::GetLastYear()
{
	GetTimeToday ( m_timeEnd );
	m_timeEnd = CTime ( m_timeEnd.GetYear(), 1, 1, 0, 0, 0, -1 );
	AddDays ( m_timeEnd, -1 );
	m_timeStart = CTime ( m_timeEnd.GetYear(), 1, 1, 0, 0, 0, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetLastFinancialYear()
{
	CTime timeNow;
	GetTimeToday ( timeNow );

	m_timeEnd = CTime ( timeNow.GetYear(), m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0, -1 );
	
	if ( m_timeEnd > timeNow )
		m_timeEnd = CTime ( timeNow.GetYear()-1, m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0, -1 );

	m_timeStart = CTime ( m_timeEnd.GetYear()-1, m_nFinancialYearMonth, m_nFinancialYearDay, 0, 0, 0, -1 );
	AddDays ( m_timeEnd, -1 );
}

/*****************************************************************************/

void CSSDateRange::GetLast3Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	SubtractMonths ( m_timeStart, 3 );
}

/*****************************************************************************/

void CSSDateRange::GetLast6Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	SubtractMonths ( m_timeStart, 6 );
}

/*****************************************************************************/

void CSSDateRange::GetLast9Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	SubtractMonths ( m_timeStart, 9 );
}

/*****************************************************************************/

void CSSDateRange::GetLast12Months()
{
	GetTimeToday ( m_timeEnd );
	m_timeStart = m_timeEnd;
	SubtractMonths ( m_timeStart, 12 );
}

/*****************************************************************************/
