/**********************************************************************/
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportPaymentServer.h"
/**********************************************************************/

CEposReportPaymentServer::CEposReportPaymentServer( CEposSelectArray& SelectArray ) : CEposReportPaymentBase( SelectArray )
{
	m_nReportType = REPORT_TYPE_PAYMENT_SERVER;
}

/**********************************************************************/

void CEposReportPaymentServer::InitialiseConsolidationBlocks()
{
	CEposReportPaymentBase::InitialiseConsolidationBlocksStageOne();
}

/**********************************************************************/

void CEposReportPaymentServer::SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo )
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
		info.m_nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
		info.m_nServerLocOrDbIdx = nDbIdx;
		break;

	case SERVER_REPORT_LOC:
		info.m_nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( nLocIdx );
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

bool CEposReportPaymentServer::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 120 );
	m_ReportFile.AddColumn ( "Name", TA_LEFT, 350 );

	if ( EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE )
		m_ReportFile.AddColumn ( "Qty", TA_RIGHT, 200 );
	
	m_ReportFile.AddColumn ( "Tender", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Gratuity", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Cashback", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Surplus", TA_RIGHT, 200 );

	if ( EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE )
		m_ReportFile.AddColumn ( "Change", TA_RIGHT, 200 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.GetLineCount();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int s = 0; s < m_ServerBlockMap.GetSize(); s++ )
	{
		CEposReportConsolServerBlockMap infoServer;
		m_ServerBlockMap.GetAt( s, infoServer );

		for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
		{
			int nIdx = m_BlockMap.GetSortArrayIdx(n);
			CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( infoServer.m_nBlockIdxStart + nIdx );

			if ( ( ReportBlock.GotData() == FALSE ) && ( NODE_SYSTEM != ReportBlock.m_nEntityType ) )
				continue;

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
			m_ReportFile.LockBlankLines(1);

			CreateSalesSection( ReportBlock );		
		}
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

