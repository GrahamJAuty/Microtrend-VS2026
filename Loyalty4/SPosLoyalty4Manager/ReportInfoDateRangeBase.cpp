//$$******************************************************************
#include "ReportInfoDateRangeBase.h"
//$$******************************************************************

CReportInfoDateRangeBase::CReportInfoDateRangeBase()
{
	//date
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();
	//options
	m_bSummary = FALSE;
}

//$$******************************************************************

const char* CReportInfoDateRangeBase::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

const char* CReportInfoDateRangeBase::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

void CReportInfoDateRangeBase::SetDateFrom(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateFrom.SetDate(time);
}

//$$******************************************************************

void CReportInfoDateRangeBase::SetDateTo(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateTo.SetDate(time);
}

//$$******************************************************************

const char* CReportInfoDateRangeBase::GetDateRangeString()
{
	if (m_dateFrom != m_dateTo)
	{
		m_strDateRange.Format(" (%s - %s)", m_dateFrom.GetDate(), m_dateTo.GetDate());
	}
	else
	{
		m_strDateRange.Format(" (%s)", m_dateFrom.GetDate());
	}

	return m_strDateRange;
}

//$$******************************************************************

bool CReportInfoDateRangeBase::IsValidDate(const char* szDate)
{
	CSSDate dateAudit(szDate);
	return ((dateAudit >= m_dateFrom) && (dateAudit <= m_dateTo));
}

//$$******************************************************************
