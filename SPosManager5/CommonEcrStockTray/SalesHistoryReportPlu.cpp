/**********************************************************************/
#include "DateRangeHelpers.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PluSalesHistoryFileArray.h"
#include "PMSReportFileFinder.h"
#include "PriceHelpers.h"
#include "ReportCostFinder.h"
#include "ReportHelpers.h"
#include "ReportPointCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportPlu.h"
/**********************************************************************/

CSalesHistoryReportPluConsolidationOptions::CSalesHistoryReportPluConsolidationOptions()
{
	m_bWantTranServer = FALSE;
	m_bWantItemServer = FALSE;
	m_bWantCustomer = FALSE;
	m_bIsUnsoldItemReport = FALSE;
	m_bWantDeptGrpSale = FALSE;
	m_bWantDeptGrpItem = FALSE;
	m_bWantPluInfo = FALSE;
	m_bWantRepGrp = FALSE;
	m_bWantConGrp = FALSE;
	m_bWantTable = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSalesHistoryReportPluColumn::CSalesHistoryReportPluColumn()
{
	m_strText = "";
	m_nAlign = TA_LEFT;
	m_nWidth = 200;
	m_nTotalType = TOTAL_TYPE_NONE;
	m_nBlock = 1;
}

/**********************************************************************/

CSalesHistoryReportPluColumn::CSalesHistoryReportPluColumn( const char* szText, int nAlign, int nWidth, int nTotalType, int nBlock )
{
	m_strText = szText;
	m_nAlign = nAlign;
	m_nWidth = nWidth;
	m_nTotalType = nTotalType;
	m_nBlock = nBlock;
}

/**********************************************************************/
/**********************************************************************/

CSalesHistoryReportPlu::CSalesHistoryReportPlu() : m_csvUnderline ( '\t', '"', TRUE, FALSE, FALSE )
{
	m_strWeekDays[0] = "Sun";
	m_strWeekDays[1] = "Mon";
	m_strWeekDays[2] = "Tue";
	m_strWeekDays[3] = "Wed";
	m_strWeekDays[4] = "Thu";
	m_strWeekDays[5] = "Fri";
	m_strWeekDays[6] = "Sat";
	
	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	m_bDepartmentByItem = FALSE;
	m_ReportFilters.PluFilter.SetMasterFilterNo(0);
	m_bSaleTime = FALSE;
	m_nTaxFilter = TAXBAND_FILTER_ALL;
	m_nPriceLevelFilter = PRICELEVEL_FILTER_ALL;
	m_nModifierFilter = MODIFIER_FILTER_ALL;

	m_bEposSales = TRUE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;
	m_bGuestToTerminal = FALSE;
	m_bWantTableName = FALSE;

	m_nConLevel = NODE_SYSTEM;
	m_nTableNo = 0;
	m_nDbIdx = 0;

	m_bEODMode = FALSE;
	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);
	
	m_bEstimatedProfit = FALSE;
	m_bPercentSale = FALSE;
	m_nPercentDepth = 0;
	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;
	m_bCustomer = FALSE;
	m_bVIPSales = FALSE;

	m_bHasModifierField = FALSE;

	m_nTermDateType = 0;
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void CSalesHistoryReportPlu::Reset( CSalesHistoryCSVRecordPlu& ReportRecord, CLocationSelectorEntity& LocSelEntity, bool bEposSales, bool bManualSales, bool bImportSales, bool bGuestToTerminal )
{
	int nReportType = ReportRecord.GetReportType();
	if ( nReportType == SH_CUSTOM )
		ReportRecord.GetReportFields( m_arrayReportFields );
	else
		m_arrayReportFields.RemoveAll();

	m_nReportType = ReportRecord.GetReportType();
	m_nReportMode = ReportRecord.GetReportMode();
	m_strReportName = ReportRecord.GetReportName();

	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	m_ReportFilters.PluFilter.SetMasterFilterNo( 0 );
	m_bEposSales = bEposSales;
	m_bManualSales = bManualSales;
	m_bImportSales = bImportSales;
	m_bGuestToTerminal = bGuestToTerminal;

	LocSelEntity.GetEPOSSelectArray( m_SelectArray );
	m_nConLevel = LocSelEntity.GetConType();
	m_nTableNo = LocSelEntity.GetEntityNo();
	m_nDbIdx = LocSelEntity.GetDbIdx();

	ResetInternal ( ReportRecord );
}

/**********************************************************************/
#else
/**********************************************************************/

void CSalesHistoryReportPlu::Reset( CSalesHistoryCSVRecordPlu& ReportRecord, CPosTrayTask& EmailTask )
{
	int nReportType = ReportRecord.GetReportType();
	if ( nReportType == SH_CUSTOM )
		ReportRecord.GetReportFields( m_arrayReportFields );
	else
		m_arrayReportFields.RemoveAll();

	m_nReportType = ReportRecord.GetReportType();
	m_nReportMode = ReportRecord.GetReportMode();
	m_strReportName = ReportRecord.GetReportName();

	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	m_bEposSales = EmailTask.GetBatchEposSalesFlag();
	m_bManualSales = EmailTask.GetBatchManualSalesFlag();
	m_bImportSales = EmailTask.GetBatchImportSalesFlag();
	m_bGuestToTerminal = EmailTask.GetBatchPMSTerminalFlag();

	if ( ( FALSE == m_bEposSales ) && ( FALSE == m_bManualSales ) && ( FALSE == m_bImportSales ) )
		m_bEposSales = TRUE;

	CLocationSelectorLookup lookup( EmailTask );
	CLocationSelectorEntity entity( lookup );

	entity.GetEPOSSelectArray( m_SelectArray );
	m_nConLevel = lookup.GetConType();
	m_nTableNo = lookup.GetEntityNo();
	dbDatabase.FindDatabaseByNumber( lookup.GetDbNo(), m_nDbIdx );
	
	ResetInternal ( ReportRecord );
}

/**********************************************************************/
#endif
/**********************************************************************/

void CSalesHistoryReportPlu::ResetInternal( CSalesHistoryCSVRecordPlu& ReportRecord )
{
	m_bHasModifierField = ReportRecord.HasModifierField();

#ifdef STOCKMAN_SYSTEM

	if ( m_bManualSales == TRUE || m_bImportSales == TRUE )
	{
		switch ( m_nConLevel )
		{
		case NODE_DATABASE:
			m_SelectArray.SelectDatabaseForNonEposSales( m_nDbIdx, ( m_bManualSales != 0 ), ( m_bImportSales != 0 ) );
			break;

		case NODE_SYSTEM:
			{
				for ( int n = 0; n < dbDatabase.GetSize(); n++ )
					m_SelectArray.SelectDatabaseForNonEposSales( n, ( m_bManualSales != 0 ), ( m_bImportSales != 0 ) );
			}
			break;
		}
	}
	
#endif

	m_SelectArray.MakeList( TRUE );

	DataManagerNonDb.SessionDateTimeFilter.Reset();
	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);

	m_bEstimatedProfit = FALSE;
	m_bPercentSale = FALSE;
	m_nPercentDepth = 0;
	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;
	m_bCustomer = FALSE;
	m_bVIPSales = FALSE;

	switch ( m_nReportType )
	{
	case SH_PLU_ITEM:
	case SH_PLU_TOP100:
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;
		
	case SH_PLU_ITEM_DEPT:
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		break;

	case SH_PLU_ITEM_MONTH:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;
	
	case SH_PLU_ITEM_MONTH_DEPT:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		break;

	case SH_PLU_TOP100_DEPT:
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		break;

	case SH_PLU_TOP100_MONTH:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;

	case SH_PLU_TOP100_MONTH_DEPT:
		m_arrayReportFields.Add( SH_FIELD_MONTH  );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		break;

	case SH_PLU_UNSOLD:
	case SH_PLU_UNSOLD_DEPT:
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		break;

	case SH_PLU_UNSOLD_MONTH:
	case SH_PLU_UNSOLD_MONTH_DEPT:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_PLUNO );
		break;

	case SH_PLU_DEPT:
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;

	case SH_PLU_DEPT_MONTH:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;

	case SH_PLU_DAILY:
		m_arrayReportFields.Add( SH_FIELD_DATE_BUSINESS );
		break;

	case SH_PLU_MONTHLY:
		m_arrayReportFields.Add( SH_FIELD_MONTH );
		break;

	case SH_PLU_WEEKLY:
		m_arrayReportFields.Add( SH_FIELD_WEEK );
		break;

	case SH_PLU_DAILY_BYWEEK:
		m_arrayReportFields.Add( SH_FIELD_WEEK );
		m_nReportMode = SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC;
		break;

	case SH_PLU_LOC_BYWDAY:
		m_arrayReportFields.Add( SH_FIELD_LOC );
		m_nReportMode = SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC;
		break;

	case SH_PLU_DEPT_BYWDAY:
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_FILTER );
		m_nReportMode = SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC;
		break;

	case SH_PLU_TAX:
		m_arrayReportFields.Add( SH_FIELD_MONTH );
		break;

	default:
		break;
	}

	m_arrayBufferedLines.RemoveAll();
	m_arraySalesChart.RemoveAll();
	m_arrayUnsoldItems.RemoveAll();
	m_QtyValueTable.ClearConsolidatedLevels();
	m_QtyValueTable.ClearFieldStructure();
}

/**********************************************************************/

