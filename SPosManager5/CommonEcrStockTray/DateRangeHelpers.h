#pragma once
/**********************************************************************/

class CDateRangeHelpers  
{
public:
	CDateRangeHelpers();

public:
	int GetWeekNo( COleDateTime dateToCheck );

	void GetDateRange ( const char* szText, CTime& timeFrom, CTime& timeTo );
	void GetOleDateRange ( const char* szText, COleDateTime& timeFrom, COleDateTime& timeTo );
	
	void GetPastAndFutureDateRange ( const char* szText, CTime& timeFrom, CTime& timeTo, bool bFromToday );
	void GetPastAndFutureOleDateRange ( const char* szText, COleDateTime& timeFrom, COleDateTime& timeTo, bool bExpand );

	void GetCustomerSyncDateRange ( const char* szText, CTime& timeFrom, CTime& timeTo );
	void GetCustomerSyncOleDateRange ( const char* szText, COleDateTime& timeFrom, COleDateTime& timeTo );
	
	void GetLYDateRange( const char* szText, CTime& timeFrom, CTime& timeTo );
	void GetLYOleDateRange ( const char* szText, COleDateTime& timeFrom, COleDateTime& timeTo );
	
	void GetEarliestReportDateString ( CString& strType );

	void AddDaysToTime ( CTime& time, int nDays );
	void AddDaysToTime ( COleDateTime& time, int nDays );

	void GetFutureDateTo( CString& strDateRange, COleDateTime& oleDateFrom, COleDateTime& oleDateTo, COleDateTime& oleFutureDateTo );

private:
	void GetWeek1StartDate( int nYear, COleDateTime& dateWeek1Start, int nYearStartMonth, int nYearStartDay );
	int GetWeekNo( COleDateTime dateToCheck, int nYearStartMonth, int nYearStartDay );	

private:
	void GetLYDay( CTime& timeFrom, CTime& timeTo );
	void GetLYWeek( CTime& timeFrom, CTime& timeTo );
	void GetLYMonth( CTime& timeFrom, CTime& timeTo );
	void GetLYQuarter( CTime& timeFrom, CTime& timeTo );
	void GetLYYear( CTime& timeFrom, CTime& timeTo );
};

/**********************************************************************/
