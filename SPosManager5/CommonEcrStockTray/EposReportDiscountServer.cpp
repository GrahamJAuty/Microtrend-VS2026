/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportDiscountServer.h"
/**********************************************************************/

CEposReportDiscountServer::CEposReportDiscountServer( CEposSelectArray& SelectArray ) : CEposReportDiscountBase( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_DISCOUNT;
	m_ReportSettings.SetOfferFilter( 0x1FF );
	m_ReportSettings.SetSubtotalItemsFlag( FALSE );
}

/**********************************************************************/

CEposReportDiscountServer::CEposReportDiscountServer( CEposSelectArray& SelectArray, const char* szCustomSettings ) : CEposReportDiscountBase( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_DISCOUNT;
	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine( strParams );
}

/**********************************************************************/

void CEposReportDiscountServer::InitialiseConsolidationBlocks()
{
	CEposReportDiscountBase::InitialiseConsolidationBlocksStageOne();
}

/**********************************************************************/

void CEposReportDiscountServer::SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo )
{
	m_nServerBlockIdxStart = 0;
	
	CEposReportConsolServerBlockMap info;
	info.m_nServerNo = nServerNo;

	switch( m_nServerReportType )
	{
	case SERVER_REPORT_CODE:
	case SERVER_REPORT_SYS:
		info.m_nServerLocOrDbSort = 0;
		info.m_nServerLocOrDbIdx = 0;
		break;

	case SERVER_REPORT_NAME:
		info.m_nServerLocOrDbSort = 0;
		info.m_nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( nLocIdx, nServerNo );
		break;

	case SERVER_REPORT_DB:
		info.m_nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx(nDbIdx );
		info.m_nServerLocOrDbIdx = nDbIdx;
		break;

	case SERVER_REPORT_LOC:
		info.m_nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx(nLocIdx );
		info.m_nServerLocOrDbIdx = nLocIdx;
		break;
	}

	int nPos;
	if ( m_ServerBlockMap.Find( info, nPos ) == TRUE )
	{
		m_ServerBlockMap.GetAt( nPos, info );
		m_nServerBlockIdxStart = info.m_nBlockIdxStart;
	}
	else
	{
		int nSize = m_arrayReportBlocks.GetSize();
		info.m_nBlockIdxStart = nSize;
		m_nServerBlockIdxStart = nSize;
		m_ServerBlockMap.InsertAt( nPos, info );
		InitialiseConsolidationBlocksStageTwo();		
	}
}

/**********************************************************************/

bool CEposReportDiscountServer::CreateReport()
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
	for ( int s = 0; s < m_ServerBlockMap.GetSize(); s++ )
	{
		CEposReportConsolServerBlockMap infoServer;
		m_ServerBlockMap.GetAt( s, infoServer );

		int nServerBlocks = 0;
		for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
		{
			int nIdx = m_BlockMap.GetSortArrayIdx(n);
			CEposReportDiscountBlock ReportBlock = m_arrayReportBlocks.GetAt( infoServer.m_nBlockIdxStart + nIdx );

			if ( ReportBlock.GotData() == FALSE )
				continue;

			if ( TRUE == ReportBlock.m_bAddToTotal )
			{
				nTotalBlocks++;
				nServerBlocks++;
			}

			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
			
			m_ReportFile.HandleNewPage();
		
			CString strBlock; 
			ReportBlock.GetHeaderText( strBlock, FALSE );
			
			CString strHeader = "";
			strHeader += GetServerHeader( infoServer.m_nServerLocOrDbIdx, infoServer.m_nServerNo );
			strHeader += ", ";
			strHeader += strBlock;

			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			
			if ( m_nAuditLevel == ReportBlock.m_nEntityType )
			{
				CreateDiscountSection( ReportBlock, FALSE );
				WriteUnderline();
			}

			CreateDiscountSection( ReportBlock, TRUE );
		}

		nServerBlocks = 0;
	}

	if ( nTotalBlocks >= 2 )
	{
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( "<..>Grand Total" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		WriteGrandTotals();
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/