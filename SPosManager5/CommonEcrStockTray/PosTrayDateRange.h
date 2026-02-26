#pragma once
/**********************************************************************/
 
/**********************************************************************/

class CPosTrayDateRange
{
public:
	CPosTrayDateRange();
	void SetDateToday( COleDateTime& date );
	void SetDateToday( COleDateTime& date, int nSODHour, int nSODMinute );
	void GetRange ( const char* szDateRange, COleDateTime& dateStart, COleDateTime& dateEnd, COleDateTime& dateFutureEnd );
	
private:
	void GetRange ( const char* szDateRange );
	void GetTimeToday ( COleDateTime& time );
	void AddDays ( COleDateTime& time, int nDays );
	void SubtractMonths ( COleDateTime& time, int nMonths );

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
	COleDateTime m_dateStart;
	COleDateTime m_dateEnd;
	COleDateTime m_dateFutureEnd;
	COleDateTime m_dateToday;

	int m_nStartOfWeek;			// 1 = sun, 2 = mon ...  7 = sat
	int m_nTaxQuarterMonth;		// 1 = jan, 2 = feb ... 12 = dec
	int m_nFinancialYearDay;		// 1 to 31
	int m_nFinancialYearMonth;	// 1 = jan, 2 = feb ... 12 = dec
};

/**********************************************************************/
