/**********************************************************************/
#pragma once
/**********************************************************************/

/**********************************************************************/
/*	SSRANGE.CPP											*/
/**********************************************************************/

extern const char* szRANGE_TODAY;
extern const char* szRANGE_YESTERDAY;
extern const char* szRANGE_THIS_BUSDAY;
extern const char* szRANGE_LAST_BUSDAY;
extern const char* szRANGE_SUN_THIS_WEEK;
extern const char* szRANGE_MON_THIS_WEEK;
extern const char* szRANGE_TUE_THIS_WEEK;
extern const char* szRANGE_WED_THIS_WEEK;
extern const char* szRANGE_THU_THIS_WEEK;
extern const char* szRANGE_FRI_THIS_WEEK;
extern const char* szRANGE_SAT_THIS_WEEK;
extern const char* szRANGE_SUN_LAST_WEEK;
extern const char* szRANGE_MON_LAST_WEEK;
extern const char* szRANGE_TUE_LAST_WEEK;
extern const char* szRANGE_WED_LAST_WEEK;
extern const char* szRANGE_THU_LAST_WEEK;
extern const char* szRANGE_FRI_LAST_WEEK;
extern const char* szRANGE_SAT_LAST_WEEK;
extern const char* szRANGE_PREVIOUS_7DAYS;
extern const char* szRANGE_PREVIOUS_14DAYS;
extern const char* szRANGE_PREVIOUS_21DAYS;
extern const char* szRANGE_PREVIOUS_28DAYS;
extern const char* szRANGE_PREVIOUS_13WEEKS;
extern const char* szRANGE_THIS_WEEK;
extern const char* szRANGE_THIS_MONTH;
extern const char* szRANGE_THIS_VAT_QUARTER;
extern const char* szRANGE_THIS_GST_QUARTER;
extern const char* szRANGE_THIS_YEAR;
extern const char* szRANGE_THIS_FINANCIAL_YEAR;
extern const char* szRANGE_LAST_WEEK;
extern const char* szRANGE_LAST_MONTH;
extern const char* szRANGE_LAST_VAT_QUARTER;
extern const char* szRANGE_LAST_GST_QUARTER;
extern const char* szRANGE_LAST_YEAR;
extern const char* szRANGE_LAST_FINANCIAL_YEAR;
extern const char* szRANGE_LAST_3_MONTHS;
extern const char* szRANGE_LAST_6_MONTHS;
extern const char* szRANGE_LAST_9_MONTHS;
extern const char* szRANGE_LAST_12_MONTHS;
extern const char* szRANGE_CUSTOM_DATE;		// not implemented by CSSDateRange
extern const char* szRANGE_CUSTOM_RANGE;	// not implemented by CSSDateRange
extern const char* szRANGE_ALL;				// not implemented by CSSDateRange

/**********************************************************************/
/*	SSDATE.CPP														  */
/**********************************************************************/

class CSSDate
{
public:
	CSSDate() { m_strDate = ""; }
	CSSDate ( const char* szDate );				/* dd/mm/yyyy & ddmmyyyy */
	CSSDate ( const CSSDate& d );					/* copy constructor */
	CSSDate& operator= ( const CSSDate& d );		/* assignment operator */

	bool operator<  ( const CSSDate& d ) { return ( m_strDate <  d.m_strDate ); }
	bool operator<= ( const CSSDate& d ) { return ( m_strDate <= d.m_strDate ); }
	bool operator>  ( const CSSDate& d ) { return ( m_strDate >  d.m_strDate ); }
	bool operator>= ( const CSSDate& d ) { return ( m_strDate >= d.m_strDate ); }
	bool operator== ( const CSSDate& d ) { return ( m_strDate == d.m_strDate ); }
	bool operator!= ( const CSSDate& d ) { return ( m_strDate != d.m_strDate ); }

	static CSSDate GetCurrentDate();

	void SetCurrentDate();
	bool SetDate ( CTime& time );
	bool SetDate ( const char* szDate );			/* dd/mm/yyyy & ddmmyyyy */
	const char* GetDate();						/* dd/mm/yyyy */
	const char* GetCSVDate();					/* ddmmyyyy */
	int GetDay();								/* 0 or 1 - 31 */
	int GetMonth();							/* 0 or 1 - 12 */
	int GetYear();								/* 0 or yyyy */
	int GetDayOfWeek();							/* 1=sunday 2=monday..7=saturday (0=not set) */
	bool IsSet();

private:
	void Validate ( const char* szDate );

private:
	CString m_strDate;							/* yyyymmdd */
	CString m_strFormatted;

	static const int nMonthArray[12];
};

/**********************************************************************/
/*	SSTIME.CPP											*/
/**********************************************************************/

