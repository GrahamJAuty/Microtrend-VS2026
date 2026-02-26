/**********************************************************************/
//#include "DataManager.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportDiscountSummary.h"
/**********************************************************************/

CEposReportDiscountSummary::CEposReportDiscountSummary( CEposSelectArray& SelectArray, const char* szCustomSettings ) : CEposReportDiscountBase( SelectArray )
{
	m_nServerBlockIdxStart = 0;
	m_nReportType = REPORT_TYPE_NOSERVER_DISCOUNT;
	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine( strParams );
}

/**********************************************************************/

void CEposReportDiscountSummary::InitialiseConsolidationBlocks()
{
	CEposReportDiscountBase::InitialiseConsolidationBlocksStageOne();
	CEposReportDiscountBase::InitialiseConsolidationBlocksStageTwo();
}

/**********************************************************************/

bool CEposReportDiscountSummary::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "Promo", TA_LEFT, 600 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Item", TA_LEFT, 600 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportDiscountBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pDiscount -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	int nTotalBlocks = 0;
	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportDiscountBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		if ( TRUE == ReportBlock.m_bAddToTotal )
			nTotalBlocks++;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();
	
		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
	
		if ( m_nAuditLevel == ReportBlock.m_nEntityType )
		{
			CreateDiscountSection( ReportBlock, FALSE );
			WriteUnderline();
		}

		CreateDiscountSection( ReportBlock, TRUE );
	}

	StatusProgress.Unlock();

	if ( nTotalBlocks >= 2 )
	{
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( "<..>Grand Total" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		WriteGrandTotals();
	}
	
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/