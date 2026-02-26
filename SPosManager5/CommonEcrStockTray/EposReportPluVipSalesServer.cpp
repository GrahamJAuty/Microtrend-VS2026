/**********************************************************************/
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportPluVipSalesServer.h"
/**********************************************************************/

CEposReportPluVipSalesServer::CEposReportPluVipSalesServer( CEposSelectArray& SelectArray, int nInitialReportType ) : CEposReportPluVipSalesBase( SelectArray )
{
	switch( nInitialReportType )
	{
	case REPORT_TYPE_VIPSALES_TRANSERVER:
		m_nReportType = nInitialReportType;
		m_bAllowValue = EcrmanOptions.GetReportsVIPRetailFlag();
		m_bDiscountDetail = m_bAllowValue && EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = m_bAllowValue && EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = TRUE;
		break;

	case REPORT_TYPE_PLUWASTE_TRANSERVER:
		m_nReportType = nInitialReportType;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = FALSE;
		m_bPremiumDetail = FALSE;
		m_bVIPReport = FALSE;	
		break;

	case REPORT_TYPE_COVERS_PLU_TRANSERVER:
		m_nReportType = nInitialReportType;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;	
		break;

	case REPORT_TYPE_PLUSALES_TRANSERVER:
	default:
		m_nReportType = REPORT_TYPE_PLUSALES_TRANSERVER;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;	
		break;
	}
	
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

void CEposReportPluVipSalesServer::InitialiseConsolidationBlocks()
{
	CEposReportPluVipSalesBase::InitialiseConsolidationBlocksStageOne();
}

/**********************************************************************/

void CEposReportPluVipSalesServer::SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo )
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

bool CEposReportPluVipSalesServer::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	AddReportColumn ( "Plu Number", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Description", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Dept", TA_RIGHT, 200, FALSE );
	AddReportColumn ( "Quantity", TA_RIGHT, 250, TRUE );

	if ( TRUE == m_bAllowValue )
	{
		if ( IsWastageReport() == TRUE )
			AddReportColumn ( "Waste", TA_RIGHT, 250, TRUE );
		else
		{
			AddReportColumn ( "Sales", TA_RIGHT, 250, TRUE );

			if ( TRUE == m_bDiscountDetail )
			{
				AddReportColumn ( "Discount", TA_RIGHT, 250, TRUE );

				if (TRUE == m_bPremiumDetail)
				{
					AddReportColumn("Premium", TA_RIGHT, 250, TRUE);
				}

				AddReportColumn ( "Retail", TA_RIGHT, 250, TRUE );
			}
		}

		AddReportColumn ( "Average", TA_RIGHT, 250, TRUE );
		AddReportColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 200, FALSE );
		AddReportColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250, TRUE );

		if ( TRUE == m_bNetOfTax )
			AddReportColumn ( "Est. Net", TA_RIGHT, 250, TRUE );
	}

	AddReportColumn ( "Est. Cost", TA_RIGHT, 250, TRUE );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPluSalesBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
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

		int nServerCovers = 0;
		int nServerBlocks = 0;
		for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
		{
			int nIdx = m_BlockMap.GetSortArrayIdx(n);
			CEposReportPluSalesBlock ReportBlock = m_arrayReportBlocks.GetAt( infoServer.m_nBlockIdxStart + nIdx );

			if ( ReportBlock.GotData() == FALSE )
				continue;

			if ( TRUE == ReportBlock.m_bAddToTotal )
			{
				nTotalBlocks++;
				nServerBlocks++;
				nServerCovers += ReportBlock.m_nCovers;
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
			
			CreateSalesSection( ReportBlock );
			WriteBlockTotals( ReportBlock );
		}

		if ( nServerBlocks >= 2 )
		{
			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( "<..>Server Total" );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			WriteConsolidatedTotals( m_arrayTotals1, "Server", nServerCovers, TRUE );
		}
		
		m_arrayTotals2.Consolidate( m_arrayTotals1 );
		m_arrayTotals1.Clear();

		nServerBlocks = 0;
	}

	if ( nTotalBlocks >= 2 )
	{
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( "<..>Grand Total" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		WriteConsolidatedTotals( m_arrayTotals2, "Grand", m_nTotalCovers, TRUE );
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/