void CSalesHistoryReportPlu::SetChartDetails( CSalesHistoryCSVRecordPlu& ReportRecord )
{
	m_nReportMode = SALESHISTORY_REPORTMODE_CHART;

	m_chartinfo.SetChartType( ReportRecord.GetChartType() );
	m_chartinfo.SetBottom100Flag( ReportRecord.IsBottom100() );
	m_chartinfo.SetSortedByValueFlag( ReportRecord.IsSortedByValue() );
	m_chartinfo.SetChartSize( ReportRecord.GetChartSize() );
	m_chartinfo.SetChartCutOff( ReportRecord.GetChartCutOff() );

	if ( m_nReportType != SH_CUSTOM )
	{
		CString strBestWorst;
		if ( m_chartinfo.IsBottom100() )
			strBestWorst = "Bottom";
		else
			strBestWorst = "Top";
			
		CString strRepType;
		if ( m_chartinfo.IsSortedByValue() )
			strRepType = "Value";
		else
			strRepType = "Quantity";

		CString strChartSize = "";
		if ( m_chartinfo.GetChartType() == CHART_TYPE_MAXITEMS )
			strChartSize.Format( " %d", m_chartinfo.GetChartSize() );
			
		CString strTitle;
		strTitle.Format ( "%s%s Plu Items by %s",
			(const char*) strBestWorst,
			(const char*) strChartSize,
			(const char*) strRepType );

		m_strReportName = strTitle;
	}

	bool bGotHeader = FALSE;
	for ( int n = m_arrayTotalFlags.GetSize() - 1; n >= 1; n-- )
	{
		if ( ( bGotHeader == FALSE ) && ( m_arrayHeaderFlags.GetAt(n) == 1 ) )
		{
			m_arrayTotalFlags.SetAt( n, 1 );
			bGotHeader = TRUE;
		}
		else
			m_arrayTotalFlags.SetAt( n, 0 );
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::SetTableFilter( CTableFilter info )
{
	m_TableFilter = info;
	m_TableReportHelpers.SetLocIdx(-1);
}

/**********************************************************************/

bool CSalesHistoryReportPlu::PrepareFields()
{
	bool bGotDescriptionField = FALSE;

	if ( NODE_SYSTEM == m_nConLevel )
	{
		bool bGotDatabaseField = FALSE;
		bool bGotNonGlobalField = FALSE;

		for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
		{		
			int nField = m_arrayReportFields.GetAt( nPos );
			int nFieldType = nField % 100;
	
			if ( SH_FIELD_DBASE == nFieldType )
			{
				bGotDatabaseField = TRUE;
				break;
			}

			switch( nFieldType )
			{
			case SH_FIELD_PLUNO:
				bGotNonGlobalField = TRUE;
				break;

			case SH_FIELD_DEPTNO_SALE:
			case SH_FIELD_DEPTNO_ITEM:
			case SH_FIELD_DEPTNO_FILTER:
				if ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;

			case SH_FIELD_ACAT_SALE:
			case SH_FIELD_ACAT_ITEM:
				if ( EcrmanOptions.GetGlobalAnalysisCategoryFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;

			case SH_FIELD_DEPTGRP_SALE:
			case SH_FIELD_DEPTGRP_ITEM:
			case SH_FIELD_DEPTGRP_FILTER:
				if ( EcrmanOptions.GetGlobalDeptGroupEposFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;

			case SH_FIELD_REPGRP:
				if ( EcrmanOptions.GetGlobalDeptGroupReportFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;
		
			case SH_FIELD_CONGRP:
				if ( EcrmanOptions.GetGlobalDeptGroupConsolFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;

			case SH_FIELD_MODIFIER:
				if ( EcrmanOptions.GetGlobalModifierFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;

			case SH_FIELD_TAXBAND:
				if ( EcrmanOptions.GetGlobalTaxFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;
			}
		}

		if ( ( TRUE == bGotNonGlobalField ) && ( FALSE == bGotDatabaseField ) )
		{
			m_arrayReportFields.InsertAt( 0, SH_FIELD_DBASE + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		}
	}

	CReportConsolidationArray<CSortedStringItem> FieldsSoFar;
	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{		
		int nField = m_arrayReportFields.GetAt( nPos );
		int nFieldType = nField % 100;
		bool bAccept = FALSE;

		if ( DataManagerNonDb.SessionDateTimeFilter.GetCalendarDateOnlyFlag() == TRUE )
		{
			if ( nFieldType == SH_FIELD_DATE_BUSINESS )
			{
				nField += ( SH_FIELD_DATE_CALENDAR - SH_FIELD_DATE_BUSINESS );
				m_arrayReportFields.SetAt( nPos, nField );
				nFieldType = SH_FIELD_DATE_CALENDAR;
			}
		}

		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:
		case SH_FIELD_WEEK:
		case SH_FIELD_WEEKDAY:
		case SH_FIELD_DATE_BUSINESS:
		case SH_FIELD_DATE_CALENDAR:
		case SH_FIELD_TIME:
		case SH_FIELD_SEQNO:
		case SH_FIELD_DBASE:
		case SH_FIELD_LOC:
		case SH_FIELD_TERMINAL:
		case SH_FIELD_TABLENO:
		case SH_FIELD_TABLEGRP:
		case SH_FIELD_REPPOINT:
		case SH_FIELD_DEPTNO_SALE:
		case SH_FIELD_DEPTNO_ITEM:
		case SH_FIELD_DEPTNO_FILTER:
		case SH_FIELD_DEPTGRP_SALE:
		case SH_FIELD_DEPTGRP_ITEM:
		case SH_FIELD_DEPTGRP_FILTER:
		case SH_FIELD_REPGRP:
		case SH_FIELD_CONGRP:
		case SH_FIELD_PLUNO:
		case SH_FIELD_DESCRIPTION:
		case SH_FIELD_CASHRSPTEXT:
		case SH_FIELD_ATTRIBUTETEXT1:
		case SH_FIELD_SORTCODE:
		case SH_FIELD_MODIFIER:
		case SH_FIELD_TRANSERVER:
		case SH_FIELD_ITEMSERVER:
		case SH_FIELD_CUSTOMER:
		case SH_FIELD_ACAT_SALE:
		case SH_FIELD_ACAT_ITEM:
		case SH_FIELD_LEVEL:
		case SH_FIELD_TAXBAND:
#ifndef STOCKMAN_SYSTEM
		case SH_FIELD_PLUSUPP:
#endif
			{
				int nPos;
				CString strField;
				strField.Format ( "%3.3d", nFieldType );

				CSortedStringItem item;
				item.m_strItem = strField;

				if ( FieldsSoFar.Find( item, nPos ) == FALSE )
				{
					FieldsSoFar.Consolidate( item );
					bAccept = TRUE;
				}
			}
			break;
		}

		if ( SysInfo.GetMaxBasePluLen() == 0 )
			if ( SH_FIELD_MODIFIER == nFieldType )
				bAccept = FALSE;

		if ( SysInfo.IsEcrLiteSystem() == TRUE )
		{
			switch( nFieldType )
			{
			case SH_FIELD_TABLENO:
			case SH_FIELD_TABLEGRP:
				bAccept = FALSE;
				break;
			}
		}

		if ( bAccept == FALSE )
		{
			m_arrayReportFields.RemoveAt( nPos );
			nPos--;
		}
		else
		{
			switch( nFieldType )
			{
			case SH_FIELD_DESCRIPTION:
			case SH_FIELD_CASHRSPTEXT:
				bGotDescriptionField = TRUE;
				break;
			}
		}
	}

	m_arrayHeaderFlags.RemoveAll();
	m_arrayTotalFlags.RemoveAll();

	//OVERALL REPORT WILL HAVE GRAND TOTAL BUT NO HEADER
	m_arrayHeaderFlags.Add(0);
	m_arrayTotalFlags.Add(1);
	
	int nFieldCount = m_arrayReportFields.GetSize();
	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		int nField = m_arrayReportFields.GetAt(nPos);
		int nFieldType = nField % 100;
		int nTotalType = nField - nFieldType;
		bool bHeader = ( nTotalType == SH_FIELD_HEADER || nTotalType == SH_FIELD_HEADER + SH_FIELD_TOTAL );
		bool bTotal = ( nTotalType == SH_FIELD_TOTAL || nTotalType == SH_FIELD_HEADER + SH_FIELD_TOTAL );
	
		switch( nFieldType )
		{
		case SH_FIELD_DEPTNO_FILTER:
			nFieldType = ( m_bDepartmentByItem ) ? SH_FIELD_DEPTNO_ITEM : SH_FIELD_DEPTNO_SALE;
			break;

		case SH_FIELD_DEPTGRP_FILTER:
			nFieldType = ( m_bDepartmentByItem ) ? SH_FIELD_DEPTGRP_ITEM : SH_FIELD_DEPTGRP_SALE;
			break;

		case SH_FIELD_PLUNO:
			if ( TRUE == bGotDescriptionField )
				nFieldType = SH_FIELD_PLUNO_ONLY;
			break;
		}

		m_arrayReportFields.SetAt( nPos, nFieldType );
		m_arrayHeaderFlags.Add ( bHeader ? 1 : 0 );
		m_arrayTotalFlags.Add ( bTotal ? 1 : 0 );
	}

	m_arrayTotalFlags.SetAt( nFieldCount, 0 );
	m_arrayHeaderFlags.SetAt( nFieldCount, 0 );

	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		switch( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_PLUNO:
		case SH_FIELD_PLUNO_ONLY:
			m_QtyValueTable.AddField( SORT_TYPE_INT64 );
			break;

		case SH_FIELD_TIME:
		case SH_FIELD_TRANSERVER:
		case SH_FIELD_ITEMSERVER:
		case SH_FIELD_CUSTOMER:
		case SH_FIELD_DESCRIPTION:
		case SH_FIELD_CASHRSPTEXT:
		case SH_FIELD_ATTRIBUTETEXT1:
		case SH_FIELD_SORTCODE:
			m_QtyValueTable.AddField( SORT_TYPE_STRING );
			break;

		case SH_FIELD_WEEKDAY:
		case SH_FIELD_MODIFIER:
		case SH_FIELD_LEVEL:
		case SH_FIELD_TAXBAND:
		case SH_FIELD_DBASE:
		case SH_FIELD_LOC:
		case SH_FIELD_TERMINAL:
		case SH_FIELD_TABLENO:
		case SH_FIELD_TABLEGRP:
		case SH_FIELD_REPPOINT:
		case SH_FIELD_DEPTNO_SALE:
		case SH_FIELD_DEPTNO_ITEM:
		case SH_FIELD_DEPTGRP_SALE:
		case SH_FIELD_DEPTGRP_ITEM:
		case SH_FIELD_REPGRP:
		case SH_FIELD_CONGRP:
		case SH_FIELD_ACAT_SALE:
		case SH_FIELD_ACAT_ITEM:
		case SH_FIELD_PLUSUPP:
		case SH_FIELD_SEQNO:
			m_QtyValueTable.AddField( SORT_TYPE_INT );
			break;

		case SH_FIELD_MONTH:
		case SH_FIELD_WEEK:
		case SH_FIELD_DATE_BUSINESS:
		case SH_FIELD_DATE_CALENDAR:
			m_QtyValueTable.AddField( SORT_TYPE_DATE );
			break;
		}
	}

	for ( int nPos = m_arrayTotalFlags.GetSize() - 1; nPos >= 0; nPos-- )
	{
		if ( ( m_arrayTotalFlags.GetAt( nPos) == 1 ) || ( m_arrayHeaderFlags.GetAt( nPos ) == 1 ) )
		{
			m_nPercentDepth = nPos;
			break;
		}
	}
	
	return ( nFieldCount != 0 );
}

/**********************************************************************/

void CSalesHistoryReportPlu::Consolidate() 
{
	m_QtyValueTable.ClearAutoFillDates();

	if ( DealerFlags.GetWeeklyReportNoZeroSkipFlag() == TRUE )
	{
		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
			{
				int nSize = m_arrayReportFields.GetSize();
				if ( nSize > 0 )
				{
					if ( m_arrayReportFields.GetAt( nSize - 1 ) == SH_FIELD_WEEK )
					{
						int nStartOffset = 0;
						int nEndOffset = 0;
						
						COleDateTime oleDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom().m_oleDateTime;
						COleDateTime oleDateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo().m_oleDateTime;
						
						int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay() + 1;
						int nWeekEndDay = ( 1 == nWeekStartDay ) ? 7 : nWeekStartDay - 1;
							
						{
							int nReportStartDay = oleDateFrom.GetDayOfWeek();
						
							if (nReportStartDay < nWeekStartDay)
							{
								nReportStartDay += 7;
							}

							nStartOffset = nReportStartDay - nWeekStartDay;
							oleDateFrom -= COleDateTimeSpan ( nStartOffset, 0, 0, 0 );
						}

						{
							int nReportEndDay = oleDateTo.GetDayOfWeek();
						
							if (nReportEndDay < nWeekStartDay)
							{
								nReportEndDay += 7;
							}

							int nTemp = nReportEndDay - nWeekStartDay;
							oleDateTo -= COleDateTimeSpan ( nTemp, 0, 0, 0 );

							nEndOffset = ( 6 - nTemp );
						}

						m_QtyValueTable.SetAutoFillDates( oleDateFrom, oleDateTo, nStartOffset, nEndOffset );
					}
				}
			}
			break;
		}
	}

	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	dbLocation.ResetRpNoReport();
	dbReportpoint.BuildSortIdxTables();
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();

	CSalesHistoryReportPluConsolidationOptions ConOptions;

	switch( m_nReportType )
	{
	case SH_PLU_UNSOLD:
	case SH_PLU_UNSOLD_DEPT:
	case SH_PLU_UNSOLD_MONTH:
	case SH_PLU_UNSOLD_MONTH_DEPT:
		ConOptions.m_bIsUnsoldItemReport = TRUE;
		break;
	}
	
	m_bWantTableName = FALSE;
	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		switch ( m_arrayReportFields.GetAt(n) )
		{
		case SH_FIELD_TAXBAND:
		case SH_FIELD_ACAT_SALE:
		case SH_FIELD_ACAT_ITEM:
		case SH_FIELD_PLUSUPP:
		case SH_FIELD_DESCRIPTION:
		case SH_FIELD_SORTCODE:
			ConOptions.m_bWantPluInfo = TRUE;
			break;

		case SH_FIELD_DEPTGRP_SALE:
			ConOptions.m_bWantDeptGrpSale = TRUE;
			break;

		case SH_FIELD_DEPTNO_ITEM:
			ConOptions.m_bWantPluInfo = TRUE;
			break;

		case SH_FIELD_DEPTGRP_ITEM:
			ConOptions.m_bWantPluInfo = TRUE;
			ConOptions.m_bWantDeptGrpItem = TRUE;
			break;

		case SH_FIELD_TRANSERVER:
			ConOptions.m_bWantTranServer = TRUE;
			break;

		case SH_FIELD_ITEMSERVER:
			ConOptions.m_bWantItemServer = TRUE;
			break;

		case SH_FIELD_CUSTOMER:
			ConOptions.m_bWantCustomer = TRUE;
			break;

		case SH_FIELD_REPGRP:
			ConOptions.m_bWantRepGrp = TRUE;
			break;

		case SH_FIELD_CONGRP:
			ConOptions.m_bWantConGrp = TRUE;
			break;

		case SH_FIELD_TABLENO:
		case SH_FIELD_TABLEGRP:
			ConOptions.m_bWantTable = TRUE;
			m_bWantTableName = TRUE;
			break;
		}
	}

	if ( m_nTaxFilter != TAXBAND_FILTER_ALL )
		ConOptions.m_bWantPluInfo = TRUE;
	
	if ( m_bDepartmentByItem == TRUE )
		ConOptions.m_bWantPluInfo = TRUE;

	if ( TRUE == m_bEstimatedProfit )
		ConOptions.m_bWantPluInfo = TRUE;

	if ( SH_PLU_TAX == m_nReportType )
		ConOptions.m_bWantPluInfo = TRUE;

	if ( m_TableFilter.m_nType != TABLEFILTER_ALL )
		ConOptions.m_bWantTable = TRUE;

	m_QtyValueTable.ClearConsolidatedLevels();
	ConsolidateInternal(1, ConOptions);
		
	if ( SALESHISTORY_REPORTMODE_TWOBLOCK == m_nReportMode )
	{
		SelectDateBlock(2);
		ConsolidateInternal(2, ConOptions);
		SelectDateBlock(1);
	}	
}

/**********************************************************************/

void CSalesHistoryReportPlu::ConsolidateInternal( int nBlock, CSalesHistoryReportPluConsolidationOptions& ConOptions ) 
{
	CPluSalesHistoryFileArray arrayFileInfo( m_SelectArray );
	arrayFileInfo.SetEposSalesFlag( m_bEposSales );
	arrayFileInfo.SetImportSalesFlag( m_bImportSales );
	arrayFileInfo.SetManualSalesFlag( m_bManualSales );
	arrayFileInfo.SetEODModeFlag( m_bEODMode );
	arrayFileInfo.SetGuestToTerminalFlag( m_bGuestToTerminal );
	arrayFileInfo.SetSaleTimeFlag( m_bSaleTime );
	arrayFileInfo.BuildFileList();

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	bool bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();

	CReportCostFinder CostFinder;
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetFileCount(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetFileInfo( nFileIdx, infoFile );
		bool bIsManualOrImportFile = FALSE;
		
		int nDbIdx;
		int nDbNo;
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( infoFile.m_nSelectArrayIdx, ListItem );
			nDbIdx = ListItem.m_nDbIdx;
			nDbNo = dbDatabase.GetDbNo( nDbIdx );
			
			switch( ListItem.m_nLineType )
			{
			case EPOSSELECT_DB_MANUAL:
			case EPOSSELECT_DB_IMPORT:
				bIsManualOrImportFile = TRUE;
				break;
			}
		}
		
		int nRpIdx = 0;

		if ( infoFile.m_nTermIdx != 0 )
		{
			int nRpNo = dbLocation.GetRpNoReport( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );
			if ( dbReportpoint.FindReportpointByNumber( nDbNo, nRpNo, nRpIdx ) == TRUE )
				nRpIdx = dbReportpoint.GetSortIdxFromRpIdx( nRpIdx ) + 1;
			else
				nRpIdx = 0;
		}

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CPluSalesLine csvSales ( infoFile.IsLegacySalesFolder() );

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
		int nSortLocIdx = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
		
		int nTNo = 0;
		if ( infoFile.m_nTermIdx != 0 )
			nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );

		//THESE VALUES STAY THE SAME WITHIN EACH FILE
		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		{
			switch ( m_arrayReportFields.GetAt( n ) )
			{
			case SH_FIELD_DBASE:	m_QtyValueTable.SetBufferInt( n, nSortDbIdx );		break;
			case SH_FIELD_LOC:		m_QtyValueTable.SetBufferInt( n, nSortLocIdx );		break;
			case SH_FIELD_TERMINAL:	m_QtyValueTable.SetBufferInt( n, nTNo );			break;
			case SH_FIELD_REPPOINT:	m_QtyValueTable.SetBufferInt( n, nRpIdx );			break;	
			}
		}
	
		//MAKE SURE THAT THE RIGHT DATABASE IS LOADED FOR TAX AND GROUP INFO
		if ( m_ReportFilters.SetDatabase( nDbIdx ) == TRUE )
		{
			if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
				CostFinder.Init();
		}

		bool bIsPMSLocation = dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx );

#ifdef COMPILE_PMS
		CPMSReportFileFinder PMSReportFileFinder;
		PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, "", "", TRUE );
#endif

		CString strThisFilterDate = "";
		CString strLastFilterDate = "";
		CString strThisFilterTime = "";
		COleDateTime oleThisFilterDate = COleDateTime( 2000, 1, 1, 0, 0, 0 );
		
		if ( FALSE == bIsPMSLocation )
		{
			if ( TRUE == m_bSaleTime )
			{
				strThisFilterDate = infoFile.m_strDateSale;
				strLastFilterDate = infoFile.m_strDateSale;
			}
			else
			{
				strThisFilterDate = infoFile.m_strDateTran;
				strLastFilterDate = infoFile.m_strDateTran;
			}

			oleThisFilterDate = COleDateTime( atoi( strThisFilterDate.Left(4) ), atoi( strThisFilterDate.Mid(4,2) ), atoi( strThisFilterDate.Mid(6,2) ), 0, 0, 0 );
		}

		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );

		if ( TRUE == ConOptions.m_bWantTable )
		{
			m_TableReportHelpers.SetLocIdx( infoFile.m_nLocIdx );
			m_TableReportHelpers.BuildGroupMap( m_TableFilter );
		}
				
		int nConnectionType = CONNECTION_TYPE_SPTBOOK_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		int nLinesToRead;
		CString strBuffer;
		bool bGotSalesToday = FALSE;
		CBusinessDateInfo BusinessDateInfo;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), arrayFileInfo.GetTotalFileSize() );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
#ifdef COMPILE_PMS
				if ( TRUE  == bIsPMSLocation )
				{
					if ( strBuffer.Left(9) == "#PMS_TIME" )
					{
						int nDummy;
						PMSReportFileFinder.ProcessPMSTimeLine( strBuffer, nDummy, oleThisFilterDate, strThisFilterDate, strThisFilterTime );
						bPMSInclude = DataManagerNonDb.SessionDateTimeFilter.CheckDate( strThisFilterDate );
						continue;
					}

					if ( FALSE == bPMSInclude )
						continue;
				}
