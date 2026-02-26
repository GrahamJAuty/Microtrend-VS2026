/**********************************************************************/
#pragma once
/**********************************************************************/

struct CSalesHistoryDateInfo
{
public:
	CSalesHistoryDateInfo();
	CSalesHistoryDateInfo( COleDateTime& oleDateTime );
	void SetDateInfoForReports();

public:
	COleDateTime m_Date;
	COleDateTime m_DateWeek;
	COleDateTime m_DateMonth;
	int m_nWeekday;
};

/**********************************************************************/