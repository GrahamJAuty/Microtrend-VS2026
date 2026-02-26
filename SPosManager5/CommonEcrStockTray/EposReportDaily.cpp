/**********************************************************************/
 
/**********************************************************************/
#include "BusinessDateInfo.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportDaily.h"
/**********************************************************************/

CEposReportDaily::CEposReportDaily( CEposSelectArray& SelectArray ) : CEposReportTimed( SelectArray )
{
	m_nReportType = REPORT_TYPE_DAILY;
	m_strDateOrTimeColumn = "Date";
}

/**********************************************************************/

bool CEposReportDaily::GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel )
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

const char* CEposReportDaily::GetDateOrTimeReport( CString& strLabel )
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