#endif

				csvSales.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csvSales.GetLineType();
				{
					bool bAllowType = FALSE;
					
					switch( nLineType )
					{
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						bAllowType = ( DealerFlags.GetHoltsDiscountFlag() == FALSE ); 
						bAllowType &= ( m_ReportFilters.PluFilter.GetCurrentFilterNo() == 0 );
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
						bAllowType = ( DealerFlags.GetHoltsDiscountFlag() == FALSE ); 
						break;

					case CSVPLUDATA_PLU_SALE:
						bAllowType = TRUE;
						break;

					case CSVPLUDATA_PLU_CHEAPESTFREE:
					default:
						bAllowType = FALSE;
						break;
					}

					if ( FALSE == bAllowType )
						continue;
				}

				//FILTER FOR POST TO ROOM
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( ( FALSE == m_bGuestToTerminal ) && ( FALSE == bIsPMSLocation ) )
						if ( DataManager.Payment.IsPMSPaymentType( csvSales.GetPluPaymentNo() ) )
							continue;

				int nLevel = csvSales.GetPriceBand() + 1;
				if ( CONNECTION_TYPE_SPTBOOK_NONE != nConnectionType )
				{
					if ( ( nLevel < 1 ) || ( nLevel >= MAX_PLU_PRICES ) )
						nLevel = 1;
				}
				else
				{
					nLevel = DataManager.SBPriceText.GetReportMapPriceBand( nLevel - 2 ) + 1;
				}

				//THROW OUT NON VIP SALES FOR VIP MODE
				if ( TRUE == m_bVIPSales )
				{
					if ( ( CONNECTION_TYPE_STANDARD_NONE != nConnectionType ) || ( csvSales.IsVIPSale() == FALSE ) )
						continue;
				}
				
				//TRANSLATE SERVER INTO CONSOLIDATION STRING
				CString strTranServer = "";
				if ( TRUE == ConOptions.m_bWantTranServer )
					DataManagerNonDb.ServerNameTable.GetServerConsolidationString( nDbIdx, infoFile.m_nLocIdx, csvSales.GetTranServerNo(), strTranServer );

				CString strItemServer = "";
				if ( TRUE == ConOptions.m_bWantItemServer )
					DataManagerNonDb.ServerNameTable.GetServerConsolidationString( nDbIdx, infoFile.m_nLocIdx, csvSales.GetItemServerNo(), strItemServer );

				//TRANSLATE CUSTOMER INTO CONSOLIDATION STRING
				int nCustomerID = csvSales.GetCustomerID();
				if ( TRUE == m_bCustomer )
					if ( 0 == nCustomerID )
						continue;

				CString strCustomer = "";
				if ( TRUE == ConOptions.m_bWantCustomer )
				{
					if ( 0 == nCustomerID )
						strCustomer = "3";
					else
					{
						CCustomerSelectionListAccountInfo infoCustomer;
						infoCustomer.m_nLocIdx = infoFile.m_nLocIdx;
						infoCustomer.m_nCustomerID = nCustomerID;

						int nNodeType, nPos;
						DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoCustomer, nNodeType, nPos );
						switch( nNodeType )
						{
						case NODE_CUSTOMER_ACCOUNT:
							strCustomer.Format ( "0%8.8d%8.8d", infoCustomer.m_nSortIdx, nPos );
							break;

						case NODE_CUSTOMER_ONEOFF:
							strCustomer.Format ( "1%8.8d%8.8d", infoCustomer.m_nSortIdx, nPos );
							break;

						case NODE_PLACEHOLDER:
							strCustomer.Format ( "2%8.8d", nCustomerID );
							break;
						}
					}
				}

				//FILTER BY TABLE
				int nTableNo = 0;
				int nTableGrp = 0;
				if ( TRUE == ConOptions.m_bWantTable )
				{
					nTableNo = csvSales.GetTranTableNo();
					if ( m_TableReportHelpers.CheckTableGroup( nTableNo, nTableGrp ) == FALSE )
						continue;
				}

				//FILTER BY DATE AND TIME
				if ( FALSE == bIsPMSLocation )
				{
					if ( TRUE == m_bSaleTime )
						strThisFilterTime = csvSales.GetSaleTime();
					else	
						strThisFilterTime = csvSales.GetTranTime();
				}

				if ( ( TRUE == bIsPMSLocation ) && ( strThisFilterDate != strLastFilterDate ) )
				{	
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisFilterDate );
					oleThisFilterDate = COleDateTime( atoi( strThisFilterDate.Left(4) ), atoi( strThisFilterDate.Mid(4,2) ), atoi( strThisFilterDate.Mid(6,2) ), 0, 0, 0 );
					strLastFilterDate = strThisFilterDate;
				}

				if ( oleThisFilterDate.m_status != COleDateTime::valid )
					continue;

				switch( strThisFilterTime.GetLength() )
				{
				case 6:
					break;

				case 4:
					strThisFilterTime += "00";
					break;

				default:
					strThisFilterTime = "000000";
					break;
				}
				
				int nHour = atoi( strThisFilterTime.Left(2) );
				int nMinute = atoi( strThisFilterTime.Mid(2,2) );
				int nSecond = atoi( strThisFilterTime.Right(2) ); 
				
				//ROUND TO TIME SLICE IF REQUIRED
				switch( m_TimeSliceMap.GetReportType() )
				{
				case SH_TIMESLICE_SECOND:
					break;

				case SH_TIMESLICE_MINUTE:
					nSecond = 0;
					break;

				case SH_TIMESLICE_5MIN:
					nSecond = 0;
					nMinute -= ( nMinute % 5 );
					break;

				case SH_TIMESLICE_10MIN:
					nSecond = 0;
					nMinute -= ( nMinute % 10 );
					break;

				case SH_TIMESLICE_15MIN:
					nSecond = 0;
					nMinute -= ( nMinute % 15 );
					break;

				case SH_TIMESLICE_30MIN:
					nSecond = 0;
					nMinute -= ( nMinute % 30 );
					break;

				case SH_TIMESLICE_60MIN:
					nSecond = 0;
					nMinute = 0;
					break;
				}

				//CHECK FOR PREVIOUS DATE
				if ( BusinessDateInfo.ValidateBusinessDate( infoFile.m_nLocIdx, m_bEODMode, strThisFilterDate, strThisFilterTime ) == FALSE )
					continue;

				CString strLineTime = "";
				if ( m_TimeSliceMap.GetReportType() == SH_CUSTOM )
					strLineTime = m_TimeMap.GetSortOrder( nHour, nMinute );
				else
				{
					strLineTime.Format( "%1.1d%2.2d%2.2d%2.2d",
						BusinessDateInfo.GetTimeSortType() - 1,
						nHour,
						nMinute,
						nSecond );
				}
				
				//FILTER BY DEPT NO OF SALE
				int nDeptNoSale = csvSales.GetDeptNo();

				if ( ConOptions.m_bIsUnsoldItemReport == FALSE )
					if ( m_bDepartmentByItem == FALSE )
						if ( m_ReportFilters.CheckDepartment( nDeptNoSale ) == FALSE )
							continue;

				int nDeptGrpSale = 0;
				if ( ConOptions.m_bWantDeptGrpSale == TRUE )
				{
					nDeptGrpSale = csvSales.GetGroupNo() - 1;

					if ( nDeptGrpSale < 0 )
						nDeptGrpSale = 0;
				}

				__int64 nBasePluNo = 0;
				int nModifier = 0;
				double dMultiplier = 1.0;
				
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					nBasePluNo = CEposReport::GetSeparatedPromoPluNo( csvSales.GetPromoNoSeparate() );
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					nBasePluNo = CEposReport::GetSeparatedMixMatchPluNo( csvSales.GetMixMatchNo() );
					break;

				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csvSales.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
						
						if ( FALSE == infoPluNo.m_bValid )
							continue;

						if ( FALSE == ConOptions.m_bIsUnsoldItemReport )
							if ( m_ReportFilters.CheckPlu( infoPluNo ) == FALSE )
								continue;

						nBasePluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;

						if ( nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( nModifier );
					}
					break;
				}

				//FILTER BY PLU RANGE
				if ( m_bItemRange == TRUE )
				{
					if ( ( m_nItemFrom != 0 ) && ( nBasePluNo < m_nItemFrom ) )
						continue;

					if ( ( m_nItemTo != 0 ) && ( nBasePluNo > m_nItemTo ) )
						continue;
				}

				//SORT BY TAX BAND
				int nTaxBand = 0;
				int nCategorySale = 0;
				int nCategoryItem = 0;
				int nPluSupp = 0;
				int nDeptNoItem = 0;
				double dCostPerPlu = 0.0;
				CString strDescription = "";
				CString strSortCode = "";

				if ( ConOptions.m_bWantPluInfo == TRUE )
				{
					nCategorySale = csvSales.GetAnalysisNo();
					nCategoryItem = 0;

					bool bReportCost = DealerFlags.GetReportCostPriceFlag();

					m_PluInfoFinder.SetPluNo( nBasePluNo );

					if ( m_PluInfoFinder.PluExists() == TRUE )
					{
						nTaxBand = m_PluInfoFinder.GetModifierTaxBand( nModifier );
						nPluSupp = m_PluInfoFinder.GetSupplierNo();
						nCategoryItem = m_PluInfoFinder.GetModifierAnalysisCategory( nModifier );
						strDescription = "2";
						strDescription += m_PluInfoFinder.GetPluInfoRepText( TRUE, nModifier, FALSE );
						strSortCode = "2";
						strSortCode += m_PluInfoFinder.GetPluInfoSortCode( TRUE, nModifier, FALSE );

						if (0 == nCategorySale)
						{
							nCategorySale = nCategoryItem;
						}

						nDeptNoItem = m_PluInfoFinder.GetModifierDeptNo(nModifier);

						if (TRUE == m_bEstimatedProfit)
						{
							if (FALSE == bReportCost)
							{
								dCostPerPlu = m_PluInfoFinder.GetCost();
							}
							else
							{
								dCostPerPlu = CostFinder.GetPluCostByPluIdx(m_PluInfoFinder.GetPluIdx(), infoFile.m_nLocIdx);
							}
						}
					}
					else if ( m_PluInfoFinder.IsUnknownSpecialPlu() == TRUE )
					{
						strDescription = "1";
						strDescription += m_PluInfoFinder.GetPluInfoRepText(TRUE);
						strSortCode = "1";
						strSortCode += m_PluInfoFinder.GetPluInfoSortCode(TRUE);
					}
					else
					{
						strDescription = "0";
						strDescription += m_PluInfoFinder.GetPluInfoRepText(TRUE);
						strSortCode = "0";
						strSortCode += m_PluInfoFinder.GetPluInfoSortCode(TRUE);
					}

					if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
						nTaxBand = 0;
					else if ( (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE) && ( FALSE == bIsManualOrImportFile ) )
						nTaxBand = csvSales.GetNumericTaxBand();

					//FILTER BY TAX BAND
					if (m_nTaxFilter != TAXBAND_FILTER_ALL)
					{
						int nMask = 1;

						if ((nTaxBand >= 1) && (nTaxBand <= MAX_TAX_BANDS))
						{
							nMask <<= nTaxBand;
						}
						
						if ((m_nTaxFilter & nMask) == 0)
						{
							continue;
						}
					}
				}

				int nDeptGrpItem = 0;
				if ( ConOptions.m_bWantDeptGrpItem == TRUE )
				{
					int nDeptIdx;
					if ( DataManager.Department.FindDeptByNumber( nDeptNoItem, nDeptIdx ) == TRUE )
					{
						CDepartmentCSVRecord deptRecord;
						DataManager.Department.GetAt( nDeptIdx, deptRecord );
						nDeptGrpItem = deptRecord.GetEposGroup();
					}
				}

				//FILTER BY DEPARTMENT NUMBER OF ITEM
				if ( ConOptions.m_bIsUnsoldItemReport == FALSE )
					if ( m_bDepartmentByItem == TRUE )
						if ( m_ReportFilters.CheckDepartment( nDeptNoItem ) == FALSE )
							continue;

				//FILTER BY PRICE LEVEL
				if ( PRICELEVEL_FILTER_ALL != m_nPriceLevelFilter )
				{
					int nMask = 0;
						
					switch ( nLevel )
					{
					case 1:	nMask = 1;		break;
					case 2:	nMask = 2;		break;
					case 3:	nMask = 4;		break;
					case 4:	nMask = 8;		break;
					case 5:	nMask = 16;		break;
					case 6:	nMask = 32;		break;
					case 7:	nMask = 64;		break;
					case 8:	nMask = 128;	break;
					case 9:	nMask = 256;	break;
					case 10:nMask = 512;	break;
					}
					
					if ( ( nMask != 0 ) && ( ( m_nPriceLevelFilter & nMask ) == 0 ) )
						continue;
				}

				//FILTER BY MODIIFIER
				if ( MODIFIER_FILTER_ALL != m_nModifierFilter )
				{
					int nMask = 0;
						
					switch ( nModifier )
					{
					case 0:	nMask = 1;		break;
					case 1:	nMask = 2;		break;
					case 2:	nMask = 4;		break;
					case 3:	nMask = 8;		break;
					case 4:	nMask = 16;		break;
					case 5:	nMask = 32;		break;
					case 6:	nMask = 64;		break;
					case 7:	nMask = 128;	break;
					case 8:	nMask = 256;	break;
					case 9:	nMask = 512;	break;
					}
					
					if ( ( nMask != 0 ) && ( ( m_nModifierFilter & nMask ) == 0 ) )
						continue;
				}

				int nRepGrp = 0;
				int nConGrp = 0;
				
				if ( ( TRUE == ConOptions.m_bWantRepGrp ) || ( TRUE == ConOptions.m_bWantConGrp ) ) 
				{
					int nDeptIdx;
					int nDeptNo =  m_bDepartmentByItem ? nDeptNoItem : nDeptNoSale;
					
					if ( nDeptNo == 0 )
					{
						nRepGrp = 1000;
						nConGrp = 1000;
					}
					else
					{
						if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == TRUE )
						{
							CDepartmentCSVRecord DeptRecord;
							DataManager.Department.GetAt( nDeptIdx, DeptRecord );
							nRepGrp = DeptRecord.GetReportGroup();
							
							if ( TRUE == ConOptions.m_bWantConGrp )
							{
								int nRepGrpIdx;
								if ( DataManager.ReportGroup.FindGroupByNumber( nRepGrp, nRepGrpIdx ) == TRUE )
								{
									CGroupCSVRecordReport ReportGroup;
									DataManager.ReportGroup.GetAt( nRepGrpIdx, ReportGroup );
									nConGrp = ReportGroup.GetParentGroupNo();
								}
							}
						}
					}
				}

				if ( 0 == nDeptNoSale )		nDeptNoSale = 1000;
				if ( 0 == nDeptNoItem )		nDeptNoItem = 1000;
				if ( 0 == nDeptGrpSale )	nDeptGrpSale = 1000;
				if ( 0 == nDeptGrpItem )	nDeptGrpItem = 1000;
				
				//THESE VALUES CHANGE ON A LINE-BY-LINE BASIS
				for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
				{
					switch ( m_arrayReportFields.GetAt( n ) )
					{
					case SH_FIELD_MONTH:		m_QtyValueTable.SetBufferDate( n, BusinessDateInfo.GetBusinessDateMonth() );	break;
					case SH_FIELD_WEEK:			m_QtyValueTable.SetBufferDate( n, BusinessDateInfo.GetBusinessDateWeek() );		break;
					case SH_FIELD_WEEKDAY:		m_QtyValueTable.SetBufferInt( n, BusinessDateInfo.GetWeekday() );				break;
					case SH_FIELD_DATE_BUSINESS:m_QtyValueTable.SetBufferDate( n, BusinessDateInfo.GetBusinessDate() );			break;
					case SH_FIELD_DATE_CALENDAR:m_QtyValueTable.SetBufferDate( n, oleThisFilterDate ); break;
					case SH_FIELD_TIME:			m_QtyValueTable.SetBufferString( n, strLineTime ); break;
					case SH_FIELD_TABLENO:		m_QtyValueTable.SetBufferInt( n, nTableNo );	break;
					case SH_FIELD_TABLEGRP:		m_QtyValueTable.SetBufferInt( n, nTableGrp );	break;
					case SH_FIELD_SEQNO:		m_QtyValueTable.SetBufferInt( n, csvSales.GetTranSeqNo() );	break;
					case SH_FIELD_TRANSERVER:	m_QtyValueTable.SetBufferString( n, strTranServer ); break;
					case SH_FIELD_ITEMSERVER:	m_QtyValueTable.SetBufferString( n, strItemServer ); break;
					case SH_FIELD_CUSTOMER:		m_QtyValueTable.SetBufferString( n, strCustomer ); break;
					case SH_FIELD_PLUSUPP:		m_QtyValueTable.SetBufferInt( n, nPluSupp ); break;
					case SH_FIELD_ACAT_SALE:	m_QtyValueTable.SetBufferInt( n, nCategorySale ); break;
					case SH_FIELD_ACAT_ITEM:	m_QtyValueTable.SetBufferInt( n, nCategoryItem ); break;
					case SH_FIELD_DEPTNO_SALE:	m_QtyValueTable.SetBufferInt( n, nDeptNoSale ); break;
					case SH_FIELD_DEPTNO_ITEM:	m_QtyValueTable.SetBufferInt( n, nDeptNoItem ); break;
					case SH_FIELD_DEPTGRP_SALE:	m_QtyValueTable.SetBufferInt( n, nDeptGrpSale );	break;
					case SH_FIELD_DEPTGRP_ITEM:	m_QtyValueTable.SetBufferInt( n, nDeptGrpItem );	break;
					case SH_FIELD_REPGRP:		m_QtyValueTable.SetBufferInt( n, nRepGrp );	break;
					case SH_FIELD_CONGRP:		m_QtyValueTable.SetBufferInt( n, nConGrp );	break;
					case SH_FIELD_PLUNO:		m_QtyValueTable.SetBufferInt64( n, nBasePluNo ); break;
					case SH_FIELD_PLUNO_ONLY:	m_QtyValueTable.SetBufferInt64( n, nBasePluNo ); break;
					case SH_FIELD_DESCRIPTION:	m_QtyValueTable.SetBufferString( n, strDescription ); break;
					case SH_FIELD_SORTCODE:		m_QtyValueTable.SetBufferString( n, strSortCode ); break;
					case SH_FIELD_LEVEL:		m_QtyValueTable.SetBufferInt( n, nLevel ); break;
					case SH_FIELD_TAXBAND:		m_QtyValueTable.SetBufferInt( n, nTaxBand ); break;
					case SH_FIELD_MODIFIER:		m_QtyValueTable.SetBufferInt( n, nModifier ); break;

					case SH_FIELD_CASHRSPTEXT:	
						{
							CString strText = csvSales.GetCashRSPText(); 
							m_QtyValueTable.SetBufferString( n, strText  ); 
						}
						break;

					case SH_FIELD_ATTRIBUTETEXT1:
					{
						CString strText = csvSales.GetAttributeText1();
						m_QtyValueTable.SetBufferString(n, strText);
					}
					break;
					}
				}

				double dLineBaseQty = 0.0;
				double dLineModQty = 0.0;
				double dLineVal = 0.0;
				double dLineCost = 0.0;
				double dLineTax = 0.0;
				double dLineWaste = 0.0;
				double dLineDiscountVal = 0.0;
				double dLinePremiumVal = 0.0;
				bool bConsolidateLine = FALSE;
				
				if ( csvSales.GetPluSaleWastageFlag() == TRUE )
				{
					if ( TRUE == m_bEstimatedProfit )
					{
						dLineWaste = csvSales.GetSaleQty() * dMultiplier * dCostPerPlu;
						bConsolidateLine = TRUE;
					}
				}
				else 
				{
					switch( nLineType )
					{
					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
					{
						bConsolidateLine = TRUE;

						double dRawDiscount = csvSales.GetValue();
						if ((FALSE == bPremiumDetail) || (csvSales.IsPremium() == FALSE))
						{
							dLineDiscountVal = dRawDiscount;
						}
						else
						{
							dLinePremiumVal = dRawDiscount * (-1);
						}

						if ((TRUE == m_bEstimatedProfit) || (SH_PLU_TAX == m_nReportType))
						{
							double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
							dLineTax = dRawDiscount - ReportHelpers.CalcNonTax(dRawDiscount, dTaxRate);
							dLineTax *= -1;
							dLineBaseQty = 0.0;
							dLineModQty = 0.0;
						}
					}
					break;

					case CSVPLUDATA_PROMO_TAXABLE:
					{
						bConsolidateLine = TRUE;

						double dRawDiscount = csvSales.GetValue();
						if ((FALSE == bPremiumDetail) || (csvSales.IsPremium() == FALSE))
						{
							dLineDiscountVal = dRawDiscount;
						}
						else
						{
							dLinePremiumVal = dRawDiscount * (-1);
						}

						dLineBaseQty = csvSales.GetSaleQty();
						dLineModQty = dLineBaseQty;

						if ((TRUE == m_bEstimatedProfit) || (SH_PLU_TAX == m_nReportType))
						{
							double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
							dLineTax = dRawDiscount - ReportHelpers.CalcNonTax(dRawDiscount, dTaxRate);
							dLineTax *= -1;
						}
					}
					break;

					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					{
						bConsolidateLine = TRUE;
						
						double dRawDiscount = csvSales.GetValue();
						if ((FALSE == bPremiumDetail) || (csvSales.IsPremium() == FALSE))
						{
							dLineDiscountVal = dRawDiscount;
						}
						else
						{
							dLinePremiumVal = dRawDiscount * (-1);
						}
						
						dLineTax = 0.0;
						dLineBaseQty = csvSales.GetSaleQty();
						dLineModQty = dLineBaseQty;
					}
					break;

					case CSVPLUDATA_PLU_SALE:
						if (csvSales.GetPluSaleVoidFlag() == FALSE)
						{
							bConsolidateLine = TRUE;

							dLineModQty = csvSales.GetSaleQty();
							dLineBaseQty = dLineModQty * dMultiplier;
							dLineVal = csvSales.GetValue();

							if ((TRUE == m_bEstimatedProfit) || (SH_PLU_TAX == m_nReportType))
							{
								double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
								dLineTax = dLineVal - (ReportHelpers.CalcNonTax(dLineVal, dTaxRate));
								dLineCost = dCostPerPlu * dLineModQty * dMultiplier;
							}
						}
						break;
					}
				}

				if ( TRUE == bConsolidateLine )
				{
					switch( m_nReportMode )
					{
					case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
					{
						int nWeekday = BusinessDateInfo.GetWeekday();

						CEntityTreePluSalesHistoryWeeklyOneTotalNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dLineVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock.SetPremiumVal(dLinePremiumVal);
						Node.SetDailyVal(nWeekday, dLineVal);
						m_QtyValueTable.Consolidate(Node, nWeekday);
					}
					break;

					case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
						{
							int nWeekday = BusinessDateInfo.GetWeekday();
							double dVal = dLineVal - dLineDiscountVal;

							CEntityTreePluSalesHistoryWeeklyOneTotalNode Node;
							Node.m_SalesBlock.SetBaseQty( dLineBaseQty );
							Node.m_SalesBlock.SetModQty( dLineModQty );
							Node.m_SalesBlock.SetTotalVal( dVal );
							Node.m_SalesBlock.SetCost( dLineCost );
							Node.m_SalesBlock.SetTaxAmount( dLineTax );
							Node.m_SalesBlock.SetWasteCost( dLineWaste );
							Node.m_SalesBlock.SetDiscountVal( 0.0 );
							Node.m_SalesBlock.SetPremiumVal(0.0);
							Node.SetDailyVal( nWeekday, dVal );
							m_QtyValueTable.Consolidate( Node, nWeekday );
						}
						break;

					case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
					{
						int nWeekday = BusinessDateInfo.GetWeekday();

						CEntityTreePluSalesHistoryWeeklyOneTotalNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dLineVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock.SetPremiumVal(dLinePremiumVal);
						Node.SetDailyVal(nWeekday, dLineBaseQty);
						m_QtyValueTable.Consolidate(Node, nWeekday);
					}
					break;

					case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:
					{
						int nWeekday = BusinessDateInfo.GetWeekday();

						CEntityTreePluSalesHistoryWeeklyTwoTotalNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dLineVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock.SetPremiumVal(dLinePremiumVal);
						Node.SetDailyQty(nWeekday, dLineBaseQty);
						Node.SetDailyVal(nWeekday, dLineVal);
						m_QtyValueTable.Consolidate(Node, nWeekday);
					}
					break;

					case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
					{
						int nWeekday = BusinessDateInfo.GetWeekday();
						double dVal = dLineVal - dLineDiscountVal;

						CEntityTreePluSalesHistoryWeeklyTwoTotalNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(0.0);
						Node.m_SalesBlock.SetPremiumVal(0.0);
						Node.SetDailyQty(nWeekday, dLineBaseQty);
						Node.SetDailyVal(nWeekday, dVal);
						m_QtyValueTable.Consolidate(Node, nWeekday);
					}
					break;

					case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
					{
						int nMonthday = BusinessDateInfo.GetMonthday() - 1;

						CEntityTreePluSalesHistoryMonthlyNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dLineVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock.SetPremiumVal(dLinePremiumVal);
						Node.SetDailyVal(nMonthday, dLineVal);
						m_QtyValueTable.Consolidate(Node, nMonthday);
					}
					break;

					case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:
					{
						int nMonthday = BusinessDateInfo.GetMonthday() - 1;
						double dVal = dLineVal - dLineDiscountVal;

						CEntityTreePluSalesHistoryMonthlyNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(0.0);
						Node.m_SalesBlock.SetPremiumVal(0.0);
						Node.SetDailyVal(nMonthday, dVal);
						m_QtyValueTable.Consolidate(Node, nMonthday);
					}
					break;

					case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
					{
						int nMonthday = BusinessDateInfo.GetMonthday() - 1;

						CEntityTreePluSalesHistoryMonthlyNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dLineVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock.SetPremiumVal(dLinePremiumVal);
						Node.SetDailyVal(nMonthday, dLineVal);
						m_QtyValueTable.Consolidate(Node, nMonthday);
					}
					break;

					case SALESHISTORY_REPORTMODE_TWOBLOCK:
					{
						CEntityTreePluSalesHistoryTwoBlockNode Node;
						int nConsolBlock = (nBlock == 2) ? 1 : 0;
						Node.m_SalesBlock[nConsolBlock].SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock[nConsolBlock].SetModQty(dLineModQty);
						Node.m_SalesBlock[nConsolBlock].SetTotalVal(dLineVal);
						Node.m_SalesBlock[nConsolBlock].SetCost(dLineCost);
						Node.m_SalesBlock[nConsolBlock].SetTaxAmount(dLineTax);
						Node.m_SalesBlock[nConsolBlock].SetWasteCost(dLineWaste);
						Node.m_SalesBlock[nConsolBlock].SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock[nConsolBlock].SetPremiumVal(dLinePremiumVal);
						m_QtyValueTable.Consolidate(Node);
					}
					break;

					default:
					{
						CEntityTreePluSalesHistoryNormalNode Node;
						Node.m_SalesBlock.SetBaseQty(dLineBaseQty);
						Node.m_SalesBlock.SetModQty(dLineModQty);
						Node.m_SalesBlock.SetTotalVal(dLineVal);
						Node.m_SalesBlock.SetCost(dLineCost);
						Node.m_SalesBlock.SetTaxAmount(dLineTax);
						Node.m_SalesBlock.SetWasteCost(dLineWaste);
						Node.m_SalesBlock.SetDiscountVal(dLineDiscountVal);
						Node.m_SalesBlock.SetPremiumVal(dLinePremiumVal);
						m_QtyValueTable.Consolidate(Node);
					}
					break;
					}

					if ( TRUE == bIsPMSLocation )
					{
						if ( ( infoFile.m_nSelectArrayIdx >= 0 ) && ( infoFile.m_nSelectArrayIdx < m_SelectArray.GetListSize() ) )
						{
							m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, oleThisFilterDate );
							m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleThisFilterDate );
						}
					}
					else
					{
						bGotSalesToday = TRUE;
					}
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();

		if ( bGotSalesToday == TRUE )
		{
			if ( ( infoFile.m_nSelectArrayIdx >= 0 ) && ( infoFile.m_nSelectArrayIdx < m_SelectArray.GetListSize() ) )
			{
				m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, oleThisFilterDate );
				m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleThisFilterDate );
			}
		}
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CSalesHistoryReportPlu::CreatePluSalesHistoryReport()
{
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );
	
	bool bResult = FALSE;

	StatusProgress.Lock( TRUE, "Creating report" );
	switch ( m_nReportType )
	{
	case SH_PLU_TAX:				bResult = CreateTaxReportInternal();			break;
	case SH_PLU_UNSOLD:
	case SH_PLU_UNSOLD_DEPT:		bResult = CreateUnsoldReportInternal();			break;
	case SH_PLU_UNSOLD_MONTH:
	case SH_PLU_UNSOLD_MONTH_DEPT:	bResult = CreateUnsoldMonthlyReportInternal();	break;
	default:						bResult = CreateCustomReportInternal();			break;
	}
	StatusProgress.Unlock();
	
	return bResult;
}

