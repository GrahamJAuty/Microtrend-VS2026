/**********************************************************************/
#pragma once
/**********************************************************************/

struct CBusinessDateInfo
{
public:
	CBusinessDateInfo();
	void Reset();

public:
	bool ValidateBusinessDate( int nLocIdx, bool bEODMode, CString& strDate, CString& strTime );
	
public:
	bool GotDate(){ return m_bGotDate; }
	int GetTimeSortType(){ return m_nTimeSortType; }
	COleDateTime GetBusinessDate(){ return m_BusinessDate; }
	COleDateTime GetBusinessDateWeek(){ return m_BusinessDateWeek; }
	COleDateTime GetBusinessDateMonth(){ return m_BusinessDateMonth; }
	int GetWeekday(){ return m_nBusinessWeekday; }
	int GetMonthday(){ return m_nBusinessMonthday; }

private:
	bool m_bGotDate;
	int m_nTimeSortType;
	COleDateTime m_BusinessDate;
	COleDateTime m_BusinessDateWeek;
	COleDateTime m_BusinessDateMonth;
	int m_nBusinessMonthday;
	int m_nBusinessWeekday;
};

/**********************************************************************/