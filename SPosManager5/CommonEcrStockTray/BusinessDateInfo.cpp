/**********************************************************************/
#include "BusinessDateInfo.h"
/**********************************************************************/

CBusinessDateInfo::CBusinessDateInfo()
{
	Reset();
}

/**********************************************************************/

void CBusinessDateInfo::Reset()
{
	m_bGotDate = FALSE;
	m_nTimeSortType = 0;
}

/**********************************************************************/

bool CBusinessDateInfo::ValidateBusinessDate( int nLocIdx, bool bEODMode, CString& strDate, CString& strTime )
{
	bool bResult = FALSE;
	m_nTimeSortType = 0;
	
	COleDateTime BusinessDateOld = m_BusinessDate;

	if ( FALSE == bEODMode )
	{
		bResult = DataManagerNonDb.SessionDateTimeFilter.ValidateBusinessDate( strDate, strTime, m_nTimeSortType, m_BusinessDate );
	}
	else
	{
		CSortedDateTimeItem itemDay;
		CString strDateTime = strDate + strTime;
		if ( DataManagerNonDb.EODDateTimeFilterArray.GetBusinessDay( nLocIdx, strDateTime, itemDay ) == TRUE )
		{
			m_nTimeSortType = 1;
			m_BusinessDate = itemDay.m_oleDateTime;
			bResult = TRUE;
		}
	}

	if ( TRUE == bResult )
	{
		if ( ( FALSE == m_bGotDate ) || ( m_BusinessDate != BusinessDateOld ) )
		{
			m_bGotDate = TRUE;
	
			int nOleWeekDay = m_BusinessDate.GetDayOfWeek();
			int nOleStartDay = EcrmanOptions.GetReportsWeekStartDay() + 1;

			if ( nOleWeekDay >= nOleStartDay )
				m_nBusinessWeekday = nOleWeekDay - nOleStartDay;
			else
				m_nBusinessWeekday = nOleWeekDay + 7 - nOleStartDay;

			m_nBusinessMonthday = m_BusinessDate.GetDay();
			m_BusinessDateWeek = m_BusinessDate - COleDateTimeSpan( m_nBusinessWeekday, 0, 0, 0 );
			m_BusinessDateMonth = COleDateTime ( m_BusinessDate.GetYear(), m_BusinessDate.GetMonth(), 1, 0, 0, 0 );
		}
	}
	else
	{
		m_bGotDate = FALSE;
	}

	return bResult;
}

/**********************************************************************/
