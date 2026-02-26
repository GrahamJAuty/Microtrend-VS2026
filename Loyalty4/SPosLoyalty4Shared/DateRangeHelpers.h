#pragma once
/**********************************************************************/

class CDateRangeHelpers  
{
public:
	CDateRangeHelpers();

public:
	void GetDateRange ( const char* szText, CTime& timeFrom, CTime& timeTo );
	void GetExpiryDateRange( const char* szRangeText, CTime& timeFrom, CTime& timeTo, bool bFromToday );

public:
	void AddDaysToTime(CTime& time, int nDays);
	void AddDaysToTime(COleDateTime& time, int nDays);
};

/**********************************************************************/
