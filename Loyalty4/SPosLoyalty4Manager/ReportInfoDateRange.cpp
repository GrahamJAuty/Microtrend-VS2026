//$$******************************************************************
#include "ReportInfoDateRange.h"
//$$******************************************************************

CReportInfoDateRange::CReportInfoDateRange( const char* szLabel, const char* szTitle ) : CReportInfoDateRangeBase()
{
	m_strReportLabel = szLabel;
	m_strReportTitle = szTitle;
	m_strSummaryText = "Summary Report";

	//time
	m_bTimeRange = FALSE;
	m_timeFrom.SetTime( "00:00" );
	m_timeTo.SetTime( "23:59" );
	//options
	m_bSummaryAllowed = TRUE;
	m_bTimeAllowed = TRUE;

	if ( m_strReportLabel == "R121" )			// Visit Totaliser
	{
		m_bTimeAllowed = FALSE;
		m_strSummaryText = "Rank Entries";
	}
}

//$$******************************************************************

void CReportInfoDateRange::SetTimeFrom( CTime& time )
{
	m_timeFrom.SetTime( time );
}

//$$******************************************************************

void CReportInfoDateRange::SetTimeTo( CTime& time )
{
	m_timeTo.SetTime( time );
}

//$$******************************************************************

const char* CReportInfoDateRange::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

const char* CReportInfoDateRange::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

bool CReportInfoDateRange::IsValid ( const char* szDate, const char* szTime )
{
	bool bResult = FALSE;
	if (CReportInfoDateRangeBase::IsValidDate(szDate) == TRUE)
	{
		bResult = TRUE;

		if (TRUE == m_bTimeRange)
		{
			CSSTime timeAudit(szTime);
			if (timeAudit.IsSet() == TRUE)
			{
				bResult = ((timeAudit >= m_timeFrom) && (timeAudit <= m_timeTo));
			}
		}
	}
	return bResult;
}

//$$******************************************************************


