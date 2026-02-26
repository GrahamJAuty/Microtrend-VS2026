/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "NodeTypes.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SalesHistoryReportStock.h"
/**********************************************************************/

const char* CSalesHistoryReportStock::GetRepMonth( COleDateTime date, bool bShortName, bool bFullLine )
{
	m_strRepMonth = ( bFullLine ) ? "<..>" : "";

	if ( bShortName == FALSE )
		m_strRepMonth += date.Format ( "%B %Y" );
	else
		m_strRepMonth += date.Format ( "%b %y" );

	return m_strRepMonth;
}

/**********************************************************************/

void CSalesHistoryReportStock::SetDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession )
{
	DataManagerNonDb.SessionDateTimeFilter.SetDates( dateFrom, dateTo, ReportSession );
}

/**********************************************************************/

const char* CSalesHistoryReportStock::GetReportTitle( const char* szReportName )
{
	CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
	CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();

	CString strDateFrom;
	strDateFrom.Format ( "%2.2d/%2.2d/%4.4d",
		DateFrom.m_oleDateTime.GetDay(),
		DateFrom.m_oleDateTime.GetMonth(),
		DateFrom.m_oleDateTime.GetYear() );

	CString strDateTo;
	strDateTo.Format ( "%2.2d/%2.2d/%4.4d",
		DateTo.m_oleDateTime.GetDay(),
		DateTo.m_oleDateTime.GetMonth(),
		DateTo.m_oleDateTime.GetYear() );

	m_strTitle.Format ( "%s (%s - %s, %s%s)",
		 szReportName,
		(const char*) strDateFrom,
		(const char*) strDateTo,
		(const char*) m_strSessionTitle,
		m_ReportFilters.CategoryFilter.GetFilterNameForReport() );
	
	return m_strTitle;
}

/**********************************************************************/

bool CSalesHistoryReportStock::MatchStockpoint ( int nSpIdx )
{
	if ( m_nConLevel == NODE_SYSTEM )
		return TRUE;

	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		return FALSE;

	int nDbNo = dbDatabase.GetDbNo( m_nDbIdx );
	if ( dbStockpoint.GetDbNo( nSpIdx ) != nDbNo )
		return FALSE;

	bool bMatch = FALSE;
	switch ( m_nConLevel )
	{
	case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == m_nTableNo );		break;
	case NODE_SITE:		bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == m_nTableNo );	break;
	case NODE_SYSTEM:		
	case NODE_DATABASE:		bMatch = TRUE;		break;
	default:				bMatch = FALSE;	break;
	}
	return bMatch;
}

/**********************************************************************/

void CSalesHistoryReportStock::ShowStockpoints()
{
	CSSFile fileReport;
	if ( fileReport.Open ( Super.ReportFilename(), "ab" ) == FALSE )
		return;

	fileReport.WriteLine ( "<..>Report includes stockpoints" );
	fileReport.WriteLine ( "<LI>" );

	CArray<int,int> arraySpIdx;
	
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		if ( MatchStockpoint ( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	//if ( EcrmanOptions.GetSortLocNameFlag() == TRUE )
		dbStockpoint.SortSpListByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);

		CString strLine = "<..>";
		strLine += dbStockpoint.GetName( nSpIdx );
		fileReport.WriteLine ( strLine );
	}
}

/**********************************************************************/
