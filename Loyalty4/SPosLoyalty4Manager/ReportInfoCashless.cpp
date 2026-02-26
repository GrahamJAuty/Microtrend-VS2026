//$$******************************************************************
#include "ReportInfoCashless.h"
//$$******************************************************************

CReportInfoCashless::CReportInfoCashless( const char* szLabel, bool bLiabilityMode )
{
	m_strReportLabel = szLabel;
	
	//date
	m_dateFrom.SetCurrentDate();					
	m_dateTo.SetCurrentDate();
	m_datePrevious.SetCurrentDate();
	m_dateNext.SetCurrentDate();
	m_dateSearchFrom.SetCurrentDate();
	//time
	m_timeCutoff.SetTime( "00:00" );
	
	if (m_strReportLabel == "R124")
	{
		m_strReportTitle = "Cashless Liability Reconciliation";
	}
	else if (m_strReportLabel == "R125")
	{
		m_strReportTitle = "Cashless Posting";
	}

	m_bLiabilityMode = bLiabilityMode;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDatePreviousString()
{
	m_strDatePrevious = m_datePrevious.GetDate();
	return m_strDatePrevious;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDateNextString()
{
	m_strDateNext = m_dateNext.GetDate();
	return m_strDateNext;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDateSearchFromString()
{
	m_strDateSearchFrom = m_dateSearchFrom.GetDate();
	return m_strDateSearchFrom;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDateSearchToString()
{
	m_strDateSearchTo = m_dateSearchTo.GetDate();
	return m_strDateSearchTo	;
}

//$$******************************************************************

void CReportInfoCashless::SetDateFrom( CTime& date )
{
	SetCSSDateFromCTime(date, m_dateFrom);
}

//$$******************************************************************

void CReportInfoCashless::SetDateTo( CTime& date )
{
	SetCSSDateFromCTime(date, m_dateTo);
}

//$$******************************************************************

void CReportInfoCashless::SetDatePrevious( CTime& date )
{
	SetCSSDateFromCTime(date, m_datePrevious);
}

//$$******************************************************************

void CReportInfoCashless::SetDateNext(CTime& date)
{
	SetCSSDateFromCTime(date, m_dateNext);
}

//$$******************************************************************

void CReportInfoCashless::SetDateSearchFrom( CTime& date )
{
	SetCSSDateFromCTime(date, m_dateSearchFrom);
}

//$$******************************************************************

void CReportInfoCashless::SetDateSearchTo(CTime& date)
{
	SetCSSDateFromCTime(date, m_dateSearchTo);
}

//$$******************************************************************

void CReportInfoCashless::SetTimeCutoff(CTime& time)
{
	m_timeCutoff.SetTime(time);
}

//$$******************************************************************

void CReportInfoCashless::SetCSSDateFromCTime(CTime& source, CSSDate& dest)
{
	CTime sourceDate(source.GetYear(), source.GetMonth(), source.GetDay(), 0, 0, 0);
	dest.SetDate(sourceDate);
}

//$$******************************************************************

void CReportInfoCashless::SetRefundProcessType(int n)
{
	if ((n == 0) || (n == 1))
	{
		m_nRefundProcessType = n;
	}
}

//$$******************************************************************

const char* CReportInfoCashless::GetTimeCutoffString()
{
	m_strTimeCutoff = m_timeCutoff.GetTime();
	return m_strTimeCutoff;
}

//$$******************************************************************

const char* CReportInfoCashless::GetDateRangeString()
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

bool CReportInfoCashless::IsValidReportDate(const char* szDate, const char* szTime)
{
	CSSDate dateAudit(szDate);

	//NO CUT OFF TIME, SO USE ORDINARY DATE FILTER
	if (FALSE == m_bCutoffTime)
	{
		if (dateAudit < m_dateFrom)
		{
			return FALSE;
		}

		if (dateAudit > m_dateTo)
		{
			return FALSE;
		}

		return TRUE;
	}
	//CUT OFF TIME IS SET, SO USE CUT OFF TIME LOGIC
	else if (FALSE == m_bCutoffNextDay)
	{
		return IsValidReportDateWithCutoffTime(dateAudit, szTime, m_datePrevious, m_dateTo);
	}
	else
	{
		return IsValidReportDateWithCutoffTime(dateAudit, szTime, m_dateFrom, m_dateNext);
	}
}

//$$******************************************************************

bool CReportInfoCashless::IsValidReportDateWithCutoffTime(CSSDate& dateAudit, CString strTime, CSSDate& dateFrom, CSSDate& dateTo)
{
	//NO MATCH IF BEFORE FROM DATE
	if (dateAudit < dateFrom)
	{
		return FALSE;
	}

	//NO MATCH IF AFTER TO DATE
	if (dateAudit > dateTo)
	{
		return FALSE;
	}

	CSSTime timeAudit(strTime);

	//NO MATCH IF ON FROM DATE AND BEFORE CUT OFF TIME
	if ((dateAudit == dateFrom) && (timeAudit < m_timeCutoff))
	{
		return FALSE;
	}

	//NO MATCH IF ON TO DATE AND AFTER CUT OFF TIME
	if (dateAudit == dateTo)
	{
		if (timeAudit >= m_timeCutoff)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//$$******************************************************************

int CReportInfoCashless::IsBeforeReportDate(CAuditRecord* atcRecord)
{
	CString strDate = "";
	CString strTime = "";

	switch (atcRecord->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		strDate = atcRecord->GetDateLastUsed();
		strTime = atcRecord->GetTimeLastUsed();
		break;

	default:
		strDate = atcRecord->GetDateFiled();
		strTime = atcRecord->GetTimeFiled();
		break;
	}

	CSSDate dateAudit(strDate);
	CSSTime timeAudit(strTime);
	return IsBeforeReportDate(dateAudit, timeAudit);
}

//$$******************************************************************

int CReportInfoCashless::IsBeforeReportDate(CSSDate& dateAudit, CSSTime& timeAudit)
{
	if ((dateAudit.IsSet() == FALSE) || (timeAudit.IsSet() == FALSE))
	{
		return -1;
	}

	bool bUsePreviousDate = FALSE;
	if ((TRUE == m_bCutoffTime) && (FALSE == m_bCutoffNextDay))
	{
		bUsePreviousDate = TRUE;
	}

	if (FALSE == bUsePreviousDate)
	{
		if ( dateAudit < m_dateFrom )
		{
			return 0;
		}

		if ( (dateAudit == m_dateFrom) && ( TRUE == m_bCutoffTime ) && (timeAudit < m_timeCutoff) )
		{
			return 0;
		}

		return 1;
	}
	else
	{
		if ( dateAudit < m_datePrevious )
		{
			return 0;
		}

		if ( (dateAudit == m_datePrevious) && (timeAudit < m_timeCutoff) )
		{
			return 0;
		}

		return 1;
	}	
}

//$$******************************************************************

