/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "DateRangeHelpers.h"
//#include "DealerFlags.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportStock.h"
/**********************************************************************/

int CSalesHistoryReportStock::CreateGraph()
{
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );

	int nResult = 0;

	StatusProgress.Lock( TRUE, "Creating graph" );
	switch ( m_nReportType )
	{
	case SH_STK_CAT:		nResult = CreateCategoryGraphInternal();	break;
	case SH_STK_DAILY:		nResult = CreateDailyGraphInternal();		break;
	case SH_STK_WEEKLY:		nResult = CreateWeeklyGraphInternal();		break;
	case SH_STK_MONTHLY:	nResult = CreateMonthlyGraphInternal();		break;
	}
	StatusProgress.Unlock();
	
	return nResult;
}

/**********************************************************************/

int CSalesHistoryReportStock::CreateCategoryGraphInternal()
{
	//MAKE SURE WE HAVE SOME DATA
	if ( m_QtyValueTable.GetBranchSize(0) == 0 )
		return 2;

	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return 1;
	
	CCSV csvOut;
	csvOut.Add ( "Category" );
	csvOut.Add ( "Sales Value" );
	csvOut.Add ( GetReportTitle( "Category Sales" ) );
	csvOut.Add ( m_strConLevelTitleForGraph );
	fileGraph.WriteLine ( csvOut.GetLine() );

	//EXTRACT THE CATEGORY TOTALS
	bool bGotData = FALSE;
	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		int nCategory = m_QtyValueTable.GetLabelInt( 0, nNodeIdx1 );
		
		CEntityTreeStockSalesHistoryNormalNode SalesNode;
		m_QtyValueTable.GetNode( 0, nNodeIdx1, SalesNode );
		double dValue = SalesNode.GetTotalVal();
		
		if ( dValue == 0.0 )
			continue;
			
		CCSV csv ( ',', '"', TRUE, FALSE, FALSE );
					
		CString strNum;
		strNum.Format ( "%3.3d", nCategory );
			
		csv.Add ( strNum );
		csv.Add ( dValue, SysInfo.GetDPValue() );
		csv.Add ( DataManager.Category.GetText ( nCategory ) );

		fileGraph.WriteLine ( csv.GetLine() );
		bGotData = TRUE;
	}
	return ( bGotData == TRUE ) ? 0 : 2;
}

/**********************************************************************/

int CSalesHistoryReportStock::CreateDailyGraphInternal()
{
	//MAKE SURE WE HAVE SOME DATA
	if ( m_QtyValueTable.GetBranchSize(0) == 0 )
		return 2;

	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return 1;
	
	CCSV csvOut;
	csvOut.Add ( "Date" );
	csvOut.Add ( "Sales Value" );
	csvOut.Add ( GetReportTitle ( "Daily Sales" ) );
	csvOut.Add ( m_strConLevelTitleForGraph );
	fileGraph.WriteLine ( csvOut.GetLine() );

	bool bGotData = FALSE;
	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		COleDateTime date = m_QtyValueTable.GetLabelDate( 0, nNodeIdx1 );
		
		CEntityTreeStockSalesHistoryNormalNode SalesNode;
		m_QtyValueTable.GetNode( 0, nNodeIdx1, SalesNode );
		double dValue = SalesNode.GetTotalVal();
		
		CCSV csv ( ',', '"', TRUE, FALSE, FALSE );

		CString strDay = date.Format ( "%d/%m/%Y" );	
		csv.Add ( strDay );			
		csv.Add ( dValue, SysInfo.GetDPValue() );
		fileGraph.WriteLine ( csv.GetLine() );	
		bGotData = TRUE;
	}
	return ( bGotData == TRUE ) ? 0 : 2; 
}

/**********************************************************************/

int CSalesHistoryReportStock::CreateWeeklyGraphInternal()
{
	//MAKE SURE WE HAVE SOME DATA
	if ( m_QtyValueTable.GetBranchSize(0) == 0 )
		return 2;

	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return 1;
	
	CCSV csvOut;

	if ( DealerFlags.GetWeekNoGraphFlag() == FALSE )
		csvOut.Add ( "Week Starting" );
	else
		csvOut.Add ( "Week No" );

	csvOut.Add ( "Sales Value" );
	csvOut.Add ( GetReportTitle ( "Weekly Sales" ) );
	csvOut.Add ( m_strConLevelTitleForGraph );
	fileGraph.WriteLine ( csvOut.GetLine() );

	bool bGotData = FALSE;
	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		COleDateTime date = m_QtyValueTable.GetLabelDate( 0, nNodeIdx1 );
		
		CEntityTreeStockSalesHistoryNormalNode SalesNode;
		m_QtyValueTable.GetNode( 0, nNodeIdx1, SalesNode );
		double dValue = SalesNode.GetTotalVal();
		
		CCSV csv ( ',', '"', TRUE, FALSE, FALSE );

		CString strWeek = "";
		if ( DealerFlags.GetWeekNoGraphFlag() == FALSE )
			strWeek = date.Format ( "%d/%m/%Y" );
		else
		{
			CDateRangeHelpers DateRangeHelpers;
			strWeek.Format ( "%d", DateRangeHelpers.GetWeekNo( date ) ); 
		}

		csv.Add ( strWeek );			
		csv.Add ( dValue, SysInfo.GetDPValue() );
		fileGraph.WriteLine ( csv.GetLine() );	
		bGotData = TRUE;
	}
	return ( bGotData == TRUE ) ? 0 : 2; 	
}

/**********************************************************************/

int CSalesHistoryReportStock::CreateMonthlyGraphInternal()
{
	//MAKE SURE WE HAVE SOME DATA
	if ( m_QtyValueTable.GetBranchSize(0) == 0 )
		return 2;

	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return 1;
	
	CCSV csvOut;
	csvOut.Add ( "Month" );
	csvOut.Add ( "Sales Value" );
	csvOut.Add ( GetReportTitle ( "Monthly Sales" ) );
	csvOut.Add ( m_strConLevelTitleForGraph );
	fileGraph.WriteLine ( csvOut.GetLine() );

	bool bGotData = FALSE;
	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		CString strMonth = GetRepMonth ( m_QtyValueTable.GetLabelDate( 0, nNodeIdx1 ), TRUE, FALSE );
		
		CEntityTreeStockSalesHistoryNormalNode SalesNode;
		m_QtyValueTable.GetNode( 0, nNodeIdx1, SalesNode );
		double dValue = SalesNode.GetTotalVal();
		
		CCSV csv ( ',', '"', TRUE, FALSE, FALSE );	
		csv.Add ( strMonth );			
		csv.Add ( dValue, SysInfo.GetDPValue() );
		fileGraph.WriteLine ( csv.GetLine() );	
		bGotData = TRUE;
	}
	return ( bGotData == TRUE ) ? 0 : 2; 	
}

/**********************************************************************/


