/**********************************************************************/
#include "Consolidation.h"
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "DateRangeHelpers.h"
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportPlu.h"
/**********************************************************************/

bool CSalesHistoryReportPlu::CreateTaxReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle( m_strReportName ) );
	m_ReportFile.AddColumn ( "", TA_LEFT, 5 );
	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Sales", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Average", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250 );

	m_nProgress = 0;
	m_nLabelTaxBand = -1;
	
	CEntityTreePluSalesHistoryNormalNode SalesNode;
	m_QtyValueTable.GetGrandTotalNode( SalesNode );

	double dQty = SalesNode.m_SalesBlock.GetBaseQty();
	double dVal = SalesNode.m_SalesBlock.GetTotalVal();
	double dTax = SalesNode.m_SalesBlock.GetTaxAmount();
	double dDiscountVal = SalesNode.m_SalesBlock.GetDiscountVal();
	double dPremiumVal = SalesNode.m_SalesBlock.GetPremiumVal();
	double dNet = dVal - dDiscountVal + dPremiumVal;

	CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
	csv.Add ( "" );
	csv.Add ( dQty, SysInfo.GetDPQty() );
	csv.Add ( dVal - dDiscountVal + dPremiumVal, SysInfo.GetDPValue() );

	if (dQty != 0.0)
	{
		csv.Add(dNet / dQty, SysInfo.GetDPValue());
	}
	else
	{
		csv.Add(0.0, SysInfo.GetDPValue());
	}

	csv.Add ( dTax, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	m_ReportFile.RequestBlankLines(1);
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportPlu::GetSoldItemsList( int nDbIdx, COleDateTime dateFrom, CByteArray& FlagArray )
{
	m_bEposSales = TRUE;
	m_bManualSales = TRUE;
	m_bImportSales = TRUE;

	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );

	m_nConLevel = NODE_DATABASE;
	m_SelectArray.SelectDatabase( nDbIdx );
#ifdef STOCKMAN_SYSTEM
	m_SelectArray.SelectDatabaseForNonEposSales( nDbIdx, TRUE, TRUE );
#endif
	m_SelectArray.MakeList();

	m_nReportType = SH_CUSTOM;

	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	COleDateTime timeStart ( 2003, 1, 1, 0, 0, 0 );
	COleDateTime timeEnd ( 2003, 1, 1, 23, 59, 59 );

	CTimeSliceMapCSVRecord TimeSlice;

	CSessionCSVRecord ReportSession;
	SetReportDates( dateFrom, dateNow, ReportSession, TimeSlice );

	m_arrayReportFields.RemoveAll();
	m_arrayReportFields.Add( SH_FIELD_PLUNO );
		
	m_arrayBufferedLines.RemoveAll();
	m_arraySalesChart.RemoveAll();
	m_arrayUnsoldItems.RemoveAll();
	
	PrepareFields();
	Consolidate();
		
	FlagArray.RemoveAll();

	if ( DataManager.Plu.GetSize() > 0 )
		FlagArray.InsertAt( 0, 0, DataManager.Plu.GetSize() );

	for ( int n = 0; n < m_QtyValueTable.GetBranchSize(0); n++ )
	{
		int nPluIdx;
		__int64 nPluNo = m_QtyValueTable.GetLabelInt64( 0, n );
		if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			if ( nPluIdx < FlagArray.GetSize() )
				FlagArray.SetAt( nPluIdx, TRUE );
	}
}

/**********************************************************************/