/**********************************************************************/

void CSalesHistoryReportPlu::AddReportColumn( CSalesHistoryReportPluColumn& Column )
{
	if ( Column.m_nTotalType != TOTAL_TYPE_NONE )
	{
		if ( m_nFirstTotalColumn < 0 )
			m_nFirstTotalColumn = m_arrayReportColumns.GetSize();
	}

	m_arrayReportColumns.Add( Column );

	switch( Column.m_nTotalType )
	{
	case TOTAL_TYPE_SALES:
	case TOTAL_TYPE_BASEQTY:
	case TOTAL_TYPE_MODQTY:
	case TOTAL_TYPE_DISCOUNT:
	case TOTAL_TYPE_PREMIUM:
	case TOTAL_TYPE_NET:
	case TOTAL_TYPE_AVERAGE:
	case TOTAL_TYPE_TAX:
	case TOTAL_TYPE_NETOFTAX:
	case TOTAL_TYPE_COST:
	case TOTAL_TYPE_WASTE:
	case TOTAL_TYPE_GP:
		m_ReportFile.AddColumn ( Column.m_strText, Column.m_nAlign, Column.m_nWidth );
		m_csvUnderline.Add( "<LI>" );
		break;

	case TOTAL_TYPE_DAILY:
		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
			{
				int nDay = EcrmanOptions.GetReportsWeekStartDay();
				if ( nDay < 0 || nDay > 6 ) nDay = 0;
			
				for ( int n = 0; n < 7; n++ )
				{
					m_ReportFile.AddColumn ( m_strWeekDays [ (nDay++) % 7 ], Column.m_nAlign, Column.m_nWidth );
					m_csvUnderline.Add( "<LI>" );
				}
			}
			break;

		case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
		{
			int nDay = EcrmanOptions.GetReportsWeekStartDay();
			if (nDay < 0 || nDay > 6) nDay = 0;

			for (int n = 0; n < 7; n++)
			{
				CString strWeekday = m_strWeekDays[(nDay++) % 7];
				m_ReportFile.AddColumn(strWeekday + "Qty", Column.m_nAlign, Column.m_nWidth);
				m_ReportFile.AddColumn(strWeekday + "Val", Column.m_nAlign, Column.m_nWidth);
				m_csvUnderline.Add("<LI>");
				m_csvUnderline.Add("<LI>");
			}
		}
		break;

		case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:
		case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
			for ( int n = 1; n <= 31; n++ )
			{
				CString strNum;
				strNum.Format( "%2.2d", n );
				m_ReportFile.AddColumn ( strNum, Column.m_nAlign, Column.m_nWidth );
				m_csvUnderline.Add( "<LI>" );
			}
			break;
		}
		break;

	default:
		m_ReportFile.AddColumn ( Column.m_strText, Column.m_nAlign, Column.m_nWidth );
		m_csvUnderline.Add( "" );
		break;
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::AddReportColumnGroup( int nMasterColumn, int nBlock, CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn>& arraySubColumns )
{
	int nTotalWidth = 0;
	for ( int n = 0; n < arraySubColumns.GetSize(); n++ )
		nTotalWidth += arraySubColumns[n].m_nWidth;

	{
		COleDateTime dateFrom = ( 2 == nBlock ) ? m_oleDateFromBlock2 : m_oleDateFromBlock1;
		COleDateTime dateTo = ( 2 == nBlock ) ? m_oleDateToBlock2 : m_oleDateToBlock1;

		CString strBlockTitle;
		strBlockTitle.Format( "(%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d)",
			dateFrom.GetDay(),
			dateFrom.GetMonth(),
			dateFrom.GetYear(),
			dateTo.GetDay(),
			dateTo.GetMonth(),
			dateTo.GetYear() );

		m_ReportFile.AddColumn ( strBlockTitle, TA_CENTER, nTotalWidth );
	}

	for ( int n = 0; n < arraySubColumns.GetSize(); n++ )
	{
		CSalesHistoryReportPluColumn Column = arraySubColumns.GetAt(n);
		Column.m_nBlock = nBlock;
	
		if ( Column.m_nTotalType != TOTAL_TYPE_NONE )
		{
			if ( m_nFirstTotalColumn < 0 )
				m_nFirstTotalColumn = m_arrayReportColumns.GetSize();
		}

		m_arrayReportColumns.Add( Column );
		double dPercentWidth = ( ( (double) Column.m_nWidth ) * 100.0 ) / (double) nTotalWidth; 
		m_ReportFile.AddSubColumn ( nMasterColumn, Column.m_strText, Column.m_nAlign, dPercentWidth );
	
		switch( Column.m_nTotalType )
		{
		case TOTAL_TYPE_SALES:
		case TOTAL_TYPE_BASEQTY:
		case TOTAL_TYPE_MODQTY:
		case TOTAL_TYPE_DISCOUNT:
		case TOTAL_TYPE_PREMIUM:
		case TOTAL_TYPE_NET:
		case TOTAL_TYPE_AVERAGE:
		case TOTAL_TYPE_TAX:
		case TOTAL_TYPE_NETOFTAX:
		case TOTAL_TYPE_COST:
		case TOTAL_TYPE_WASTE:
		case TOTAL_TYPE_GP:
			m_csvUnderline.Add( "<LI>" );
			break;

		default:
			m_csvUnderline.Add( "" );
			break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::PrepareReportColumn( CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn>& arrayColumns, const char* szText, int nAlign, int nWidth, int nTotalType )
{
	arrayColumns.Add( CSalesHistoryReportPluColumn( szText, nAlign, nWidth, nTotalType ) );
}

/**********************************************************************/

bool CSalesHistoryReportPlu::CreateCustomReportInternal()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	m_nFirstTotalColumn = -1;
	m_csvUnderline.RemoveAll();
	m_arrayReportColumns.RemoveAll();

	bool bAllowValue = TRUE;
	bool bShowDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	bool bShowPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	bool bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
	
	if ( TRUE == m_bVIPSales )
	{
		if ( EcrmanOptions.GetReportsVIPType() == 2 )
		{
			bAllowValue = FALSE;
			bShowDiscountDetail = FALSE;
			bShowPremiumDetail = FALSE;
		}
	}

	bool bGotData = FALSE;

	CString strTitle = m_strReportName;
	::TrimSpaces( strTitle, FALSE );
	if (strTitle == "")
	{
		strTitle = "Custom Plu Sales History";
	}

	m_ReportFile.SetStyle1 ( GetReportTitle( strTitle ) );

	CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn> arrayItemColumns;
	CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn> arrayTotalColumns;

	if (SALESHISTORY_REPORTMODE_CHART == m_nReportMode)
	{
		PrepareReportColumn(arrayItemColumns, "Pos", TA_LEFT, 100);
	}

	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		switch ( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_MONTH:
			PrepareReportColumn( arrayItemColumns, "Month", TA_LEFT, 250 );	
			break;

		case SH_FIELD_WEEKDAY:	
			PrepareReportColumn( arrayItemColumns, "Weekday", TA_LEFT, 250 );	
			break;

		case SH_FIELD_DATE_BUSINESS:		
			PrepareReportColumn( arrayItemColumns, "Business Date", TA_LEFT, 280 );	
			break;

		case SH_FIELD_DATE_CALENDAR:		
			PrepareReportColumn( arrayItemColumns, "Calendar Date", TA_LEFT, 280 );	
			break;

		case SH_FIELD_TIME:		
			PrepareReportColumn( arrayItemColumns, "Time", TA_LEFT, 250 );	
			break;

		case SH_FIELD_SEQNO:		
			PrepareReportColumn( arrayItemColumns, "Seq No", TA_LEFT, 250 );	
			break;
		
		case SH_FIELD_DBASE:	
			PrepareReportColumn( arrayItemColumns, "Database", TA_LEFT, 300 );
			break;
		
		case SH_FIELD_LOC:		
			PrepareReportColumn( arrayItemColumns, "Location", TA_LEFT, 300 );
			break;
		
		case SH_FIELD_TERMINAL:	
			PrepareReportColumn( arrayItemColumns, "T.No", TA_LEFT, 150 );	
			break;

		case SH_FIELD_TABLENO:	
			PrepareReportColumn( arrayItemColumns, "Tbl.No", TA_LEFT, 150 );
			PrepareReportColumn( arrayItemColumns, "Table Name", TA_LEFT, 300 );
			break;

		case SH_FIELD_TABLEGRP:	
			PrepareReportColumn( arrayItemColumns, "Table Group", TA_LEFT, 300 );	
			break;
		
		case SH_FIELD_TRANSERVER:	
			PrepareReportColumn( arrayItemColumns, "Trn Svr", TA_LEFT, 200 );	
			PrepareReportColumn( arrayItemColumns, "Server Name", TA_LEFT, 350 );	
			break;

		case SH_FIELD_ITEMSERVER:	
			PrepareReportColumn( arrayItemColumns, "Item Svr", TA_LEFT, 200 );	
			PrepareReportColumn( arrayItemColumns, "Server Name", TA_LEFT, 350 );	
			break;

		case SH_FIELD_CUSTOMER:	
			PrepareReportColumn( arrayItemColumns, "Customer Type", TA_LEFT, 300 );
			PrepareReportColumn( arrayItemColumns, "Customer Name", TA_LEFT, 400 );	
			break;

		case SH_FIELD_PLUSUPP:	
			PrepareReportColumn( arrayItemColumns, "Supplier", TA_LEFT, 400 );	
			break;
		
		case SH_FIELD_WEEK:		
			PrepareReportColumn( arrayItemColumns, "Wk No", TA_LEFT, 150 );			
			PrepareReportColumn( arrayItemColumns, "Week Starting", TA_LEFT, 250 );	
			break;
		
		case SH_FIELD_DEPTNO_SALE:	
			PrepareReportColumn( arrayItemColumns, "Dpt", TA_LEFT, 100 );
			PrepareReportColumn( arrayItemColumns, "Dpt Name", TA_LEFT, 350 );
			break;

		case SH_FIELD_DEPTNO_ITEM:	
			PrepareReportColumn( arrayItemColumns, "Dpt", TA_LEFT, 100 );
			PrepareReportColumn( arrayItemColumns, "Dpt Name (Db)", TA_LEFT, 350 );
			break;

		case SH_FIELD_DEPTGRP_SALE:	
			PrepareReportColumn( arrayItemColumns, "Grp", TA_LEFT, 100 );
			PrepareReportColumn( arrayItemColumns, "Group Name", TA_LEFT, 350 );
			break;

		case SH_FIELD_DEPTGRP_ITEM:	
			PrepareReportColumn( arrayItemColumns, "Grp", TA_LEFT, 100 );
			PrepareReportColumn( arrayItemColumns, "Group Name (Db)", TA_LEFT, 350 );
			break;

		case SH_FIELD_REPGRP:	
			PrepareReportColumn( arrayItemColumns, "Rep.Grp", TA_LEFT, 180 );
			PrepareReportColumn( arrayItemColumns, "Rep.Grp Name", TA_LEFT, 350 );
			break;

		case SH_FIELD_CONGRP:	
			PrepareReportColumn( arrayItemColumns, "Con.Grp", TA_LEFT, 180 );
			PrepareReportColumn( arrayItemColumns, "Con.Grp Name", TA_LEFT, 350 );
			break;

		case SH_FIELD_ACAT_SALE:	
			PrepareReportColumn( arrayItemColumns, "Cat", TA_LEFT, 100 );
			PrepareReportColumn( arrayItemColumns, "Category Name", TA_LEFT, 350 );
			break;

		case SH_FIELD_ACAT_ITEM:	
			PrepareReportColumn( arrayItemColumns, "Cat (Db)", TA_LEFT, 100 );
			PrepareReportColumn( arrayItemColumns, "Category Name (Db)", TA_LEFT, 350 );
			break;

		case SH_FIELD_PLUNO:
			PrepareReportColumn( arrayItemColumns, "Plu Number", TA_LEFT, 350 );
			PrepareReportColumn( arrayItemColumns, "Description", TA_LEFT, 350 );
			break;

		case SH_FIELD_PLUNO_ONLY:
			PrepareReportColumn( arrayItemColumns, "Plu Number", TA_LEFT, 350 );
			break;

		case SH_FIELD_DESCRIPTION:
			PrepareReportColumn( arrayItemColumns, "Description", TA_LEFT, 350 );
			break;

		case SH_FIELD_CASHRSPTEXT:
			PrepareReportColumn( arrayItemColumns, "CashRSP Text", TA_LEFT, 350 );
			break;

		case SH_FIELD_ATTRIBUTETEXT1:
			PrepareReportColumn(arrayItemColumns, "Attribute1", TA_LEFT, 350);
			break;

		case SH_FIELD_SORTCODE:
			PrepareReportColumn( arrayItemColumns, DealerFlags.GetSortCodeLabel(), TA_LEFT, 350 );
			break;

		case SH_FIELD_LEVEL:
			PrepareReportColumn( arrayItemColumns, "P.Band", TA_LEFT, 200 );
			break;

		case SH_FIELD_TAXBAND:
			PrepareReportColumn( arrayItemColumns, EcrmanOptions.GetTaxBandString(), TA_LEFT, 250 );
			break;

		case SH_FIELD_MODIFIER:
			PrepareReportColumn( arrayItemColumns, "Modifier", TA_LEFT, 250 );
			break;

		case SH_FIELD_REPPOINT:
			PrepareReportColumn( arrayItemColumns, "Report Point", TA_LEFT, 300 );
			break;
		}
	}

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
		PrepareReportColumn( arrayTotalColumns,  "XX", TA_RIGHT, 250, TOTAL_TYPE_DAILY );
		break;
	}

	if ( m_bHasModifierField == FALSE )
	{
		PrepareReportColumn( arrayTotalColumns, "Quantity", TA_RIGHT, 250, TOTAL_TYPE_BASEQTY );
	}
	else
	{
		PrepareReportColumn( arrayTotalColumns, "Mod Qty", TA_RIGHT, 250, TOTAL_TYPE_MODQTY );
		PrepareReportColumn( arrayTotalColumns, "Plu Qty", TA_RIGHT, 250, TOTAL_TYPE_BASEQTY );
	}

	if ( FALSE == bAllowValue )
	{
		if ( TRUE == m_bEstimatedProfit )
		{
			PrepareReportColumn( arrayTotalColumns, "Est. Cost", TA_RIGHT, 250, TOTAL_TYPE_COST );
			PrepareReportColumn( arrayTotalColumns, "Est. Waste", TA_RIGHT, 250, TOTAL_TYPE_COST );
			PrepareReportColumn( arrayTotalColumns, "Est. GP", TA_RIGHT, 250, TOTAL_TYPE_GP );
			PrepareReportColumn( arrayTotalColumns, "% of Est. GP", TA_RIGHT, 250, TOTAL_TYPE_PERCENT_OF_GP );
		}
	}
	else
	{
		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
		case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:
			PrepareReportColumn( arrayTotalColumns, "XX", TA_RIGHT, 250, TOTAL_TYPE_DAILY );
			break;
		}

		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
		case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
			PrepareReportColumn( arrayTotalColumns, "Sales", TA_RIGHT, 250, TOTAL_TYPE_SALES );
			break;

		default:
			{	
				if ( FALSE == bShowDiscountDetail )
				{
					PrepareReportColumn( arrayTotalColumns, "Sales", TA_RIGHT, 250, TOTAL_TYPE_NET );
				}
				else
				{
					PrepareReportColumn( arrayTotalColumns, "Sales", TA_RIGHT, 250, TOTAL_TYPE_SALES );
					PrepareReportColumn( arrayTotalColumns, "Discount", TA_RIGHT, 250, TOTAL_TYPE_DISCOUNT );
					
					if (TRUE == bShowPremiumDetail)
					{						
						PrepareReportColumn(arrayTotalColumns, "Premium", TA_RIGHT, 250, TOTAL_TYPE_PREMIUM );
					}
					
					PrepareReportColumn( arrayTotalColumns, "Retail", TA_RIGHT, 250, TOTAL_TYPE_NET );
				}
			}
		}

		if ( TRUE == m_bEstimatedProfit )
		{
			PrepareReportColumn( arrayTotalColumns, EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250, TOTAL_TYPE_TAX );
		
			if ( TRUE == bNetOfTax )
			{
				PrepareReportColumn( arrayTotalColumns, "Est. Net", TA_RIGHT, 250, TOTAL_TYPE_NETOFTAX );
			}

			PrepareReportColumn( arrayTotalColumns, "Est. Cost", TA_RIGHT, 250, TOTAL_TYPE_COST );
			PrepareReportColumn( arrayTotalColumns, "Est. Waste", TA_RIGHT, 250, TOTAL_TYPE_WASTE );
			PrepareReportColumn( arrayTotalColumns, "Est. GP", TA_RIGHT, 250, TOTAL_TYPE_GP );
			PrepareReportColumn( arrayTotalColumns, "Est. GP%", TA_RIGHT, 250, TOTAL_TYPE_GP_PERCENT );
		}
		else 
		{
			PrepareReportColumn( arrayTotalColumns, "Average", TA_RIGHT, 250, TOTAL_TYPE_AVERAGE );
		}

		if ( TRUE == m_bPercentSale )
		{
			PrepareReportColumn( arrayTotalColumns, "%Sale", TA_RIGHT, 250, TOTAL_TYPE_PERCENT_OF_SALE );
		}

		if ( TRUE == m_bEstimatedProfit )
		{
			PrepareReportColumn( arrayTotalColumns, "% of Est. GP", TA_RIGHT, 250, TOTAL_TYPE_PERCENT_OF_GP );
		}
	}

	for (int n = 0; n < arrayItemColumns.GetSize(); n++)
	{
		AddReportColumn(arrayItemColumns.GetAt(n));
	}

	if ( SALESHISTORY_REPORTMODE_TWOBLOCK == m_nReportMode )
	{
		int nColumn = m_arrayReportColumns.GetSize();
		AddReportColumnGroup( nColumn, 1, arrayTotalColumns );
		AddReportColumnGroup( nColumn + 1, 2, arrayTotalColumns );
	}
	else
	{
		for (int n = 0; n < arrayTotalColumns.GetSize(); n++)
		{
			AddReportColumn(arrayTotalColumns.GetAt(n));
		}
	}

	m_nProgress = 0;
	m_nLabelTaxBand = 0;

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
		{
			CEntityTreePluSalesHistoryWeeklyOneTotalNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayWeeklyOneTotal( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;

	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
	{
		CEntityTreePluSalesHistoryWeeklyTwoTotalNode BlockTotalsNode, PercentTotalsNode;
		m_QtyValueTable.GetGrandTotalNode(BlockTotalsNode);
		ProcessQtyValueArrayWeeklyTwoTotal(0, 0, BlockTotalsNode, PercentTotalsNode);
	}
	break;

	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		{
			CEntityTreePluSalesHistoryMonthlyNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayMonthly( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;

	case SALESHISTORY_REPORTMODE_TWOBLOCK:
		{
			CEntityTreePluSalesHistoryTwoBlockNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayTwoBlock( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;

	case SALESHISTORY_REPORTMODE_NORMAL:
	case SALESHISTORY_REPORTMODE_CHART:
	default:
		{
			CEntityTreePluSalesHistoryNormalNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayNormal( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;
	}

	m_ReportFile.RequestBlankLines(1);
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportPlu::ProcessQtyValueArrayNormal( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryNormalNode BlockTotalsNode, CEntityTreePluSalesHistoryNormalNode PercentTotalsNode )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bHeader = ( m_arrayHeaderFlags.GetAt(nDepth) == 1 );
	bool bTotal = ( m_arrayTotalFlags.GetAt(nDepth) == 1 );
	
	if ( nDepth == m_nPercentDepth )
		PercentTotalsNode = BlockTotalsNode;

	int nArraySize = m_QtyValueTable.GetBranchSize(nBranchIdx);
	if ( nArraySize == 0 )
		return;

	if ( bHeader == TRUE )
		WriteHeader( nDepth );

	for ( int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_QtyValueTable.GetNodeCount() );
		ProcessQtyValueLine( nFieldType, nBranchIdx, nNodeIdx );

		CEntityTreePluSalesHistoryNormalNode CurrentNode;
		m_QtyValueTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLineNormal( CurrentNode, PercentTotalsNode );		
		else
			ProcessQtyValueArrayNormal( nDepth + 1, m_QtyValueTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode, PercentTotalsNode );
	}

	if ( bTotal == TRUE )
		WriteTotalsNormal( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CSalesHistoryReportPlu::ProcessQtyValueArrayTwoBlock( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryTwoBlockNode BlockTotalsNode, CEntityTreePluSalesHistoryTwoBlockNode PercentTotalsNode )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bHeader = ( m_arrayHeaderFlags.GetAt(nDepth) == 1 );
	bool bTotal = ( m_arrayTotalFlags.GetAt(nDepth) == 1 );
	
	if ( nDepth == m_nPercentDepth )
		PercentTotalsNode = BlockTotalsNode;

	int nArraySize = m_QtyValueTable.GetBranchSize(nBranchIdx);
	if ( nArraySize == 0 )
		return;

	if ( bHeader == TRUE )
		WriteHeader( nDepth );

	for ( int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_QtyValueTable.GetNodeCount() );
		ProcessQtyValueLine( nFieldType, nBranchIdx, nNodeIdx );

		CEntityTreePluSalesHistoryTwoBlockNode CurrentNode;
		m_QtyValueTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLineTwoBlock( CurrentNode, PercentTotalsNode );		
		else
			ProcessQtyValueArrayTwoBlock( nDepth + 1, m_QtyValueTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode, PercentTotalsNode );
	}

	if ( bTotal == TRUE )
		WriteTotalsTwoBlock( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CSalesHistoryReportPlu::ProcessQtyValueArrayWeeklyOneTotal( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryWeeklyOneTotalNode BlockTotalsNode, CEntityTreePluSalesHistoryWeeklyOneTotalNode PercentTotalsNode )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bHeader = ( m_arrayHeaderFlags.GetAt(nDepth) == 1 );
	bool bTotal = ( m_arrayTotalFlags.GetAt(nDepth) == 1 );
	
	if (nDepth == m_nPercentDepth)
	{
		PercentTotalsNode = BlockTotalsNode;
	}

	int nArraySize = m_QtyValueTable.GetBranchSize(nBranchIdx);
	if (nArraySize == 0)
	{
		return;
	}

	if (bHeader == TRUE)
	{
		WriteHeader(nDepth);
	}

	for (int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++)
	{
		StatusProgress.SetPos(m_nProgress++, m_QtyValueTable.GetNodeCount());
		ProcessQtyValueLine(nFieldType, nBranchIdx, nNodeIdx);

		CEntityTreePluSalesHistoryWeeklyOneTotalNode CurrentNode;
		m_QtyValueTable.GetNode(nBranchIdx, nNodeIdx, CurrentNode);

		if (nDepth == m_arrayReportFields.GetSize() - 1)
		{
			CreateReportLineWeeklyOneTotal(CurrentNode, PercentTotalsNode);
		}
		else
		{
			ProcessQtyValueArrayWeeklyOneTotal(nDepth + 1, m_QtyValueTable.GetNextBranchIdx(nBranchIdx, nNodeIdx), CurrentNode, PercentTotalsNode);
		}
	}

	if (bTotal == TRUE)
	{
		WriteTotalsWeeklyOneTotal(nDepth, BlockTotalsNode);
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::ProcessQtyValueArrayWeeklyTwoTotal(int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryWeeklyTwoTotalNode BlockTotalsNode, CEntityTreePluSalesHistoryWeeklyTwoTotalNode PercentTotalsNode)
{
	int nFieldType = m_arrayReportFields.GetAt(nDepth);
	bool bHeader = (m_arrayHeaderFlags.GetAt(nDepth) == 1);
	bool bTotal = (m_arrayTotalFlags.GetAt(nDepth) == 1);

	if (nDepth == m_nPercentDepth)
	{
		PercentTotalsNode = BlockTotalsNode;
	}

	int nArraySize = m_QtyValueTable.GetBranchSize(nBranchIdx);
	if (nArraySize == 0)
	{
		return;
	}

	if (bHeader == TRUE)
	{
		WriteHeader(nDepth);
	}

	for (int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++)
	{
		StatusProgress.SetPos(m_nProgress++, m_QtyValueTable.GetNodeCount());
		ProcessQtyValueLine(nFieldType, nBranchIdx, nNodeIdx);

		CEntityTreePluSalesHistoryWeeklyTwoTotalNode CurrentNode;
		m_QtyValueTable.GetNode(nBranchIdx, nNodeIdx, CurrentNode);

		if (nDepth == m_arrayReportFields.GetSize() - 1)
		{
			CreateReportLineWeeklyTwoTotal(CurrentNode, PercentTotalsNode);
		}
		else
		{
			ProcessQtyValueArrayWeeklyTwoTotal(nDepth + 1, m_QtyValueTable.GetNextBranchIdx(nBranchIdx, nNodeIdx), CurrentNode, PercentTotalsNode);
		}
	}

	if (bTotal == TRUE)
	{
		WriteTotalsWeeklyTwoTotal(nDepth, BlockTotalsNode);
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::ProcessQtyValueArrayMonthly( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryMonthlyNode BlockTotalsNode, CEntityTreePluSalesHistoryMonthlyNode PercentTotalsNode )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bHeader = ( m_arrayHeaderFlags.GetAt(nDepth) == 1 );
	bool bTotal = ( m_arrayTotalFlags.GetAt(nDepth) == 1 );
	
	if ( nDepth == m_nPercentDepth )
		PercentTotalsNode = BlockTotalsNode;

	int nArraySize = m_QtyValueTable.GetBranchSize(nBranchIdx);
	if ( nArraySize == 0 )
		return;

	if ( bHeader == TRUE )
		WriteHeader( nDepth );

	for ( int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_QtyValueTable.GetNodeCount() );
		ProcessQtyValueLine( nFieldType, nBranchIdx, nNodeIdx );

		CEntityTreePluSalesHistoryMonthlyNode CurrentNode;
		m_QtyValueTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLineMonthly( CurrentNode, PercentTotalsNode );		
		else
			ProcessQtyValueArrayMonthly( nDepth + 1, m_QtyValueTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode, PercentTotalsNode );
	}

	if ( bTotal == TRUE )
		WriteTotalsMonthly( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CSalesHistoryReportPlu::ProcessQtyValueLine( int nFieldType, int nBranchIdx, int nNodeIdx )
{
	int nLineDbNo = -1;

	switch ( nFieldType )
	{
	case SH_FIELD_MONTH:
		m_strLabelMonth = GetRepMonth( m_QtyValueTable.GetLabelDate( nBranchIdx, nNodeIdx ), FALSE, FALSE );	
		break;

	case SH_FIELD_WEEK:
		{	
			m_oleWeekStart = m_QtyValueTable.GetLabelDate( nBranchIdx, nNodeIdx );
			m_strLabelWeek = m_oleWeekStart.Format ( "%d/%m/%Y" );
			
			CDateRangeHelpers DateRangeHelpers;
			m_nLabelWeekNo = DateRangeHelpers.GetWeekNo( m_oleWeekStart );
		}
		break;

	case SH_FIELD_WEEKDAY:
		{
			int nWeekDay = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			nWeekDay += EcrmanOptions.GetReportsWeekStartDay();
			nWeekDay %= 7;
			m_strLabelWeekDay = m_strWeekDays [ nWeekDay ];
		}
		break;

	case SH_FIELD_DATE_BUSINESS:
		{	
			COleDateTime oleDate = m_QtyValueTable.GetLabelDate( nBranchIdx, nNodeIdx );
			m_strLabelBusinessDate = oleDate.Format ( "%d/%m/%Y" );
		}
		break;

	case SH_FIELD_DATE_CALENDAR:
		{	
			COleDateTime oleDate = m_QtyValueTable.GetLabelDate( nBranchIdx, nNodeIdx );
			m_strLabelCalendarDate = oleDate.Format ( "%d/%m/%Y" );
		}
		break;

	case SH_FIELD_TIME:
		{
			CString strLabel = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );

			if ( m_TimeSliceMap.GetReportType() != SH_CUSTOM )
			{
				m_nLabelTimeHour = atoi( strLabel.Mid(1,2) );
				m_nLabelTimeMinute = atoi( strLabel.Mid(3,2) );
				m_nLabelTimeSecond = atoi( strLabel.Right(2) );
				m_nLabelTimeStars = atoi( strLabel.Left(1) );
			}
			else
				m_nLabelTimeHour = atoi( strLabel );
		}
		break;

	case SH_FIELD_SEQNO:
		m_nLabelSeqNo = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_DBASE:
		{
			int nSortIdx = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			int nDbIdx = dbDatabase.GetDbIdxFromSortIdx( nSortIdx );
			
			if ( ( nDbIdx >= 0 ) && ( nDbIdx < dbDatabase.GetSize() ) )
			{
				m_strLabelDBase = dbDatabase.GetName( nDbIdx );
				nLineDbNo = dbDatabase.GetDbNo( nDbIdx );
			}
			else
				m_strLabelDBase = "";
		}
		break;

	case SH_FIELD_LOC:
		{
			int nSortIdx = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			int nLocIdx = dbLocation.GetLocIdxFromSortIdx( nSortIdx );

			switch( nLocIdx )
			{
			case LOCIDX_SPECIAL_MANUAL:		m_strLabelLoc = "Manual";	break;
			case LOCIDX_SPECIAL_IMPORT:		m_strLabelLoc = "Import";	break;
			case LOCIDX_SPECIAL_UNKNOWN:	m_strLabelLoc = "";			break;
			default:		
				{
					m_strLabelLoc = dbLocation.GetName( nLocIdx ); 
					nLineDbNo = dbLocation.GetDbNo( nLocIdx );
				}
				break;
			}

			if ( TRUE == m_bWantTableName )
			{
				m_TableReportHelpers.SetLocIdx( nLocIdx );
				m_TableReportHelpers.BuildGroupMap( m_TableFilter );
			}
		}
		break;

	case SH_FIELD_TERMINAL:
		{
			int nTerminalNo = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			
			if ( nTerminalNo != 0 )
				m_strLabelTerminal.Format ( "TNo %d", nTerminalNo );
			else
				m_strLabelTerminal = "";
		}
		break;

	case SH_FIELD_TABLENO:
		m_nLabelTableNo = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		m_strLabelTableName = m_TableReportHelpers.GetTableName( m_nLabelTableNo );
		break;

	case SH_FIELD_TABLEGRP:
		m_strLabelTableGrpName = m_TableReportHelpers.GetGroupName( m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx ) );
		break;

	case SH_FIELD_REPPOINT:
		{
			int nRepPoint = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			
			if ( ( nRepPoint < 1 ) || ( nRepPoint > dbReportpoint.GetSize() ) )
				m_strLabelRepPoint = "No Report Point";
			else
			{
				int nRpIdx = dbReportpoint.GetRpIdxFromSortIdx( nRepPoint - 1 );
				m_strLabelRepPoint = dbReportpoint.GetName( nRpIdx );
			}
		}
		break;

	case SH_FIELD_TRANSERVER:
		{
			CString strLabel = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
			DataManagerNonDb.ServerNameTable.ProcessServerLabel( strLabel, m_nLabelTranServerNo, m_strLabelTranServerName );
		}
		break;

	case SH_FIELD_ITEMSERVER:
		{
			CString strLabel = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
			DataManagerNonDb.ServerNameTable.ProcessServerLabel( strLabel, m_nLabelItemServerNo, m_strLabelItemServerName );
		}
		break;

	case SH_FIELD_CUSTOMER:
		{
			CString strLabel = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );

			int nLen = strLabel.GetLength();

			if ( ( 1 == nLen ) &&( strLabel == "3" )  )
			{
				m_strLabelCustomerType = "N/A";
				m_strLabelCustomerName = "Non Customer Sale";
			}
			else if ( ( 17 == nLen ) && ( strLabel.Left(1) == "0" ) )
			{
				int nIdx = atoi( strLabel.Right(8) );
				CCustomerSelectionListAccountInfo infoCustomer;
				DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nIdx, infoCustomer );
				m_strLabelCustomerType = "Account";
				m_strLabelCustomerName = infoCustomer.GetReportName();
			}
			else if ( ( 17 == nLen ) && ( strLabel.Left(1) == "1" ) )
			{
				int nIdx = atoi( strLabel.Right(8) );
				CCustomerSelectionListAccountInfo infoCustomer;
				DataManagerNonDb.CustomerNameTable.OneOff.GetCustomerStatus( nIdx, infoCustomer );
				m_strLabelCustomerType = "One-off";
				m_strLabelCustomerName = infoCustomer.GetReportName();
			}
			else if ( ( 9 == nLen ) && ( strLabel.Left(1) == "2" ) )
			{
				m_strLabelCustomerType = "Unknown";
				m_strLabelCustomerName.Format( "Customer %d", atoi( strLabel.Right(8) ) );
			}
			else
			{
				m_strLabelCustomerType = "Unknown";
				m_strLabelCustomerName = "Unknown Customer";
			}
		}
		break;

#ifndef STOCKMAN_SYSTEM
	case SH_FIELD_PLUSUPP:
		{
			int nPluSupp = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );

			if ( 0 == nPluSupp )
				m_strLabelPluSupp = "No Supplier";
			else
			{
				int nSuppIdx;
				if ( DataManagerNonDb.EcrmanSupplier.FindSupplierByNumber( nPluSupp, nSuppIdx ) == TRUE )
				{
					CEcrmanSupplierCSVRecord SuppRecord;
					DataManagerNonDb.EcrmanSupplier.GetAt( nSuppIdx, SuppRecord );
					
					m_strLabelPluSupp = SuppRecord.GetSuppName();
					::TrimSpaces( m_strLabelPluSupp, FALSE );

					if ( m_strLabelPluSupp == "" )
						m_strLabelPluSupp = SuppRecord.GetDefaultName();
				}
				else
					m_strLabelPluSupp.Format( "Supplier %d", nPluSupp );
			}
		}
		break;
#endif

	case SH_FIELD_DEPTNO_SALE:
		m_nLabelDeptNoSale = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		if ( 1000 == m_nLabelDeptNoSale ) m_nLabelDeptNoSale = 0;
		m_strLabelDeptNameSale = DataManager.Department.GetReportTextByDeptNo( m_nLabelDeptNoSale );
		break;

	case SH_FIELD_DEPTNO_ITEM:
		m_nLabelDeptNoItem = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		if ( 1000 == m_nLabelDeptNoItem ) m_nLabelDeptNoItem = 0;
		m_strLabelDeptNameItem = DataManager.Department.GetReportTextByDeptNo( m_nLabelDeptNoItem );
		break;

	case SH_FIELD_DEPTGRP_SALE:
		m_nLabelDeptGrpNoSale = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		if ( 1000 == m_nLabelDeptGrpNoSale ) m_nLabelDeptGrpNoSale = 0;
		m_strLabelDeptGrpNameSale = DataManager.EposGroup.GetGroupReportText( m_nLabelDeptGrpNoSale );
		break;

	case SH_FIELD_DEPTGRP_ITEM:
		m_nLabelDeptGrpNoItem = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		if ( 1000 == m_nLabelDeptGrpNoItem ) m_nLabelDeptGrpNoItem = 0;
		m_strLabelDeptGrpNameItem = DataManager.EposGroup.GetGroupReportText( m_nLabelDeptGrpNoItem );
		break;

	case SH_FIELD_REPGRP:
		{
			int nGrpNo = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );

			switch( nGrpNo )
			{
			case 0:
				m_nLabelRepGrpNo = -1;
				m_strLabelRepGrpName = "None";
				break;

			case 1000:
				m_nLabelRepGrpNo = 0;
				m_strLabelRepGrpName = "Promo";
				break;

			default:
				m_nLabelRepGrpNo = nGrpNo;
				m_strLabelRepGrpName = DataManager.ReportGroup.GetGroupReportText( nGrpNo );
				break;
			}
		}
		break;

	case SH_FIELD_CONGRP:
		{
			int nGrpNo = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );

			switch( nGrpNo )
			{
			case 0:
				m_nLabelConGrpNo = -1;
				m_strLabelConGrpName = "None";
				break;

			case 1000:
				m_nLabelConGrpNo = 0;
				m_strLabelConGrpName = "Promo";
				break;

			default:
				m_nLabelConGrpNo = nGrpNo;
				m_strLabelConGrpName = DataManager.ConsolGroup.GetGroupReportText( nGrpNo );
				break;
			}
		}
		break;

	case SH_FIELD_ACAT_SALE:
		m_nLabelACatNoSale = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		m_strLabelACatSale = DataManager.AnalysisCategory.GetDisplayText( m_nLabelACatNoSale );
		break;

	case SH_FIELD_ACAT_ITEM:
		m_nLabelACatNoItem = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		m_strLabelACatItem = DataManager.AnalysisCategory.GetDisplayText( m_nLabelACatNoItem );
		break;

	case SH_FIELD_PLUNO:
	case SH_FIELD_PLUNO_ONLY:
		m_nLabelPluNo = m_QtyValueTable.GetLabelInt64( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_DESCRIPTION:
		m_strLabelDescription = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_CASHRSPTEXT:
		m_strLabelCashRSPText = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_ATTRIBUTETEXT1:
		m_strLabelAttributeText1 = m_QtyValueTable.GetLabelString(nBranchIdx, nNodeIdx);
		break;

	case SH_FIELD_SORTCODE:
		m_strLabelSortCode = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
		break;
		
	case SH_FIELD_LEVEL:
		m_nLabelLevel = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_TAXBAND:
		m_nLabelTaxBand = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_MODIFIER:
		m_nLabelModifier = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
		break;
	}

	if ( (nLineDbNo != -1) && ( nLineDbNo != dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) ) )
	{
		int nDbIdx;
		if ( dbDatabase.FindDatabaseByNumber( nLineDbNo, nDbIdx ) == TRUE )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::CreateReportLineNormal( CEntityTreePluSalesHistoryNormalNode CurrentNode, CEntityTreePluSalesHistoryNormalNode PercentTotalsNode )
{
	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );
	AppendReportLineFields( csvOut );
	AppendTotalsNormal( csvOut, CurrentNode, PercentTotalsNode, FALSE );

	if ( SALESHISTORY_REPORTMODE_CHART != m_nReportMode )
	{
		m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
	}
	else
	{
		double dSalesBaseQty = CurrentNode.m_SalesBlock.GetBaseQty();
		double dSalesVal = CurrentNode.m_SalesBlock.GetTotalVal();
		double dSalesDiscountVal = CurrentNode.m_SalesBlock.GetDiscountVal();
		double dSalesPremiumVal = CurrentNode.m_SalesBlock.GetPremiumVal();
		double dSalesNet = dSalesVal - dSalesDiscountVal + dSalesPremiumVal;
		AddToChart( dSalesBaseQty, dSalesNet, csvOut.GetLine() );
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::CreateReportLineTwoBlock( CEntityTreePluSalesHistoryTwoBlockNode CurrentNode, CEntityTreePluSalesHistoryTwoBlockNode PercentTotalsNode )
{
	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );
	AppendReportLineFields( csvOut );
	AppendTotalsTwoBlock( csvOut, CurrentNode, PercentTotalsNode, FALSE );
	m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
}

/**********************************************************************/

void CSalesHistoryReportPlu::CreateReportLineWeeklyOneTotal( CEntityTreePluSalesHistoryWeeklyOneTotalNode CurrentNode, CEntityTreePluSalesHistoryWeeklyOneTotalNode PercentTotalsNode )
{
	int nWeeklyStartOffset = m_QtyValueTable.GetWeeklyStartOffset( m_oleWeekStart );
	int nWeeklyEndOffset = m_QtyValueTable.GetWeeklyEndOffset( m_oleWeekStart );

	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );
	AppendReportLineFields( csvOut );
	AppendTotalsNormal( csvOut, CurrentNode, PercentTotalsNode, FALSE, nWeeklyStartOffset, nWeeklyEndOffset );
	m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
}

/**********************************************************************/

void CSalesHistoryReportPlu::CreateReportLineWeeklyTwoTotal(CEntityTreePluSalesHistoryWeeklyTwoTotalNode CurrentNode, CEntityTreePluSalesHistoryWeeklyTwoTotalNode PercentTotalsNode)
{
	int nWeeklyStartOffset = m_QtyValueTable.GetWeeklyStartOffset(m_oleWeekStart);
	int nWeeklyEndOffset = m_QtyValueTable.GetWeeklyEndOffset(m_oleWeekStart);

	CCSV csvOut('\t', '"', TRUE, FALSE, FALSE);
	AppendReportLineFields(csvOut);
	AppendTotalsNormal(csvOut, CurrentNode, PercentTotalsNode, FALSE, nWeeklyStartOffset, nWeeklyEndOffset);
	m_ReportFile.WriteReportDataLine(csvOut.GetLine());
}

/**********************************************************************/

void CSalesHistoryReportPlu::CreateReportLineMonthly( CEntityTreePluSalesHistoryMonthlyNode CurrentNode, CEntityTreePluSalesHistoryMonthlyNode PercentTotalsNode )
{
	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );
	AppendReportLineFields( csvOut );
	AppendTotalsNormal( csvOut, CurrentNode, PercentTotalsNode, FALSE );
	m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
}

/**********************************************************************/

void CSalesHistoryReportPlu::AppendReportLineFields( CCSV& csvOut )
{
	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		switch ( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_MONTH:			csvOut.Add( m_strLabelMonth );			break;
		case SH_FIELD_WEEKDAY:			csvOut.Add( m_strLabelWeekDay );		break;
		case SH_FIELD_DATE_BUSINESS:	csvOut.Add( m_strLabelBusinessDate );	break;
		case SH_FIELD_DATE_CALENDAR:	csvOut.Add( m_strLabelCalendarDate );	break;
		case SH_FIELD_TIME:				csvOut.Add( GetLabelTimeString() );		break;
		case SH_FIELD_SEQNO:			csvOut.Add( m_nLabelSeqNo );			break;
		case SH_FIELD_DBASE:			csvOut.Add( m_strLabelDBase );			break;
		case SH_FIELD_LOC:				csvOut.Add( m_strLabelLoc );			break;
		case SH_FIELD_TERMINAL:			csvOut.Add( m_strLabelTerminal );		break;
		case SH_FIELD_REPPOINT:			csvOut.Add( m_strLabelRepPoint );		break;

		case SH_FIELD_TABLENO:			
			csvOut.Add( m_nLabelTableNo );
			csvOut.Add( m_strLabelTableName );
			break;

		case SH_FIELD_TABLEGRP:			
			csvOut.Add( m_strLabelTableGrpName );
			break;
	
		case SH_FIELD_WEEK:			
			csvOut.Add( m_nLabelWeekNo );
			csvOut.Add( m_strLabelWeek );
			break;
			
		case SH_FIELD_DEPTNO_SALE:		
			csvOut.Add( m_nLabelDeptNoSale );		
			csvOut.Add( m_strLabelDeptNameSale );
			break;

		case SH_FIELD_DEPTNO_ITEM:		
			csvOut.Add( m_nLabelDeptNoItem );		
			csvOut.Add( m_strLabelDeptNameItem );
			break;

		case SH_FIELD_DEPTGRP_SALE:		
			csvOut.Add( m_nLabelDeptGrpNoSale );		
			csvOut.Add( m_strLabelDeptGrpNameSale );
			break;

		case SH_FIELD_DEPTGRP_ITEM:		
			csvOut.Add( m_nLabelDeptGrpNoItem );		
			csvOut.Add( m_strLabelDeptGrpNameItem );
			break;

		case SH_FIELD_REPGRP:		
			if ( m_nLabelRepGrpNo >= 0 )
				csvOut.Add( m_nLabelRepGrpNo );	
			else
				csvOut.Add( "" );

			csvOut.Add( m_strLabelRepGrpName );
			break;

		case SH_FIELD_CONGRP:	
			if ( m_nLabelConGrpNo >= 0 )
				csvOut.Add( m_nLabelConGrpNo );	
			else
				csvOut.Add( "" );
			csvOut.Add( m_strLabelConGrpName );
			break;

		case SH_FIELD_ACAT_SALE:		
			csvOut.Add( m_nLabelACatNoSale );		
			csvOut.Add( m_strLabelACatSale );
			break;

		case SH_FIELD_ACAT_ITEM:		
			csvOut.Add( m_nLabelACatNoItem );		
			csvOut.Add( m_strLabelACatItem );
			break;

		case SH_FIELD_TRANSERVER:		
			csvOut.Add( m_nLabelTranServerNo );		
			csvOut.Add( m_strLabelTranServerName );
			break;

		case SH_FIELD_ITEMSERVER:		
			csvOut.Add( m_nLabelItemServerNo );		
			csvOut.Add( m_strLabelItemServerName );
			break;

		case SH_FIELD_CUSTOMER:			
			csvOut.Add( m_strLabelCustomerType );
			csvOut.Add( m_strLabelCustomerName );
			break;

		case SH_FIELD_PLUSUPP:			
			csvOut.Add( m_strLabelPluSupp );
			break;

		case SH_FIELD_PLUNO:
			{
				m_PluInfoFinder.SetPluNo( m_nLabelPluNo );
				csvOut.Add( ReportHelpers.GetDisplayPluNo( m_nLabelPluNo, TRUE ) );
				csvOut.Add( m_PluInfoFinder.GetPluInfoRepText(TRUE) );
			}
			break;

		case SH_FIELD_PLUNO_ONLY:
			csvOut.Add( ReportHelpers.GetDisplayPluNo ( m_nLabelPluNo, TRUE ) );
			break;

		case SH_FIELD_DESCRIPTION:
			{
				int nLen = m_strLabelDescription.GetLength();
				if (nLen >= 2)
					csvOut.Add(m_strLabelDescription.Right(nLen - 1));
				else
					csvOut.Add("");
			}
			break;

		case SH_FIELD_CASHRSPTEXT:
			csvOut.Add( m_strLabelCashRSPText );
			break;

		case SH_FIELD_ATTRIBUTETEXT1:
		{
			CString strText = m_strLabelAttributeText1;

			if (strText == "")
			{
				strText = "N/A";
			}

			csvOut.Add(strText);
		}
		break;

		case SH_FIELD_SORTCODE:
			{
				int nLen = m_strLabelSortCode.GetLength();
				if ( nLen >= 2 )
					csvOut.Add( m_strLabelSortCode.Right( nLen - 1 ) );
				else
					csvOut.Add( "" );
			}
			break;
			
		case SH_FIELD_LEVEL:
			csvOut.Add ( DataManager.SBPriceText.GetDisplayText( m_nLabelLevel - 1 ) );
			break;

		case SH_FIELD_TAXBAND:
			{
				CTaxRateInfo TaxRateInfo;
				DataManager.TaxRates.GetTaxRateInfo( m_nLabelTaxBand, TaxRateInfo );
				csvOut.Add( TaxRateInfo.m_strReportText );
			}
			break;

		case SH_FIELD_MODIFIER:
			{
				int nMod = m_nLabelModifier;
				csvOut.Add ( ( nMod > 0 ) ? DataManager.Modifier.GetDisplayName( nMod ) : "none" );
			}
			break;
		}
	}
}

/**********************************************************************/

const char* CSalesHistoryReportPlu::GetHeaderLine( int nDepth )
{
	CString strSpace = " ";
	CCSV csvHeader;
	
	for ( int nPos = 0; nPos < nDepth; nPos++ )
	{
		int nFieldType = m_arrayReportFields.GetAt( nPos );
		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:
			csvHeader.Add ( strSpace + m_strLabelMonth );
			break;

		case SH_FIELD_WEEK:
			{
				CString strWeekNo;
				strWeekNo.Format ( "Week %d", m_nLabelWeekNo );
				csvHeader.Add ( strSpace + strWeekNo );
				csvHeader.Add ( strSpace + m_strLabelWeek );
			}
			break;

		case SH_FIELD_WEEKDAY:
			csvHeader.Add ( strSpace + m_strLabelWeekDay );
			break;

		case SH_FIELD_DATE_CALENDAR:
			csvHeader.Add ( strSpace + m_strLabelCalendarDate );
			break;

		case SH_FIELD_DATE_BUSINESS:
			csvHeader.Add ( strSpace + m_strLabelBusinessDate );
			break;

		case SH_FIELD_TIME:
			csvHeader.Add ( strSpace + GetLabelTimeString() );
			break;

		case SH_FIELD_SEQNO:
			{
				CString strSeqNo = "";
				strSeqNo.Format( "%d", m_nLabelSeqNo );
				csvHeader.Add ( strSpace + strSeqNo );
			}
			break;

		case SH_FIELD_DBASE:
			csvHeader.Add ( strSpace + m_strLabelDBase );
			break;

		case SH_FIELD_LOC:
			csvHeader.Add ( strSpace + m_strLabelLoc );
			break;

		case SH_FIELD_TERMINAL:
			csvHeader.Add ( strSpace + m_strLabelTerminal );
			break;

		case SH_FIELD_TABLENO:
			csvHeader.Add ( strSpace + m_strLabelTableName );
			break;

		case SH_FIELD_TABLEGRP:
			csvHeader.Add ( strSpace + m_strLabelTableGrpName );
			break;

		case SH_FIELD_REPPOINT:
			csvHeader.Add ( strSpace + m_strLabelRepPoint );
			break;

		case SH_FIELD_TRANSERVER:
			{
				CString strServer;
				strServer.Format ( "Tran Server S%4.4d:", m_nLabelTranServerNo );
				csvHeader.Add ( strSpace + strServer + m_strLabelTranServerName );
			}
			break;

		case SH_FIELD_ITEMSERVER:
			{
				CString strServer;
				strServer.Format ( "Item Server S%4.4d:", m_nLabelItemServerNo );
				csvHeader.Add ( strSpace + strServer + m_strLabelItemServerName );
			}
			break;

		case SH_FIELD_CUSTOMER:
			{
				csvHeader.Add ( strSpace + m_strLabelCustomerName );
			}
			break;

		case SH_FIELD_PLUSUPP:
			{
				csvHeader.Add ( strSpace + m_strLabelPluSupp );
			}
			break;

		case SH_FIELD_DEPTNO_SALE:
			{
				CString strDeptNo;
				strDeptNo.Format ( "D%3.3d:", m_nLabelDeptNoSale );
				csvHeader.Add( strSpace + strDeptNo + m_strLabelDeptNameSale );
			}
			break;

		case SH_FIELD_DEPTNO_ITEM:
			{
				CString strDeptNo;
				strDeptNo.Format ( "D%3.3d:", m_nLabelDeptNoItem );
				csvHeader.Add( strSpace + strDeptNo + m_strLabelDeptNameItem );
			}
			break;

		case SH_FIELD_DEPTGRP_SALE:
			{
				CString strDeptGrpNo;
				strDeptGrpNo.Format ( "G%2.2d:", m_nLabelDeptGrpNoSale );
				csvHeader.Add( strSpace + strDeptGrpNo + m_strLabelDeptGrpNameSale );
			}
			break;

		case SH_FIELD_DEPTGRP_ITEM:
			{
				CString strDeptGrpNo;
				strDeptGrpNo.Format ( "G%2.2d:", m_nLabelDeptGrpNoItem );
				csvHeader.Add( strSpace + strDeptGrpNo + m_strLabelDeptGrpNameItem );
			}
			break;

		case SH_FIELD_REPGRP:
			{
				if ( m_nLabelRepGrpNo >= 0 )
				{
					CString strRepGrpNo;
					strRepGrpNo.Format ( "G%2.2d:", m_nLabelRepGrpNo );
					csvHeader.Add( strSpace + strRepGrpNo + m_strLabelRepGrpName );
				}
				else
					csvHeader.Add( strSpace + "No Rep.Grp" );
			}
			break;

		case SH_FIELD_CONGRP:
			{
				if ( m_nLabelConGrpNo >= 0 )
				{
					CString strConGrpNo;
					strConGrpNo.Format ( "G%2.2d:", m_nLabelConGrpNo );
					csvHeader.Add( strSpace + strConGrpNo + m_strLabelConGrpName );
				}
				else
					csvHeader.Add( strSpace + "No Con.Grp" );
			}
			break;

		case SH_FIELD_ACAT_SALE:
			{
				CString strCatNo;
				strCatNo.Format ( "C%2.2d:", m_nLabelACatNoSale );
				csvHeader.Add( strSpace + strCatNo + m_strLabelACatSale );
			}
			break;

		case SH_FIELD_ACAT_ITEM:
			{
				CString strCatNo;
				strCatNo.Format ( "C%2.2d:", m_nLabelACatNoItem );
				csvHeader.Add( strSpace + strCatNo + m_strLabelACatItem );
			}
			break;

		case SH_FIELD_PLUNO:
		case SH_FIELD_PLUNO_ONLY:
			csvHeader.Add ( strSpace + ReportHelpers.GetDisplayPluNo ( m_nLabelPluNo, TRUE ) );
			break;

		case SH_FIELD_DESCRIPTION:
			{
				int nLen = m_strLabelDescription.GetLength();
				if ( nLen >= 2 )
					csvHeader.Add( strSpace + m_strLabelDescription.Right( nLen - 1 ) );
				else
					csvHeader.Add( "" );
			}
			break;

		case SH_FIELD_CASHRSPTEXT:
			csvHeader.Add( strSpace + m_strLabelCashRSPText );
			break;

		case SH_FIELD_ATTRIBUTETEXT1:
		{
			CString strText = m_strLabelAttributeText1;

			if (strText == "")
			{
				strText = "N/A";
			}

			csvHeader.Add(strSpace + strText);
		}
		break;

		case SH_FIELD_SORTCODE:
			{
				int nLen = m_strLabelSortCode.GetLength();
				if ( nLen >= 2 )
					csvHeader.Add( strSpace + m_strLabelSortCode.Right( nLen - 1 ) );
				else
					csvHeader.Add( "" );
			}
			break;

		case SH_FIELD_LEVEL:
			{
				CString strLevel = DataManager.PriceText.GetReportText( m_nLabelLevel - 1 );
				csvHeader.Add ( strSpace + strLevel );
			}
			break;

		case SH_FIELD_TAXBAND:
			{
				CTaxRateInfo TaxRateInfo;
				DataManager.TaxRates.GetTaxRateInfo( m_nLabelTaxBand, TaxRateInfo );
				csvHeader.Add ( strSpace + TaxRateInfo.m_strReportText );
			}
			break;

		case SH_FIELD_MODIFIER:	
			{
				int nMod = m_nLabelModifier;

				if ( nMod <= 0 )
					csvHeader.Add ( strSpace + "No modifier" );
				else
					csvHeader.Add ( strSpace + DataManager.Modifier.GetDisplayName( nMod ) );
			}
			break;
		}
	}
	m_strHeader = csvHeader.GetLine();
	return m_strHeader;
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteTotalsNormal( int nDepth, CEntityTreePluSalesHistoryNormalNode& BlockTotalsNode )
{
	//DISPLAY TOP/BOTTOM SALES AND CLEAR BUFFER
	if ( SALESHISTORY_REPORTMODE_CHART == m_nReportMode )
	{
		for ( int nIndex = 0; nIndex < m_arrayBufferedLines.GetSize(); nIndex++ )
		{
			if ( m_chartinfo.GetChartType() == CHART_TYPE_CUTOFF )
			{
				double dLineValue = m_arraySalesChart.GetAt( nIndex );
				double dCutOff = m_chartinfo.GetChartCutOff();

				if ( m_chartinfo.IsBottom100() )
					dCutOff = -dCutOff;
				
				if ( dLineValue < dCutOff )
					break;
			}
		
			CCSV line ( m_arrayBufferedLines.GetAt( nIndex ), '\t', '"', 20 );
			line.InsertAt( 0, nIndex + 1 );
			m_ReportFile.WriteReportMiscLine( line.GetLine() );
		}
		m_arrayBufferedLines.RemoveAll();
		m_arraySalesChart.RemoveAll();
		return;
	}

	//CREATE UNDERLINE FOR TOTALS
	m_ReportFile.WriteReportMiscLine( m_csvUnderline.GetLine() );
	
	//GET THE TOTAL DESCRIPTION AND QTY/VALUE ETC
	CString strTotal;
	if ( nDepth == 0 )
		strTotal = "<..>Grand Total";
	else
	{
		strTotal = "<..>Totals (";
		strTotal += GetHeaderLine ( nDepth );
		strTotal += " )";
	}

	CEntityTreePluSalesHistoryNormalNode DummyTotalsNode;
	
	CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );
	csvOut.Add ( strTotal );

	AppendTotalsNormal( csvOut, BlockTotalsNode, DummyTotalsNode, TRUE );

	m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
	m_ReportFile.RequestBlankLines(1);
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteTotalsTwoBlock( int nDepth, CEntityTreePluSalesHistoryTwoBlockNode& BlockTotalsNode )
{
	//CREATE UNDERLINE FOR TOTALS
	m_ReportFile.WriteReportMiscLine( m_csvUnderline.GetLine() );
	
	//GET THE TOTAL DESCRIPTION AND QTY/VALUE ETC
	CString strTotal;
	if (nDepth == 0)
	{
		strTotal = "<..>Grand Total";
	}
	else
	{
		strTotal = "<..>Totals (";
		strTotal += GetHeaderLine ( nDepth );
		strTotal += " )";
	}

	CEntityTreePluSalesHistoryTwoBlockNode DummyTotalsNode;
	
	CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );
	csvOut.Add ( strTotal );

	AppendTotalsTwoBlock( csvOut, BlockTotalsNode, DummyTotalsNode, TRUE );

	m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
	m_ReportFile.RequestBlankLines(1);
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteTotalsWeeklyOneTotal( int nDepth, CEntityTreePluSalesHistoryWeeklyOneTotalNode& BlockTotalsNode )
{
	//CREATE UNDERLINE FOR TOTALS
	m_ReportFile.WriteReportMiscLine( m_csvUnderline.GetLine() );
	
	//GET THE TOTAL DESCRIPTION AND QTY/VALUE ETC
	CString strTotal = "";
	if (nDepth == 0)
	{
		strTotal = "<..>Grand Total";
	}
	else
	{
		strTotal = "<..>Totals (";
		strTotal += GetHeaderLine ( nDepth );
		strTotal += " )";
	}

	CEntityTreePluSalesHistoryWeeklyOneTotalNode DummyTotalsNode;
	
	CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );
	csvOut.Add ( strTotal );

	AppendTotalsNormal( csvOut, BlockTotalsNode, DummyTotalsNode, TRUE );

	m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
	m_ReportFile.RequestBlankLines(1);
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteTotalsWeeklyTwoTotal(int nDepth, CEntityTreePluSalesHistoryWeeklyTwoTotalNode& BlockTotalsNode)
{
	//CREATE UNDERLINE FOR TOTALS
	m_ReportFile.WriteReportMiscLine(m_csvUnderline.GetLine());

	//GET THE TOTAL DESCRIPTION AND QTY/VALUE ETC
	CString strTotal = "";
	if (nDepth == 0)
	{
		strTotal = "<..>Grand Total";
	}
	else
	{
		strTotal = "<..>Totals (";
		strTotal += GetHeaderLine(nDepth);
		strTotal += " )";
	}

	CEntityTreePluSalesHistoryWeeklyTwoTotalNode DummyTotalsNode;

	CCSV csvOut('\t', '"', TRUE, FALSE, FALSE);
	csvOut.Add(strTotal);

	AppendTotalsNormal(csvOut, BlockTotalsNode, DummyTotalsNode, TRUE);

	m_ReportFile.WriteReportMiscLine(csvOut.GetLine());
	m_ReportFile.RequestBlankLines(1);
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteTotalsMonthly( int nDepth, CEntityTreePluSalesHistoryMonthlyNode& BlockTotalsNode )
{
	//CREATE UNDERLINE FOR TOTALS
	m_ReportFile.WriteReportMiscLine( m_csvUnderline.GetLine() );
	
	//GET THE TOTAL DESCRIPTION AND QTY/VALUE ETC
	CString strTotal;
	if (nDepth == 0)
	{
		strTotal = "<..>Grand Total";
	}
	else
	{
		strTotal = "<..>Totals (";
		strTotal += GetHeaderLine ( nDepth );
		strTotal += " )";
	}

	CEntityTreePluSalesHistoryMonthlyNode DummyTotalsNode;
	
	CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );
	csvOut.Add ( strTotal );

	AppendTotalsNormal( csvOut, BlockTotalsNode, DummyTotalsNode, TRUE );

	m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
	m_ReportFile.RequestBlankLines(1);
}

/**********************************************************************/

void CSalesHistoryReportPlu::AppendTotalsNormal( CCSV& csv, CEntityTreePluSalesHistoryNormalNode& LineTotals, CEntityTreePluSalesHistoryNormalNode& BlockTotals, bool bTotals, int nStartOffset, int nEndOffset )
{
	if (m_nFirstTotalColumn >= 0)
	{
		int nDailyColStart = -1;
		int nDailyColCount = LineTotals.GetDailyColCount();
		AppendDateBlockTotals(csv, 1, &LineTotals.m_SalesBlock, &BlockTotals.m_SalesBlock, bTotals, nDailyColStart, nDailyColCount, nStartOffset, nEndOffset);

		if (nDailyColStart >= 0)
		{
			LineTotals.AppendDaily(m_nReportMode, csv, nDailyColStart, nStartOffset, nEndOffset);
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::AppendTotalsTwoBlock( CCSV& csv, CEntityTreePluSalesHistoryTwoBlockNode& LineTotals, CEntityTreePluSalesHistoryTwoBlockNode& BlockTotals, bool bTotals )
{
	if ( m_nFirstTotalColumn >= 0 )
	{
		int nDailyColDummy = -1;
		AppendDateBlockTotals( csv, 1, &LineTotals.m_SalesBlock[0], &BlockTotals.m_SalesBlock[0], bTotals, nDailyColDummy );
		AppendDateBlockTotals( csv, 2, &LineTotals.m_SalesBlock[1], &BlockTotals.m_SalesBlock[1], bTotals, nDailyColDummy );
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::AppendDateBlockTotals( CCSV& csv, int nBlock, CEntityTreePluSalesHistoryBlock* pLineTotals, CEntityTreePluSalesHistoryBlock* pBlockTotals, bool bTotals, int& nDailyColStart, int nDailyColCount, int nStartOffset, int nEndOffset )
{
	//LINE TOTALS
	double dLineBaseQty = pLineTotals -> GetBaseQty();
	double dLineModQty = pLineTotals -> GetModQty();
	double dLineVal = pLineTotals -> GetTotalVal();
	double dLineCost = pLineTotals -> GetCost();
	double dLineWasteCost = pLineTotals -> GetWasteCost();
	double dLineTax = pLineTotals -> GetTaxAmount();
	double dLineDiscountVal = pLineTotals -> GetDiscountVal();
	double dLinePremiumVal = pLineTotals->GetPremiumVal();

	double dLineNet = dLineVal - dLineDiscountVal + dLinePremiumVal;
	double dLineGP = dLineNet - ( dLineCost + dLineTax  + dLineWasteCost );

	//BLOCK TOTALS
	double dBlockVal = pBlockTotals -> GetTotalVal();
	double dBlockCost = pBlockTotals -> GetCost();
	double dBlockWasteCost = pBlockTotals -> GetWasteCost();
	double dBlockTax = pBlockTotals -> GetTaxAmount();
	double dBlockDiscountVal = pBlockTotals -> GetDiscountVal();
	double dBlockPremiumVal = pBlockTotals->GetPremiumVal();

	double dBlockNet = dBlockVal - dBlockDiscountVal + dBlockPremiumVal;
	double dBlockGP = dBlockNet - ( dBlockCost + dBlockTax + dBlockWasteCost );

	int nCol = m_nFirstTotalColumn;
	if ( SALESHISTORY_REPORTMODE_CHART == m_nReportMode )
		nCol--;

	for ( int n = m_nFirstTotalColumn; n < m_arrayReportColumns.GetSize(); n++ )
	{
		if ( m_arrayReportColumns[n].m_nBlock != nBlock )
		{
			nCol++;
			continue;
		}

		switch( m_arrayReportColumns[n].m_nTotalType )
		{
		case TOTAL_TYPE_BASEQTY:
			csv.SetAt( nCol++, dLineBaseQty, SysInfo.GetDPQty() );
			break;
		
		case TOTAL_TYPE_MODQTY:
			csv.SetAt( nCol++, dLineModQty, SysInfo.GetDPQty() );
			break;

		case TOTAL_TYPE_DAILY:
			nDailyColStart = nCol;
			nCol += nDailyColCount;
			break;

		case TOTAL_TYPE_SALES:
			csv.SetAt( nCol++, dLineVal, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_DISCOUNT:
			csv.SetAt( nCol++, dLineDiscountVal, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_PREMIUM:
			csv.SetAt(nCol++, dLinePremiumVal, SysInfo.GetDPValue());
			break;

		case TOTAL_TYPE_NET:
			csv.SetAt( nCol++, dLineNet, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_AVERAGE:
			csv.SetAt( nCol++, ReportHelpers.CalcAverage ( dLineBaseQty, dLineNet ), SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_TAX:
			csv.SetAt( nCol++, dLineTax, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_NETOFTAX:
			csv.SetAt( nCol++, dLineNet - dLineTax, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_COST:
			csv.SetAt( nCol++, dLineCost, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_WASTE:
			csv.SetAt( nCol++, dLineWasteCost, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_GP:
			csv.SetAt( nCol++, dLineGP, SysInfo.GetDPValue() );
			break;

		case TOTAL_TYPE_GP_PERCENT:
			csv.SetAt ( nCol++, ReportHelpers.CalcGPPercent ( dLineNet - dLineTax, 0.0, dLineCost + dLineWasteCost ), 2 );
			break;

		case TOTAL_TYPE_PERCENT_OF_GP:
			if (FALSE == bTotals)
			{
				double dPercentOfProfit = 0.0;

				if (dBlockGP != 0.0)
				{
					dPercentOfProfit = (dLineGP * 100.0) / dBlockGP;
				}

				csv.SetAt(nCol++, dPercentOfProfit, 2);
			}
			
			break;
			
		case TOTAL_TYPE_PERCENT_OF_SALE:
			if (FALSE == bTotals)
			{
				double dPercentOfSale = 0.0;

				if (dBlockNet != 0.0)
				{
					dPercentOfSale = (dLineNet * 100.0) / dBlockNet;
				}

				csv.SetAt(nCol++, dPercentOfSale, 2);
			}
			break;

		default:
			nCol++;
			break;
		}
	}	
}

/**********************************************************************/

void CSalesHistoryReportPlu::AddToChart( double dQty, double dValue, const char* szLine )
{
	double dToInsert = m_chartinfo.IsSortedByValue() ? dValue : dQty;
	
	if (m_chartinfo.IsBottom100())
	{
		dToInsert = -dToInsert;
	}

	dToInsert += 0.0000001;

	int nStartIdx = 0;
	int nEndIdx = m_arraySalesChart.GetSize() - 1;

	while( nStartIdx <= nEndIdx )
	{
		int nArrayIdx = ( nStartIdx + nEndIdx ) / 2;
		double dToCompare = m_arraySalesChart.GetAt( nArrayIdx );
		
		if ( dToInsert > dToCompare )
		{
			nEndIdx = nArrayIdx - 1;
			continue;
		}

		if ( dToInsert < dToCompare )
		{
			nStartIdx = nArrayIdx + 1;
			continue;
		}

		if ( dToInsert == dToCompare )
		{
			nStartIdx = nArrayIdx;
			break;
		}
	}
		
	if ( m_chartinfo.GetChartType() != CHART_TYPE_MAXITEMS )
	{
		m_arraySalesChart.InsertAt( nStartIdx, dToInsert );
		m_arrayBufferedLines.InsertAt( nStartIdx, szLine );
	}
	else if ( nStartIdx < m_chartinfo.GetChartSize() )
	{
		m_arraySalesChart.InsertAt( nStartIdx, dToInsert );
		m_arrayBufferedLines.InsertAt( nStartIdx, szLine );

		if ( m_arraySalesChart.GetSize() == m_chartinfo.GetChartSize() + 1 )
		{
			m_arraySalesChart.RemoveAt ( m_chartinfo.GetChartSize() );
			m_arrayBufferedLines.RemoveAt( m_chartinfo.GetChartSize() );
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteHeader( int nDepth )
{
	CString strTemp = GetHeaderLine(nDepth);
	if ( strTemp.GetLength() >= 1 ) strTemp = strTemp.Right( strTemp.GetLength() - 1 );
	CString strHeader = "<..>";
	strHeader += strTemp;
	m_ReportFile.RequestBlankLines(1);
	m_ReportFile.WriteReportMiscLine( strHeader );
	m_ReportFile.WriteReportMiscLine( "<LI>" );
}

/**********************************************************************/

const char* CSalesHistoryReportPlu::GetLabelTimeString()
{
	m_strLabelTimeString = "";

	switch( m_TimeSliceMap.GetReportType() )
	{
	case SH_TIMESLICE_MINUTE:
		m_strLabelTimeString.Format( "%2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute );
		break;

	case SH_TIMESLICE_5MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 4 );
		break;

	case SH_TIMESLICE_10MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 9 );
		break;

	case SH_TIMESLICE_15MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 14 );
		break;

	case SH_TIMESLICE_30MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 29 );
		break;

	case SH_TIMESLICE_60MIN:
		m_strLabelTimeString.Format( "%2.2d:00 - %2.2d:59",
			m_nLabelTimeHour,
			m_nLabelTimeHour );
		break;

	case SH_CUSTOM:
		{
			m_strLabelTimeString.Format( "%4.4d", m_nLabelTimeHour );
			m_strLabelTimeString = m_TimeMap.GetSortOrderName( m_strLabelTimeString );
		}
		break;

	case SH_TIMESLICE_SECOND:
	default:
		m_strLabelTimeString.Format( "%2.2d:%2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeSecond );
		break;
	}

	switch ( m_nLabelTimeStars )
	{
	case 1:
		m_strLabelTimeString += "*";
		break;

	case 2:
		m_strLabelTimeString += "**";
		break;
	}

	return m_strLabelTimeString;
}

/**********************************************************************/


