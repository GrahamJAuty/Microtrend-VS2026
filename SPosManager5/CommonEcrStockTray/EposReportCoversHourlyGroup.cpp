/**********************************************************************/
#include "ReportTypes.h"
#include "SalesHistoryFields.h"
/**********************************************************************/
#include "EposReportCoversHourlyGroup.h"
/**********************************************************************/

CEposReportCoversHourlyGroup::CEposReportCoversHourlyGroup( CEposSelectArray& SelectArray, bool bDeptMode ) : CEposReportCoversTimedGroup( SelectArray )
{
	Init( bDeptMode );
	m_nTimeSliceType = SH_TIMESLICE_60MIN;
}

/**********************************************************************/

CEposReportCoversHourlyGroup::CEposReportCoversHourlyGroup( CEposSelectArray& SelectArray, const char* szCustomSettings, bool bDeptMode ) : CEposReportCoversTimedGroup( SelectArray )
{
	Init( bDeptMode );
	LoadTimeSliceSettings( szCustomSettings, m_ReportSettings, m_nTimeSliceType, m_TimeMap );
}

/**********************************************************************/

void CEposReportCoversHourlyGroup::Init( bool bDeptMode )
{
	m_bDeptMode = bDeptMode;
	m_nReportType = ( m_bDeptMode ) ? REPORT_TYPE_COVERS_HOURLY_DEPT : REPORT_TYPE_COVERS_HOURLY_GROUP;
	m_bIsCoversReport = TRUE;
	m_strDateOrTimeColumn = "Time";

	m_strSODLabel.Format ( "%2.2d%2.2d", 
		EcrmanOptions.GetReportsDayStartHour(),
		EcrmanOptions.GetReportsDayStartMinute() );
}

/**********************************************************************/

bool CEposReportCoversHourlyGroup::GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel )
{
	int nDayShift = 0;
	CString strTimeCode = "";
	GetTimeSliceTimeCode( strThisSaleTime, m_strSODLabel, m_nTimeSliceType, m_TimeMap, strTimeCode, nDayShift );
	strLabel = ( 1 == nDayShift ) ? "1" : "0"; 
	strLabel += strTimeCode;
	return TRUE;
}

/**********************************************************************/

const char* CEposReportCoversHourlyGroup::GetDateOrTimeReport( CString& strLabel )
{
	m_strDateOrTimeReport = "";

	int nLen = strLabel.GetLength() - 1;

	if ( nLen >= 1 )
		m_strDateOrTimeReport = GetTimeSliceString( m_nTimeSliceType, m_TimeMap, strLabel.Right( nLen ) );

	return m_strDateOrTimeReport;
}

/**********************************************************************/
