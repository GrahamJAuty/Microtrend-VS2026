/**********************************************************************/
#include "ReportTypes.h"
#include "SalesHistoryFields.h"
/**********************************************************************/
#include "EposReportHourly.h"
/**********************************************************************/

CEposReportHourly::CEposReportHourly( CEposSelectArray& SelectArray ) : CEposReportTimed( SelectArray )
{
	Init();
	m_nTimeSliceType = SH_TIMESLICE_60MIN;
}

/**********************************************************************/

CEposReportHourly::CEposReportHourly( CEposSelectArray& SelectArray, const char* szCustomSettings ) : CEposReportTimed( SelectArray )
{
	Init();
	LoadTimeSliceSettings( szCustomSettings, m_ReportSettings, m_nTimeSliceType, m_TimeMap );
}

/**********************************************************************/

void CEposReportHourly::Init()
{
	m_nReportType = REPORT_TYPE_HOURLY;
	m_strDateOrTimeColumn = "Time";

	m_strSODLabel.Format ( "%2.2d%2.2d", 
		EcrmanOptions.GetReportsDayStartHour(),
		EcrmanOptions.GetReportsDayStartMinute() );
}

/**********************************************************************/

bool CEposReportHourly::GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel )
{
	int nDayShift = 0;
	CString strTimeCode = "";
	GetTimeSliceTimeCode( strThisSaleTime, m_strSODLabel, m_nTimeSliceType, m_TimeMap, strTimeCode, nDayShift );
	strLabel = ( 1 == nDayShift ) ? "1" : "0"; 
	strLabel += strTimeCode;
	return TRUE;
}

/**********************************************************************/

const char* CEposReportHourly::GetDateOrTimeReport( CString& strLabel )
{
	m_strDateOrTimeReport = "";

	int nLen = strLabel.GetLength() - 1;

	if ( nLen >= 1 )
		m_strDateOrTimeReport = GetTimeSliceString( m_nTimeSliceType, m_TimeMap, strLabel.Right( nLen ) );

	return m_strDateOrTimeReport;
}

/**********************************************************************/
