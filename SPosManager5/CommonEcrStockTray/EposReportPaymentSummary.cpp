/**********************************************************************/
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPaymentSummary.h"
/**********************************************************************/

CEposReportPaymentSummary::CEposReportPaymentSummary( CEposSelectArray& SelectArray ) : CEposReportPaymentBase( SelectArray )
{
	m_nReportType = REPORT_TYPE_PAYMENT_SUMMARY;
}

/**********************************************************************/

void CEposReportPaymentSummary::InitialiseConsolidationBlocks()
{
	CEposReportPaymentBase::InitialiseConsolidationBlocksStageOne();
	CEposReportPaymentBase::InitialiseConsolidationBlocksStageTwo();
}

/**********************************************************************/

bool CEposReportPaymentSummary::CheckGraph()
{
	if ( ( EcrmanOptions.GetGlobalPaymentFlag() == FALSE ) && ( NODE_SYSTEM == m_nConLevel ) )
	{
		Prompter.Error( "You cannot create a graph of this report across multiple databases\n\nwhen payment methods are database specific." );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportPaymentSummary::CreateReport()
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
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ( ReportBlock.GotData() == FALSE ) && ( NODE_SYSTEM != ReportBlock.m_nEntityType ) )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines(1);

		CreateSalesSection( ReportBlock );		
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

bool CEposReportPaymentSummary::CreateGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open( Super.ReportFilename(), "wb" ) == FALSE )
		return FALSE;

	CCSV csvOut;
	csvOut.Add( "Payment Type" );
	csvOut.Add( "Sales Value" );
	csvOut.Add( GetReportNameInternal( m_nReportType ) );
	csvOut.Add( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );
	
	StatusProgress.Lock( TRUE, "Creating graph" );

	CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( 0 );

	int nSize = ReportBlock.m_pNet -> GetSize();

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolPaySumOneVal ConsolInfo;
		ReportBlock.m_pNet -> GetAt( i, ConsolInfo );

		CCSV csvOut;
		csvOut.Add ( DataManager.Payment.GetDisplayName( ConsolInfo.m_nSubType ) );
		csvOut.Add ( ConsolInfo.GetReportValue(), SysInfo.GetDPValue() );
		fileGraph.WriteLine( csvOut.GetLine() );
	}
	StatusProgress.Unlock();
	return TRUE;
}

/**********************************************************************/


