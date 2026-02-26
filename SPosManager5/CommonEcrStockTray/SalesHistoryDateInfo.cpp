/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
/**********************************************************************/
#include "SalesHistoryDateInfo.h"
/**********************************************************************/

CSalesHistoryDateInfo::CSalesHistoryDateInfo()
{
	m_Date = COleDateTime::GetCurrentTime();
	SetDateInfoForReports();
}

/**********************************************************************/

CSalesHistoryDateInfo::CSalesHistoryDateInfo( COleDateTime& oleDateTime )
{
	m_Date = oleDateTime;
	SetDateInfoForReports();
}

/**********************************************************************/

void CSalesHistoryDateInfo::SetDateInfoForReports()
{
	if ( m_Date.m_status == COleDateTime::valid )
	{
		int nOleWeekDay = m_Date.GetDayOfWeek();
		int nOleStartDay = EcrmanOptions.GetReportsWeekStartDay() + 1;

		if ( nOleWeekDay >= nOleStartDay )
			m_nWeekday = nOleWeekDay - nOleStartDay;
		else
			m_nWeekday = nOleWeekDay + 7 - nOleStartDay;

		m_DateWeek = m_Date - COleDateTimeSpan( m_nWeekday, 0, 0, 0 );
		m_DateMonth = COleDateTime ( m_Date.GetYear(), m_Date.GetMonth(), 1, 0, 0, 0 );
	}
}

/**********************************************************************/
