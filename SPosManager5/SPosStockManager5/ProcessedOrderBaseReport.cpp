/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "NodeTypes.h"
#include "StockpointCSVArray.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "ProcessedOrderBaseReport.h"
/**********************************************************************/

CProcessedOrderBaseReport::CProcessedOrderBaseReport()
{
	m_nReportType = ORDER_ONORDER_SUPPLIER;
	m_nConLevel = NODE_STOCKPOINT;
	m_nTableNo = 1;
	m_strConLevel = "";
	m_nDbIdx = 0;
	m_strDateFrom = "";
	m_strDateTo = "";
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";
	m_strReference = "";
}

/**********************************************************************/
CString CProcessedOrderBaseReport::m_strReportParamsFilename;
/**********************************************************************/

const char* CProcessedOrderBaseReport::GetReportParamsFilename ( int nRepType )
{
	CFilenameUpdater FnUp ( SysFiles::OrderBasePrm, 0, nRepType );
	m_strReportParamsFilename = FnUp.GetFilenameToUse();
	return m_strReportParamsFilename;
}

/**********************************************************************/

void CProcessedOrderBaseReport::ShowStockpoints( CReportFile& ReportFile )
{
	ReportFile.WriteLine ( "<..>Report includes stockpoints" );
	ReportFile.WriteLine ( "<LI>" );

	CArray<int,int> arraySpIdx;
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		if ( MatchStockpoint ( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	dbStockpoint.SortSpListByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);
		CString strLine = "<..>";
		strLine += dbStockpoint.GetName( nSpIdx );
		ReportFile.WriteLine ( strLine );
	}
}

/**********************************************************************/

bool CProcessedOrderBaseReport::MatchStockpoint ( int nSpIdx )
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

const char* CProcessedOrderBaseReport::GetReportTitle()
{
	CStockReportNames ReportNames;

	m_strReportTitle.Format ( "%s report for %s", 
		ReportNames.GetComboName ( m_nReportType ),
		( const char* ) m_strConLevel );
	
	if ( m_strDateFrom != "" )
	{
		CString strDateRange;
		strDateRange.Format ( " (%s - %s)",
				(const char*) m_strDateFrom,
				(const char*) m_strDateTo );
		m_strReportTitle += strDateRange;
	}

	return m_strReportTitle;
}

/*********************************************************************/

void CProcessedOrderBaseReport::SetSupplierArray ( CWordArray& wSuppIdxArray )
{
	m_wSuppIdxArray.RemoveAll();
	for ( int nIndex = 0; nIndex < wSuppIdxArray.GetSize(); nIndex++ )
		m_wSuppIdxArray.Add ( wSuppIdxArray.GetAt ( nIndex ) );
}

/*********************************************************************/

