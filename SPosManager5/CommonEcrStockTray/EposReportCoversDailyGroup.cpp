/**********************************************************************/
#include "BusinessDateInfo.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportCoversDailyGroup.h"
/**********************************************************************/

CEposReportCoversDailyGroup::CEposReportCoversDailyGroup( CEposSelectArray& SelectArray, bool bDeptMode ) : CEposReportCoversTimedGroup( SelectArray )
{
	m_bDeptMode = bDeptMode;
	m_nReportType = ( m_bDeptMode ) ? REPORT_TYPE_COVERS_DAILY_DEPT : REPORT_TYPE_COVERS_DAILY_GROUP;
	m_bIsCoversReport = TRUE;
	m_strDateOrTimeColumn = "Date";
}

/**********************************************************************/

bool CEposReportCoversDailyGroup::GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel )
{
	bool bResult = FALSE;

	CBusinessDateInfo BusinessDateInfo;
	if ( BusinessDateInfo.ValidateBusinessDate( nLocIdx, m_bEODMode, strThisSaleDate, strThisSaleTime ) == TRUE )
	{
		COleDateTime date = BusinessDateInfo.GetBusinessDate();
		strLabel.Format( "%4.4d%2.2d%2.2d", date.GetYear(), date.GetMonth(), date.GetDay() );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

const char* CEposReportCoversDailyGroup::GetDateOrTimeReport( CString& strLabel )
{
	m_strDateOrTimeReport = "";

	if ( strLabel.GetLength() == 8 )
	{
		m_strDateOrTimeReport.Format( "%s/%s/%s",
			(const char*) strLabel.Right(2),
			(const char*) strLabel.Mid(4,2),
			(const char*) strLabel.Left(4) );
	}

	return m_strDateOrTimeReport;
}

/**********************************************************************/