class CSSTime
{
public:
	CSSTime() { m_strTime = ""; }
	CSSTime ( const char* szTime );				/* hh:mm, hh:mm:ss, hhmm, hhmmss */
	CSSTime ( const CSSTime& t );					/* copy constructor */
	CSSTime& operator= ( const CSSTime& t );		/* assignment operator */

	bool operator<  ( const CSSTime& t ) { return ( m_strTime <  t.m_strTime ); }
	bool operator<= ( const CSSTime& t ) { return ( m_strTime <= t.m_strTime ); }
	bool operator>  ( const CSSTime& t ) { return ( m_strTime >  t.m_strTime ); }
	bool operator>= ( const CSSTime& t ) { return ( m_strTime >= t.m_strTime ); }
	bool operator== ( const CSSTime& t ) { return ( m_strTime == t.m_strTime ); }
	bool operator!= ( const CSSTime& t ) { return ( m_strTime != t.m_strTime ); }

	static CSSTime GetCurrentTime();

	void SetCurrentTime();
	bool SetTime ( CTime& time );
	bool SetTime ( const char* szTime );			/* hh:mm, hh:mm:ss, hhmm, hhmmss */
	const char* GetTime();						/* hh:mm */
	const char* GetCSVTime();					/* hhmm */
	int GetHour();								/* 0, 0 - 23 */
	int GetMinute();							/* 0, 0 - 59 */
	int GetSecond();							/* 0, 0 - 59 */
	bool IsSet();

private:
	void Validate ( const char* szTime );

private:
	CString m_strTime;							/* hhmmss */
	CString m_strFormatted;
};

/**********************************************************************/
/*	DATETIME.CPP											*/
/**********************************************************************/

class CSSDateTime
{
public:
	CSSDateTime ( const char* szDate, const char* szTime );	/* ddmmyyyy, dd/mm/yyyy and hhmm, hh:mm, hhmmsss, hh:mm:ss */

	bool operator<  ( CSSDateTime& dt );
	bool operator<= ( CSSDateTime& dt );
	bool operator>  ( CSSDateTime& dt );
	bool operator>= ( CSSDateTime& dt );
	bool operator== ( CSSDateTime& dt );
	bool operator!= ( CSSDateTime& dt );

	bool IsSet();

private:
	CString m_strDateTime;					/* yyyymmddhhmmss */
	CSSDate m_date;
	CSSTime m_time;
};

/**********************************************************************/
/*	SSRANGE.CPP											*/
/**********************************************************************/

class CSSDateRange
{
public:
	CSSDateRange();
	CSSDateRange( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth );
	CSSDateRange( int nSODHour, int nSODMinute );
	CSSDateRange( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth, int nSODHour, int nSODMinute );

public:
	void GetRange ( const char* szDateRange, CTime& dateStart, CTime& dateEnd );
	void GetRange ( const char* szDateRange, CSSDate& dateStart, CSSDate& dateEnd );

private:
	void Init ( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth, int nSODHour, int nSODMinute );
	
private:
	void GetRange ( const char* szDateRange );
	void GetTimeToday ( CTime& time );
	void AddDays ( CTime& time, int nDays );
	void SubtractMonths ( CTime& time, int nMonths );

	void GetToday();
	void GetYesterday();
	void GetSunThisWeek();
	void GetMonThisWeek();
	void GetTueThisWeek();
	void GetWedThisWeek();
	void GetThuThisWeek();
	void GetFriThisWeek();
	void GetSatThisWeek();
	void GetSunLastWeek();
	void GetMonLastWeek();
	void GetTueLastWeek();
	void GetWedLastWeek();
	void GetThuLastWeek();
	void GetFriLastWeek();
	void GetSatLastWeek();
	void GetPrevious7Days();
	void GetPrevious14Days();
	void GetPrevious21Days();
	void GetPrevious28Days();
	void GetPrevious13Weeks();
	void GetThisWeek();
	void GetThisMonth();
	void GetThisTaxQuarter();
	void GetThisYear();
	void GetThisFinancialYear();
	void GetLastWeek();
	void GetLastMonth();
	void GetLastTaxQuarter();
	void GetLastYear();
	void GetLastFinancialYear();
	void GetLast3Months();
	void GetLast6Months();
	void GetLast9Months();
	void GetLast12Months();

	void GetWeekday( bool bLastWeek, int nWeekday );

private:
	CTime m_timeStart;
	CTime m_timeEnd;

	int m_nStartOfWeek;			// 1 = sun, 2 = mon ...  7 = sat
	int m_nTaxQuarterMonth;		// 1 = jan, 2 = feb ... 12 = dec
	int m_nFinancialYearDay;		// 1 to 31
	int m_nFinancialYearMonth;	// 1 = jan, 2 = feb ... 12 = dec

	int m_nSODHour;
	int m_nSODMinute;
};

/**********************************************************************/

extern const char* szVERSION_DATETIMESUBS;

/**********************************************************************/
