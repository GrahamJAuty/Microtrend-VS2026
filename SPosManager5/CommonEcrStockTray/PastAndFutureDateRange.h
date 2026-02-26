#pragma once
/**********************************************************************/
extern const char* szRANGE_TOMORROW;
extern const char* szRANGE_NEXT_WEEK;
extern const char* szRANGE_NEXT_MONTH;
extern const char* szRANGE_NEXT_YEAR;
extern const char* szRANGE_NEXT_3_MONTHS;
extern const char* szRANGE_NEXT_6_MONTHS;
extern const char* szRANGE_NEXT_9_MONTHS;
extern const char* szRANGE_NEXT_12_MONTHS;
/**********************************************************************/
 
class CPastAndFutureDateRange
{
public:
	CPastAndFutureDateRange();
	CPastAndFutureDateRange ( int nStartOfWeek, int nTaxQuarterMonth, int nFinancialYearDay, int nFinancialYearMonth );
	void GetRange ( const char* szDateRange, CTime& dateStart, CTime& dateEnd, bool bFromToday );
	void GetRange ( const char* szDateRange, CSSDate& dateStart, CSSDate& dateEnd, bool bFromToday );

private:
	void GetRange ( const char* szDateRange, bool bFromToday );
	void GetTimeToday ( CTime& time );
	void AddDays ( CTime& time, int nDays );
	void AddMonths ( CTime& time, int nMonths );

	void GetToday();
	void GetTomorrow();
	void GetYesterday();
	void GetThisWeek( bool bFromToday, bool bFullWeek = FALSE );
	void GetThisMonth( bool bFromToday );
	void GetThisYear( bool bFromToday );
	void GetNextWeek();
	void GetNextMonth();
	void GetNextYear();
	void GetNext3Months();
	void GetNext6Months();
	void GetNext9Months();
	void GetNext12Months();
	void GetLastWeek();
	void GetLastMonth();
	void GetLastYear();
	void GetLast3Months();
	void GetLast6Months();
	void GetLast9Months();
	void GetLast12Months();
	
private:
	CTime m_timeStart;
	CTime m_timeEnd;

	int m_nStartOfWeek;			// 1 = sun, 2 = mon ...  7 = sat
	int m_nTaxQuarterMonth;		// 1 = jan, 2 = feb ... 12 = dec
	int m_nFinancialYearDay;		// 1 to 31
	int m_nFinancialYearMonth;	// 1 = jan, 2 = feb ... 12 = dec
};

/**********************************************************************/
