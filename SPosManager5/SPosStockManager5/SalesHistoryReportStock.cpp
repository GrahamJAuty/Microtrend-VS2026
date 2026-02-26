/**********************************************************************/
#include <share.h>
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "SalesHistoryDateInfo.h"
#include "SalesTypes.h"
#include "SiteCSVArray.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportStock.h"
/**********************************************************************/

CSalesHistoryReportStock::CSalesHistoryReportStock()
{
	m_strWeekDays[0] = "Sun";
	m_strWeekDays[1] = "Mon";
	m_strWeekDays[2] = "Tue";
	m_strWeekDays[3] = "Wed";
	m_strWeekDays[4] = "Thu";
	m_strWeekDays[5] = "Fri";
	m_strWeekDays[6] = "Sat";
	
	m_ReportFilters.CategoryFilter.SetCategoryFilterNo( CATEGORYSET_ALL );
	m_bCategoryByItem = FALSE;

	m_bEposSales = TRUE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;

	m_nConLevel = NODE_SYSTEM;
	m_nTableNo = 0;
	m_nDbIdx = 0;

	m_bPercentSale = FALSE;
	m_nPercentDepth = 0;
	m_bEstimatedProfit = FALSE;
	m_bItemRange = FALSE;
	m_bLastLineWasBlank = TRUE;
	m_strItemFrom = "";
	m_strItemTo = "";

	m_nApparentConLevel = NODE_SYSTEM;
	m_nApparentTableNo = 0;
	m_bApparent = FALSE;
}

/**********************************************************************/

void CSalesHistoryReportStock::Reset( CSalesHistoryCSVRecordStock& ReportRecord, CLocationSelectorEntity& LocSelEntity, bool bEposSales, bool bManualSales, bool bImportSales )
{
	m_nApparentConLevel = NODE_SYSTEM;
	m_nApparentTableNo = 0;
	m_bApparent = FALSE;

	int nReportType = ReportRecord.GetReportType();

	if ( nReportType == SH_CUSTOM )
		ReportRecord.GetReportFields( m_arrayReportFields );
	else
		m_arrayReportFields.RemoveAll();

	m_nReportType = ReportRecord.GetReportType();
	m_nReportMode = ReportRecord.GetReportMode();
	m_strReportName = ReportRecord.GetReportName();
	
	m_ReportFilters.CategoryFilter.SetCategoryFilterNo( CATEGORYSET_ALL );
	m_SupplierIdxArray.RemoveAll();
	m_bEposSales = bEposSales;
	m_bManualSales = bManualSales;
	m_bImportSales = bImportSales;
	
	m_nConLevel = LocSelEntity.GetConType();
	m_nTableNo = LocSelEntity.GetEntityNo();
	m_nDbIdx = LocSelEntity.GetDbIdx();

	m_bPercentSale = FALSE;
	m_nPercentDepth = 0;
	m_bEstimatedProfit = FALSE;
	m_bItemRange = FALSE;
	m_bLastLineWasBlank = TRUE;
	m_strItemFrom = "";
	m_strItemTo = "";
	
	switch ( m_nReportType )
	{
	case SH_STK_ITEM:
	case SH_STK_TOP100:
		m_arrayReportFields.Add( SH_FIELD_CODE );
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER );
		break;
		
	case SH_STK_ITEM_CAT:
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		break;

	case SH_STK_ITEM_MONTH:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER );
		break;
	
	case SH_STK_ITEM_MONTH_CAT:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		break;

	case SH_STK_TOP100_CAT:
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		break;

	case SH_STK_TOP100_MONTH:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER );
		break;

	case SH_STK_TOP100_MONTH_CAT:
		m_arrayReportFields.Add( SH_FIELD_MONTH  );
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		break;

	case SH_STK_UNSOLD:
	case SH_STK_UNSOLD_CAT:
		m_arrayReportFields.Add( SH_FIELD_CODE );
		break;

	case SH_STK_UNSOLD_MONTH:
	case SH_STK_UNSOLD_MONTH_CAT:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER );
		m_arrayReportFields.Add( SH_FIELD_CODE );
		break;

	case SH_STK_CAT:
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER );
		break;

	case SH_STK_CAT_MONTH:
		m_arrayReportFields.Add( SH_FIELD_MONTH + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER );
		break;

	case SH_STK_DAILY:
		m_arrayReportFields.Add( SH_FIELD_DATE_BUSINESS );
		break;

	case SH_STK_MONTHLY:
		m_arrayReportFields.Add( SH_FIELD_MONTH );
		break;

	case SH_STK_WEEKLY:
		m_arrayReportFields.Add( SH_FIELD_WEEK );
		break;

	case SH_STK_DAILY_BYWEEK:
		m_arrayReportFields.Add( SH_FIELD_WEEK );
		m_nReportMode = SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL;
		break;

	case SH_STK_CAT_BYWDAY:
		m_arrayReportFields.Add( SH_FIELD_CAT_FILTER );
		m_nReportMode = SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL;
		break;

	case SH_STK_SP_BYWDAY:
		m_arrayReportFields.Add( SH_FIELD_SP );
		m_nReportMode = SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL;
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

void CSalesHistoryReportStock::SetChartDetails( CSalesHistoryCSVRecordStock& ReportRecord )
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
		strTitle.Format ( "%s%s Stock Items by %s",
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

void CSalesHistoryReportStock::SetSupplierFilter( CWordArray& array )
{
	m_SupplierIdxArray.RemoveAll();
	for ( int n = 0; n < array.GetSize(); n++ )
		m_SupplierIdxArray.Add( array.GetAt(n) );
}

/**********************************************************************/

bool CSalesHistoryReportStock::PrepareFields()
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
			case SH_FIELD_CODE:
				bGotNonGlobalField = TRUE;
				break;

			case SH_FIELD_CAT_SALE:
			case SH_FIELD_CAT_ITEM:
			case SH_FIELD_CAT_FILTER:
				if ( StockOptions.GetGlobalCategoryFlag() == FALSE )
				{
					bGotNonGlobalField = TRUE;
				}
				break;

			case SH_FIELD_CUSTOM1:
			case SH_FIELD_CUSTOM2:
			case SH_FIELD_CUSTOM3:
			case SH_FIELD_CUSTOM4:
			case SH_FIELD_CUSTOM5:
				if ( StockOptions.GetGlobalCustomFieldFlag() == FALSE )
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

	m_nApparentConLevel = m_nConLevel;
	m_nApparentTableNo = m_nTableNo;

	CReportConsolidationArray<CSortedStringItem> FieldsSoFar;
	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{		
		int nField = m_arrayReportFields.GetAt( nPos );
		int nFieldType = nField % 100;
		bool bAccept = FALSE;

		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:
		case SH_FIELD_WEEK:
		case SH_FIELD_WEEKDAY:
		case SH_FIELD_DATE_BUSINESS:
		case SH_FIELD_TIME:
		case SH_FIELD_DBASE:
		case SH_FIELD_CODE:
		case SH_FIELD_DESCRIPTION:
		case SH_FIELD_SITE:
		case SH_FIELD_SP:
		case SH_FIELD_CAT_SALE:
		case SH_FIELD_CAT_ITEM:
		case SH_FIELD_CAT_FILTER:
		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
		case SH_FIELD_SUPP:
		case SH_FIELD_SUPPREF:
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

		if ( bAccept == FALSE )
		{
			m_arrayReportFields.RemoveAt( nPos );
			nPos--;
		}
		else 
		{
			switch( nFieldType )
			{
			case SH_FIELD_DBASE:
				m_nApparentConLevel = NODE_DATABASE;
				break;

			case SH_FIELD_SITE:
				m_nApparentConLevel = NODE_SITE;
				break;

			case SH_FIELD_SP:
				m_nApparentConLevel = NODE_STOCKPOINT;
				break;
			
			case SH_FIELD_DESCRIPTION:
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
		case SH_FIELD_CAT_FILTER:
			nFieldType = ( m_bCategoryByItem ) ? SH_FIELD_CAT_ITEM : SH_FIELD_CAT_SALE;
			break;

		case SH_FIELD_CODE:
			if ( TRUE == bGotDescriptionField )
				nFieldType = SH_FIELD_CODE_ONLY;
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
		case SH_FIELD_CODE:
		case SH_FIELD_CODE_ONLY:
		case SH_FIELD_SUPPREF:
		case SH_FIELD_TIME:
		case SH_FIELD_DESCRIPTION:
			m_QtyValueTable.AddField( SORT_TYPE_STRING );
			break;

		case SH_FIELD_DBASE:
		case SH_FIELD_SITE:
		case SH_FIELD_SP:
		case SH_FIELD_CAT_SALE:
		case SH_FIELD_CAT_ITEM:
		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
		case SH_FIELD_WEEKDAY:
		case SH_FIELD_SUPP:
			m_QtyValueTable.AddField( SORT_TYPE_INT );
			break;

		case SH_FIELD_MONTH:
		case SH_FIELD_WEEK:
		case SH_FIELD_DATE_BUSINESS:
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

void CSalesHistoryReportStock::Consolidate( bool bNoCategoryFilter )
{
	m_QtyValueTable.ClearAutoFillDates();

	if ( DealerFlags.GetWeeklyReportNoZeroSkipFlag() == TRUE )
	{
		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
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
						
							if ( nReportStartDay < nWeekStartDay ) 
								nReportStartDay += 7;

							nStartOffset = nReportStartDay - nWeekStartDay;
							oleDateFrom -= COleDateTimeSpan ( nStartOffset, 0, 0, 0 );
						}

						{
							int nReportEndDay = oleDateTo.GetDayOfWeek();
						
							if ( nReportEndDay < nWeekStartDay ) 
								nReportEndDay += 7;

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

	m_QtyValueTable.ClearConsolidatedLevels();
	dbDatabase.BuildSortIdxTables();
	dbSite.BuildSortIdxTables();
	dbStockpoint.BuildSortIdxTables();

	switch( m_nReportType )
	{
	case SH_STK_UNSOLD:
	case SH_STK_UNSOLD_CAT:
	case SH_STK_UNSOLD_MONTH:
	case SH_STK_UNSOLD_MONTH_CAT:
		bNoCategoryFilter = FALSE;
		break;
	}

	bool bWantSupplier = FALSE;
	bool bFilterSupplier = FALSE;
	bool bWantCategory = FALSE;
	bool bWantCustomField = FALSE;

	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		switch( m_arrayReportFields.GetAt( n ) )
		{
		case SH_FIELD_SUPP:	
		case SH_FIELD_SUPPREF:
			bWantSupplier = TRUE;	
			break;

		case SH_FIELD_CAT_ITEM:
			bWantCategory = TRUE;
			break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			bWantCustomField = TRUE;
			break;
		}
	}

	if ( m_bCategoryByItem == TRUE )
		bWantCategory = TRUE;

	//MAP SUPPLIER NUMBERS TO SORT POSITIONS
	m_SupplierReportMap.CreateMap( m_SupplierIdxArray );

	//LIST SHOULD BE ONE BIGGER THAN SUPPLIER ARRAY TO
	//ALLOW FOR UNSPECIFIED SUPPLIER
	if ( DataManager.Supplier.GetSize() >= m_SupplierIdxArray.GetSize() )
	{
		bWantSupplier = TRUE;
		bFilterSupplier = TRUE;
	}

	CString strMonthFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom().m_oleDateTime.Format( "%Y%m" );
	CString strMonthTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo().m_oleDateTime.Format( "%Y%m" );

	CStringArray FileList, DateList;
	CWordArray SpIdxList;
	__int64 nTotalFileSize = 0;

	CWordArray arraySpIdx;
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		if ( MatchStockpoint( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	StatusProgress.Lock( TRUE, "Finding Stockpoint Sales Files" );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);

		StatusProgress.SetPos( n, arraySpIdx.GetSize() );
		
		CString str;
		str.Format( "(%s)", dbStockpoint.GetName( nSpIdx ) );
		StatusProgress.SetDialogText( str );

		dbStockpoint.GetHistoryFileList( nSpIdx, strMonthFrom, strMonthTo, FileList, DateList, nTotalFileSize, TRUE );
	
		while ( SpIdxList.GetSize() < FileList.GetSize() )
			SpIdxList.Add( nSpIdx );
	}

	StatusProgress.Unlock();

	__int64 nProgress = 0;
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	bool bUpdateCustomFieldValueMap = TRUE;

	CCSVSalesHistory csvSales( "" );

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < FileList.GetSize(); nFileIdx++ )
	{
		int nSpIdx = SpIdxList.GetAt(nFileIdx);
		int nSiteIdx = SITEIDX_SPECIAL_UNKNOWN;
		int nDbIdx = DBIDX_SPECIAL_UNKNOWN;
		
		int nDbNo = dbStockpoint.GetDbNo( nSpIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
		{
			if ( m_ReportFilters.SetDatabase(nDbIdx) == TRUE )
			{
				bUpdateCustomFieldValueMap = TRUE;
			}

			int nSiteNo = dbStockpoint.GetSiteNo( nSpIdx );
			if ( dbSite.FindSiteByNumber( nDbNo, nSiteNo, nSiteIdx ) == FALSE )
				nSiteIdx = SITEIDX_SPECIAL_UNKNOWN;
		}

		if ( ( TRUE == bUpdateCustomFieldValueMap ) && ( FALSE == bNoCategoryFilter ) )
		{
			if ( ( NODE_SYSTEM == m_nConLevel ) && ( StockOptions.GetGlobalCustomFieldFlag() == FALSE ) )
				DataManager.CustomFieldManager.UpdateValueMapsFromComboSelectorsSystem();
			else
				DataManager.CustomFieldManager.UpdateValueMapsFromComboSelectorsDatabase();
			
			bUpdateCustomFieldValueMap = FALSE;
		}

		CString strFilename = FileList.GetAt( nFileIdx );
		
		CString strMonth = DateList.GetAt( nFileIdx );
		if ( strMonth.GetLength() != 6 )
			continue;

		COleDateTime oleSaleMonth( atoi ( strMonth.Left(4) ), atoi ( strMonth.Right ( 2 ) ), 1, 0, 0, 0 );
		if ( oleSaleMonth.m_status != COleDateTime::valid )
			continue;
				
		CSSFile fileSales;
		if ( fileSales.Open ( strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		int nTaxDateCode = -1;
		COleDateTime oleTaxDate = COleDateTime( 1980, 1, 1, 0, 0, 0 );
		
		int nSortSpIdx = dbStockpoint.GetSortIdxFromSpIdx( nSpIdx );
		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
		int nSortSiteIdx = dbSite.GetSortIdxFromSiteIdx( nSiteIdx );

		//THESE FIELDS STAY THE SAME THROUGHOUT THE FILE
		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		{
			switch ( m_arrayReportFields.GetAt( n ) )
			{
			case SH_FIELD_MONTH:	m_QtyValueTable.SetBufferDate( n, oleSaleMonth );	break;
			case SH_FIELD_DBASE:	m_QtyValueTable.SetBufferInt( n, nSortDbIdx );		break;
			case SH_FIELD_SITE:		m_QtyValueTable.SetBufferInt( n, nSortSiteIdx  );	break;
			case SH_FIELD_SP:		m_QtyValueTable.SetBufferInt( n, nSortSpIdx );		break;
			}
		}

		//MAKE SURE WE HAVE THE RIGHT DATABASE LOADED FOR TAX ETC
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		
		int nMDay = 0;
		CSalesHistoryDateInfo infoDate, infoDateOne, infoDateTwo, infoLineDate;
	
		CString strBuffer;
		while ( fileSales.ReadString ( strBuffer ) )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			if ( strBuffer == "" || strBuffer.Left(1) == "#" )
				continue;

			csvSales.ParseLine( strBuffer );
		
			//FILTER BY SALES TYPE
			bool bTypeOK = FALSE;
			switch ( csvSales.GetType() )
			{
			case SALES_TYPE_E1:		bTypeOK = m_bEposSales;		break;
			case SALES_TYPE_MANUAL:	bTypeOK = m_bManualSales;	break;
			case SALES_TYPE_IMPORT:	bTypeOK = m_bImportSales;	break;
			}

			if ( bTypeOK == FALSE )
				continue;

			//FILTER BY CATEGORY OF SALE
			int nCategorySale = csvSales.GetCategory();

			if ( bNoCategoryFilter == FALSE )
			{
				if ( m_bCategoryByItem == FALSE )
					if ( m_ReportFilters.CheckCategory( nCategorySale ) == FALSE )
						continue;
			}

			//FILTER BY DATE
			CString strSaleTime = csvSales.GetTime();
			
			switch( strSaleTime.GetLength() )
			{
			case 6:
				break;

			case 4:
				strSaleTime += "00";
				break;

			default:
				strSaleTime = "000000";
				break;
			}
			
			int nSaleMDay = csvSales.GetMDay();

			COleDateTime oleSale = COleDateTime (
				oleSaleMonth.GetYear(),
				oleSaleMonth.GetMonth(),
				nSaleMDay,
				atoi(strSaleTime.Left(2)), 
				atoi(strSaleTime.Mid(2,2)), 
				atoi(strSaleTime.Right(2)) );

			if ( oleSale.GetStatus() != COleDateTime::valid )
				continue;

			if ( nSaleMDay != nMDay )
			{
				infoDate.m_Date = COleDateTime( oleSale.GetYear(), oleSale.GetMonth(), oleSale.GetDay(), 0, 0, 0 );
				infoDateOne.m_Date = infoDate.m_Date - COleDateTimeSpan( 1, 0, 0, 0 );
				infoDateTwo.m_Date = infoDate.m_Date - COleDateTimeSpan( 2, 0, 0, 0 );

				infoDate.SetDateInfoForReports();
				infoDateOne.SetDateInfoForReports();
				infoDateTwo.SetDateInfoForReports();

				nMDay = nSaleMDay;
			}

			CString strDate;
			strDate.Format( "%s%2.2d", (const char*) strMonth, oleSale.GetDay() );
			
			CString strLineTime = "";
			switch ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strDate, strSaleTime ) )
			{
			case 1:
				strLineTime = "0";
				strLineTime += strSaleTime;
				infoLineDate = infoDate;
				break;

			case 2:
				strLineTime = "1";
				strLineTime += strSaleTime;
				infoLineDate = infoDateOne;
				break;

			case 3:
				strLineTime = "2";
				strLineTime += strSaleTime;
				infoLineDate = infoDateTwo;
				break;

			case 0:
			default:
				continue;
			}

			//FILTER BY STOCKCODE
			CString strStockCode = csvSales.GetStockCode();
			if ( m_bItemRange == TRUE )
			{
				if ( m_strItemFrom != "" && strStockCode < m_strItemFrom )
					continue;
				
				if ( m_strItemTo != "" && strStockCode > m_strItemTo )
					continue;
			}

			//LOOK UP ITEM ONLY IF NEEDED
			int nStockIdx = 0;
			bool bIsKnown = FALSE;
			CStockCSVRecord StockRecord;
			CString strDescription = "";
			
			//if ( ( TRUE == bWantSupplier ) || ( TRUE == bWantCategory ) || ( TRUE == m_bEstimatedProfit ) || ( TRUE == bWantCustomField ) )
			{
				CString strStockCode = csvSales.GetStockCode();
				if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
				{		
					bIsKnown = TRUE;
					DataManager.Stock.GetAt( nStockIdx, StockRecord );
					strDescription = "1";
					strDescription += StockRecord.GetDescription();
				}
				else
					strDescription = "0**Unknown";
			}
			
			//FILTER BY SUPPLIER
			int nSuppNo = 0;
			CString strSuppRef = "";
			int nSuppSortIdx = 0;

			if ( bWantSupplier == TRUE )
			{
				if ( bIsKnown == TRUE )
				{
					nSuppNo = StockRecord.GetSuppNo(0);
					strSuppRef = StockRecord.GetSuppRefExternal(0, DealerFlags.GetSuppRefFlag() );
				}

				nSuppSortIdx = m_SupplierReportMap.GetSortIdxBySuppNo( nSuppNo );
			}

			if ( nSuppSortIdx == -1 )
				continue;

			//GET ITEM CATEGORY
			int nCategoryItem = 0;

			if ( bWantCategory == TRUE )
				if ( bIsKnown == TRUE )
					nCategoryItem = StockRecord.GetCategory();

			int nCustomFieldSortIdx[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
				
			//FILTER BY CATEGORY OF ITEM
			if ( bNoCategoryFilter == FALSE )
			{
				if ( m_bCategoryByItem == TRUE )
					if ( m_ReportFilters.CheckCategory( nCategoryItem ) == FALSE )
						continue;

				//FILTER BY CUSTOM FIELD
				for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
					nCustomFieldSortIdx[nFieldNo] = CUSTOM_FIELD_SORTIDX_NA;

				bool bNoCustomFieldMatch = FALSE;

				for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
				{
					int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
					int nValueNo = StockRecord.GetCustomFieldValueNo( nFieldNo );
					int nSortIdx = DataManager.CustomFieldManager.GetSortIdxByValueNo( nSlot, nValueNo );
		
					if ( nSortIdx == -1 )
					{
						bNoCustomFieldMatch = TRUE;
						break;
					}

					nCustomFieldSortIdx[ nFieldNo ] = nSortIdx;
				}

				if ( TRUE == bNoCustomFieldMatch )
					continue;
			}

			//THE ACTUAL DATE OF SALE IS USED FOR TAX BY DATE
			if ( ( -1 == nTaxDateCode ) || ( infoDate.m_Date != oleTaxDate ) )
			{
				CString strDate;
				strDate.Format( "%4.4d%2.2d%2.2d",
					infoDate.m_Date.GetYear(),
					infoDate.m_Date.GetMonth(),
					infoDate.m_Date.GetDay() );

				nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strDate );
				oleTaxDate = infoDate.m_Date;
			}

			double dLineQty = 0.0;
			double dLineVal = 0.0;
			double dLineCost = 0.0;
			double dLineWaste = 0.0;
			double dLineTax = 0.0;
			bool bConsolidateLine = FALSE;

			if ( csvSales.GetType() == SALES_TYPE_ECRWASTE )
			{
				if ( ( TRUE == m_bEstimatedProfit ) && ( TRUE == bIsKnown ) )
				{
					dLineWaste = StockRecord.GetUnitCost() * csvSales.GetStockValue();
					bConsolidateLine = TRUE;
				}
			}
			else
			{
				bConsolidateLine = TRUE;

				dLineQty = csvSales.GetStockQty();
				dLineVal = csvSales.GetStockValue();

				if  ( ( TRUE == m_bEstimatedProfit ) && ( TRUE == bIsKnown ) )
				{
					DataManager.StockPlu.HideProgress();
					dLineCost = StockRecord.GetUnitCost() * dLineQty;
					dLineTax = DataManager.Stock.CalculateHistoricalTaxContentFromValue ( StockRecord, dLineVal, nTaxDateCode );
		
					if ( StockOptions.GetStockSubUnitsFlag() == TRUE  )
						dLineWaste = DataManager.Stock.GetUnitWastageCost( StockRecord, 0 ) * dLineQty;
				}
			}

			//THESE FIELDS CHANGE ON A LINE-BY-LINE BASIS
			for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
			{
				int nFieldType = m_arrayReportFields.GetAt( n );
				switch ( nFieldType )
				{
				case SH_FIELD_WEEK:				m_QtyValueTable.SetBufferDate( n, infoLineDate.m_DateWeek ); break;
				case SH_FIELD_WEEKDAY:			m_QtyValueTable.SetBufferInt( n, infoLineDate.m_nWeekday );	 break;
				case SH_FIELD_DATE_BUSINESS:	m_QtyValueTable.SetBufferDate( n, infoLineDate.m_Date ); break;
				case SH_FIELD_TIME:				m_QtyValueTable.SetBufferString( n, strLineTime ); break;
				case SH_FIELD_CODE:				m_QtyValueTable.SetBufferString( n, strStockCode ); break;
				case SH_FIELD_CODE_ONLY:		m_QtyValueTable.SetBufferString( n, strStockCode ); break;
				case SH_FIELD_DESCRIPTION:		m_QtyValueTable.SetBufferString( n, strDescription ); break;
				case SH_FIELD_CAT_SALE:			m_QtyValueTable.SetBufferInt( n, nCategorySale ); break;
				case SH_FIELD_CAT_ITEM:			m_QtyValueTable.SetBufferInt( n, nCategoryItem ); break;
				case SH_FIELD_SUPP:				m_QtyValueTable.SetBufferInt( n, nSuppSortIdx ); break;
				case SH_FIELD_SUPPREF:			m_QtyValueTable.SetBufferString( n, strSuppRef ); break;
				case SH_FIELD_CUSTOM1:			m_QtyValueTable.SetBufferInt( n, nCustomFieldSortIdx[1] );	break;
				case SH_FIELD_CUSTOM2:			m_QtyValueTable.SetBufferInt( n, nCustomFieldSortIdx[2] );	break;
				case SH_FIELD_CUSTOM3:			m_QtyValueTable.SetBufferInt( n, nCustomFieldSortIdx[3] );	break;
				case SH_FIELD_CUSTOM4:			m_QtyValueTable.SetBufferInt( n, nCustomFieldSortIdx[4] );	break;
				case SH_FIELD_CUSTOM5:			m_QtyValueTable.SetBufferInt( n, nCustomFieldSortIdx[5] );	break;
				}
			}

			if ( TRUE == bConsolidateLine )
			{
				switch( m_nReportMode )
				{
				case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
					{
						CEntityTreeStockSalesHistoryWeeklyNode Node;
						Node.SetQty( dLineQty );
						Node.SetTotalVal( dLineVal );
						Node.SetDailyVal( infoLineDate.m_nWeekday, dLineVal );
						Node.SetCost( dLineCost );
						Node.SetWasteCost( dLineWaste );
						Node.SetTaxAmount( dLineTax );
						m_QtyValueTable.Consolidate ( Node, infoLineDate.m_nWeekday );
					}
					break;

				case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
					{
						CEntityTreeStockSalesHistoryWeeklyNode Node;
						Node.SetQty( dLineQty );
						Node.SetTotalVal( dLineVal );
						Node.SetDailyVal( infoLineDate.m_nWeekday, dLineQty );
						Node.SetCost( dLineCost );
						Node.SetWasteCost( dLineWaste );
						Node.SetTaxAmount( dLineTax );
						m_QtyValueTable.Consolidate ( Node, infoLineDate.m_nWeekday );
					}
					break;

				case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
					{
						int nMonthday = infoLineDate.m_Date.GetDay() - 1;

						CEntityTreeStockSalesHistoryMonthlyNode Node;
						Node.SetQty( dLineQty );
						Node.SetTotalVal( dLineVal );
						Node.SetDailyVal( nMonthday, dLineVal );
						Node.SetCost( dLineCost );
						Node.SetWasteCost( dLineWaste );
						Node.SetTaxAmount( dLineTax );
						m_QtyValueTable.Consolidate ( Node, nMonthday );
					}
					break;

				case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
					{
						int nMonthday = infoLineDate.m_Date.GetDay() - 1;

						CEntityTreeStockSalesHistoryMonthlyNode Node;
						Node.SetQty( dLineQty );
						Node.SetTotalVal( dLineVal );
						Node.SetDailyVal( nMonthday, dLineQty );
						Node.SetCost( dLineCost );
						Node.SetWasteCost( dLineWaste );
						Node.SetTaxAmount( dLineTax );
						m_QtyValueTable.Consolidate ( Node, nMonthday );
					}
					break;

				case SALESHISTORY_REPORTMODE_NORMAL:
				default:
					{
						CEntityTreeStockSalesHistoryNormalNode Node;
						Node.SetQty( dLineQty );
						Node.SetTotalVal( dLineVal );
						Node.SetCost( dLineCost );
						Node.SetWasteCost( dLineWaste );
						Node.SetTaxAmount( dLineTax );
						m_QtyValueTable.Consolidate ( Node );
					}
					break;
				}
			}
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CSalesHistoryReportStock::WriteReportLine( const char* szLine )
{
	CString strLine = szLine;
	if ( strLine == "" )
		return;

	m_ReportFile.WriteLine( strLine );
	m_bLastLineWasBlank = FALSE;
}

/**********************************************************************/

void CSalesHistoryReportStock::WriteBlankLine( bool bOnlyIfNeeded )
{
	if ( bOnlyIfNeeded && m_bLastLineWasBlank )
		return;

	m_ReportFile.WriteLine ( "" );
	m_bLastLineWasBlank = TRUE;
}

/**********************************************************************/

bool CSalesHistoryReportStock::CreateReport()
{
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );

	if ( m_bApparent == TRUE )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartIdx, nEndIdx );

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			if ( MatchStockpoint( nSpIdx ) == TRUE )
				DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
		}
	}

	{
		m_strUnderline = "";
		m_nColumnsBeforeTotal = 0;

		for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
		{
			switch ( m_arrayReportFields.GetAt( nPos ) )
			{
			case SH_FIELD_WEEK:		
			case SH_FIELD_CODE:	
				m_strUnderline += "\t\t";
				m_nColumnsBeforeTotal += 2;
				break;

			default:			
				m_strUnderline += "\t";
				m_nColumnsBeforeTotal += 1;
				break;
			}
		}

		int nTotals = 3;
		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
			nTotals += 7;	
			break;

		case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
		case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
			nTotals += 31;	
			break;
		}

		if ( TRUE == m_bEstimatedProfit )
			nTotals += 3;

		for ( int n = 1; n <= nTotals; n++ )
		{
			m_strUnderline += "<LI>";
		
			if ( n < nTotals )
				m_strUnderline += "\t";
		}
	}

	bool bResult = FALSE;

	StatusProgress.Lock( TRUE, "Creating report" );
	switch ( m_nReportType )
	{
	case SH_STK_UNSOLD:
	case SH_STK_UNSOLD_CAT:			bResult = CreateUnsoldReportInternal();				break;
	case SH_STK_UNSOLD_MONTH:
	case SH_STK_UNSOLD_MONTH_CAT:	bResult = CreateUnsoldMonthlyReportInternal();		break;
	default:						bResult = CreateCustomReportInternal();				break;
	}
	StatusProgress.Unlock();
	
	return bResult;
}

/**********************************************************************/

void CSalesHistoryReportStock::AddWeekdayColumns()
{
	int nDay = EcrmanOptions.GetReportsWeekStartDay();
	if ( nDay < 0 || nDay > 6 ) nDay = 0;
			
	for ( int n = 0; n < 7; n++ )
		m_ReportFile.AddColumn ( m_strWeekDays [ (nDay++) % 7 ], TA_RIGHT, 250 );
}

/**********************************************************************/

void CSalesHistoryReportStock::AddMonthdayColumns()
{
	for ( int n = 1; n <= 31; n++ )
	{
		CString strNum;
		strNum.Format( "%2.2d", n );
		m_ReportFile.AddColumn ( strNum, TA_RIGHT, 250 );
	}
}

/**********************************************************************/

bool CSalesHistoryReportStock::CreateCustomReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	bool bGotData = FALSE;

	CString strTitle = m_strReportName;
	::TrimSpaces( strTitle, FALSE );
	if ( strTitle == "" ) strTitle = "Custom Stock Sales History";
	m_ReportFile.SetStyle1 ( GetReportTitle( strTitle ) );

	if ( SALESHISTORY_REPORTMODE_CHART == m_nReportMode )
		m_ReportFile.AddColumn( "Pos", TA_LEFT, 100 );
	
	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		int nFieldType = m_arrayReportFields.GetAt( nPos );
		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:			m_ReportFile.AddColumn ( "Month", TA_LEFT, 250 );			break;
		case SH_FIELD_WEEKDAY:			m_ReportFile.AddColumn ( "Weekday", TA_LEFT, 250 );			break;
		case SH_FIELD_DATE_BUSINESS:	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );			break;
		case SH_FIELD_TIME:				m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );			break;
		case SH_FIELD_DBASE:			m_ReportFile.AddColumn ( "Database", TA_LEFT, 300 );		break;
		case SH_FIELD_SITE:				m_ReportFile.AddColumn ( "Site", TA_LEFT, 350 );			break;
		case SH_FIELD_SP:				m_ReportFile.AddColumn ( "Stockpoint", TA_LEFT, 350 );		break;
		case SH_FIELD_CAT_SALE:			m_ReportFile.AddColumn ( "Category", TA_LEFT, 350 );		break;
		case SH_FIELD_CAT_ITEM:			m_ReportFile.AddColumn ( "Category (Db)", TA_LEFT, 350 );	break;
		case SH_FIELD_SUPP:				m_ReportFile.AddColumn ( "Supplier", TA_LEFT, 350 );		break;
		case SH_FIELD_SUPPREF:			m_ReportFile.AddColumn ( "Reference", TA_LEFT, 350 );		break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			m_ReportFile.AddColumn ( DataManagerNonDb.CustomFieldNames.GetName( nFieldType + 1 - SH_FIELD_CUSTOM1 ), TA_LEFT, 350 );	
			break;

		case SH_FIELD_WEEK:		
			m_ReportFile.AddColumn ( "Wk No", TA_LEFT, 150 );			
			m_ReportFile.AddColumn ( "Week Starting", TA_LEFT, 250 );	
			break;
		
		case SH_FIELD_CODE:
			m_ReportFile.AddColumn ( "Stockcode", TA_LEFT, 350 );
			m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
			break;

		case SH_FIELD_CODE_ONLY:
			m_ReportFile.AddColumn ( "Stockcode", TA_LEFT, 350 );
			break;

		case SH_FIELD_DESCRIPTION:
			m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
			break;
		}
	}

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:	
		AddWeekdayColumns();
		break;

	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		AddMonthdayColumns();
		break;
	}

	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 250 );

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:	
		AddWeekdayColumns();
		break;

	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
		AddMonthdayColumns();
		break;
	}

	m_ReportFile.AddColumn ( "Sales", TA_RIGHT, 250 );

	if ( m_bEstimatedProfit == TRUE )
	{
		m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250 );
		m_ReportFile.AddColumn ( "Est. Cost", TA_RIGHT, 250 );
		m_ReportFile.AddColumn ( "Est. Waste", TA_RIGHT, 250 );
		m_ReportFile.AddColumn ( "Est. GP", TA_RIGHT, 250 );
		m_ReportFile.AddColumn ( "Est. GP%", TA_RIGHT, 250 );
	}
	else
		m_ReportFile.AddColumn ( "Average", TA_RIGHT, 250 );

	if ( m_bPercentSale == TRUE )
		m_ReportFile.AddColumn( "%Sale", TA_RIGHT, 250 );

	if ( TRUE == m_bEstimatedProfit )
		m_ReportFile.AddColumn( "% of Est. GP", TA_RIGHT, 250 );

	if ( m_bApparent == TRUE )
		m_ReportFile.AddColumn( "Apparent", TA_RIGHT, 250 );

	m_nProgress = 0;

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
		{
			CEntityTreeStockSalesHistoryWeeklyNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayWeekly( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;

	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		{
			CEntityTreeStockSalesHistoryMonthlyNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayMonthly( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;

	case SALESHISTORY_REPORTMODE_NORMAL:
	case SALESHISTORY_REPORTMODE_CHART:
	default:
		{
			CEntityTreeStockSalesHistoryNormalNode BlockTotalsNode, PercentTotalsNode;
			m_QtyValueTable.GetGrandTotalNode( BlockTotalsNode );
			ProcessQtyValueArrayNormal( 0, 0, BlockTotalsNode, PercentTotalsNode );
		}
		break;
	}

	WriteBlankLine( FALSE );
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportStock::ProcessQtyValueArrayNormal( int nDepth, int nBranchIdx, CEntityTreeStockSalesHistoryNormalNode BlockTotalsNode, CEntityTreeStockSalesHistoryNormalNode PercentTotalsNode )
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
		
		CEntityTreeStockSalesHistoryNormalNode CurrentNode;
		m_QtyValueTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLine( CurrentNode, PercentTotalsNode );		
		else
			ProcessQtyValueArrayNormal( nDepth + 1,m_QtyValueTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode, PercentTotalsNode );
	}

	if ( bTotal == TRUE )
		WriteTotals( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CSalesHistoryReportStock::ProcessQtyValueArrayWeekly( int nDepth, int nBranchIdx, CEntityTreeStockSalesHistoryWeeklyNode BlockTotalsNode, CEntityTreeStockSalesHistoryWeeklyNode PercentTotalsNode )
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
		
		CEntityTreeStockSalesHistoryWeeklyNode CurrentNode;
		m_QtyValueTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLine( CurrentNode, PercentTotalsNode );		
		else
			ProcessQtyValueArrayWeekly( nDepth + 1, m_QtyValueTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode, PercentTotalsNode );
	}

	if ( bTotal == TRUE )
		WriteTotals( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CSalesHistoryReportStock::ProcessQtyValueArrayMonthly( int nDepth, int nBranchIdx, CEntityTreeStockSalesHistoryMonthlyNode BlockTotalsNode, CEntityTreeStockSalesHistoryMonthlyNode PercentTotalsNode )
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
		
		CEntityTreeStockSalesHistoryMonthlyNode CurrentNode;
		m_QtyValueTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLine( CurrentNode, PercentTotalsNode );		
		else
			ProcessQtyValueArrayMonthly( nDepth + 1, m_QtyValueTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode, PercentTotalsNode );
	}

	if ( bTotal == TRUE )
		WriteTotals( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CSalesHistoryReportStock::ProcessQtyValueLine( int nFieldType, int nBranchIdx, int nNodeIdx )
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
			m_strLabelDate = oleDate.Format ( "%d/%m/%Y" );
		}
		break;

	case SH_FIELD_TIME:
		{
			CString strLabel = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );

			m_strLabelTime.Format ( "%s:%s:%s",
				(const char*) strLabel.Mid(1,2),
				(const char*) strLabel.Mid(3,2),
				(const char*) strLabel.Right(2) );
				
			if ( strLabel.Left( 1 ) == "1" )
				m_strLabelTime += "*";
			if ( strLabel.Left( 1 ) == "2" )
				m_strLabelTime += "**";
		}
		break;


	case SH_FIELD_DBASE:
		{
			int nSortIdx = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			int nDbIdx = dbDatabase.GetDbIdxFromSortIdx( nSortIdx );
		 	
			if ( ( nDbIdx >= 0 ) && ( nDbIdx < dbDatabase.GetSize() ) )
			{
				m_strLabelDBase = dbDatabase.GetName( nDbIdx );
				nLineDbNo = dbDatabase.GetDbNo( nDbIdx );
				m_nApparentTableNo = dbDatabase.GetDbNo( nDbIdx );
			}
			else
				m_strLabelDBase = "";
		}
		break;

	case SH_FIELD_CODE:
	case SH_FIELD_CODE_ONLY:
		m_strLabelStockCode = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_DESCRIPTION:
		m_strLabelDescription = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
		break;

	case SH_FIELD_SITE:
		{
			int nSortIdx = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			int nSiteIdx = dbSite.GetSiteIdxFromSortIdx( nSortIdx );

			if ( ( nSiteIdx >= 0 ) && ( nSiteIdx < dbSite.GetSize() ) )
			{
				m_strLabelSite = dbSite.GetName( nSiteIdx );
				nLineDbNo = dbSite.GetDbNo( nSiteIdx );
				m_nApparentTableNo = dbSite.GetSiteNo( nSiteIdx );
			}
			else
				m_strLabelSite = "None";
		}
		break;

	case SH_FIELD_SP:
		{
			int nSortIdx = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			int nSpIdx = dbStockpoint.GetSpIdxFromSortIdx( nSortIdx );

			if ( ( nSpIdx >= 0 ) && ( nSpIdx < dbStockpoint.GetSize() ) )
			{
				m_strLabelStockpoint = dbStockpoint.GetName( nSpIdx );
				nLineDbNo = dbStockpoint.GetDbNo( nSpIdx );
				m_nApparentTableNo = dbStockpoint.GetSpNo( nSpIdx );
			}
			else
				m_strLabelStockpoint = "";
		}
		break;

	case SH_FIELD_CAT_SALE:
		{
			int nCategory = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			m_strLabelCategorySale = DataManager.Category.GetText( nCategory );
		}
		break;

	case SH_FIELD_CAT_ITEM:
		{
			int nCategory = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			m_strLabelCategoryItem = DataManager.Category.GetText( nCategory );
		}
		break;

	case SH_FIELD_CUSTOM1:
	case SH_FIELD_CUSTOM2:
	case SH_FIELD_CUSTOM3:
	case SH_FIELD_CUSTOM4:
	case SH_FIELD_CUSTOM5:
		{
			int nFieldNo = ( nFieldType - SH_FIELD_CUSTOM1 ) + 1;
			int n = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
				
			if ( CUSTOM_FIELD_SORTIDX_NA == n )
			{
				m_strLabelCustomField[ nFieldNo ] = DataManager.CustomFieldManager.GetFieldNameByFieldNo( nFieldNo );
				m_strLabelCustomField[ nFieldNo ] += " N/A";
			}
			else if ( ( n < 0 ) || ( n >= DataManager.CustomFieldValue[ nFieldNo ].GetSize() ) )
			{
				m_strLabelCustomField[ nFieldNo ] = DataManager.CustomFieldManager.GetFieldNameByFieldNo( nFieldNo );
				m_strLabelCustomField[ nFieldNo ] = " Unknown";
			}
			else
				m_strLabelCustomField[ nFieldNo ] = DataManager.CustomFieldValue[ nFieldNo ].GetName( n );
		}
		break;
			
	case SH_FIELD_SUPP:
		{
			int nSuppSortIdx = m_QtyValueTable.GetLabelInt( nBranchIdx, nNodeIdx );
			int nSuppIdx = m_SupplierReportMap.GetSuppIdxBySortIdx( nSuppSortIdx );

			if ( nSuppIdx == 0 )
				m_strLabelSuppName = "Unspecified";
			else if ( ( nSuppIdx >= 1 ) && ( nSuppIdx <= DataManager.Supplier.GetSize() ) )
				m_strLabelSuppName = DataManager.Supplier.GetName( nSuppIdx - 1 );
			else
				m_strLabelSuppName = "Unknown";
		}
		break;

	case SH_FIELD_SUPPREF:
		m_strLabelSuppRef = m_QtyValueTable.GetLabelString( nBranchIdx, nNodeIdx );
		break;
	}

	if ( (nLineDbNo != -1) && ( nLineDbNo != dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) ) )
	{
		int nDbIdx;
		if ( dbDatabase.FindDatabaseByNumber( nLineDbNo, nDbIdx ) == TRUE )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

			if ( m_bApparent == TRUE )
			{
				int nStartIdx, nEndIdx;
				dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartIdx, nEndIdx );

				for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
				{
					if ( MatchStockpoint( nSpIdx ) == TRUE )
						DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
				}
			}
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportStock::CreateReportLine(CEntityTreeStockSalesHistoryNormalNode& CurrentNode, CEntityTreeStockSalesHistoryNormalNode& PercentTotalsNode)
{
	double dSalesQty = CurrentNode.GetQty();
	double dSalesVal = CurrentNode.GetTotalVal();
	double dSalesCost = CurrentNode.GetCost();
	double dSalesTax = CurrentNode.GetTaxAmount();
	double dWasteCost = CurrentNode.GetWasteCost();

	double dGroupSalesQty = PercentTotalsNode.GetQty();
	double dGroupSalesVal = PercentTotalsNode.GetTotalVal();
	double dGroupSalesCost = PercentTotalsNode.GetCost();
	double dGroupSalesTax = PercentTotalsNode.GetTaxAmount();
	double dGroupWasteCost = PercentTotalsNode.GetWasteCost();

	double dGP = dSalesVal - (dSalesCost + dSalesTax + dWasteCost);
	double dGroupGP = dGroupSalesVal - (dGroupSalesCost + dGroupSalesTax + dGroupWasteCost);

	int nWeeklyStartOffset = 0;
	int nWeeklyEndOffset = 0;

	switch (m_nReportMode)
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
		nWeeklyStartOffset = m_QtyValueTable.GetWeeklyStartOffset(m_oleWeekStart);
		nWeeklyEndOffset = m_QtyValueTable.GetWeeklyEndOffset(m_oleWeekStart);
		break;
	}

	CCSV csvOut('\t', '"', TRUE, FALSE, FALSE);

	double dApparentQty = 0.0;
	int nSubUnits = 1;
	AppendReportLineFields(csvOut, dApparentQty, nSubUnits);

	switch (m_nReportMode)
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		CurrentNode.AppendDaily(m_nReportMode, nSubUnits, csvOut, nWeeklyStartOffset, nWeeklyEndOffset);
		break;
	}

	{
		CString strQty;
		::GetStockLevelString(dSalesQty, nSubUnits, strQty);
		csvOut.Add(strQty);
	}

	switch (m_nReportMode)
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
		CurrentNode.AppendDaily(m_nReportMode, 1, csvOut, nWeeklyStartOffset, nWeeklyEndOffset);
		break;
	}

	csvOut.Add(dSalesVal, SysInfo.GetDPValue());

	if (FALSE == m_bEstimatedProfit)
	{
		csvOut.Add(ReportHelpers.CalcAverage(dSalesQty, dSalesVal), SysInfo.GetDPValue());
	}
	else
	{
		csvOut.Add(dSalesTax, SysInfo.GetDPValue());
		csvOut.Add(dSalesCost, SysInfo.GetDPValue());
		csvOut.Add(dWasteCost, SysInfo.GetDPValue());
		csvOut.Add(dGP, SysInfo.GetDPValue());
		csvOut.Add(ReportHelpers.CalcGPPercent(dSalesVal - dSalesTax, 0, dSalesCost + dWasteCost), 2);
	}

	//PERCENTAGES
	if (m_bPercentSale == TRUE)
	{
		double dPercent = 0.0;
		if (dGroupSalesVal != 0.0)
		{
			dPercent = (dSalesVal * 100.0) / dGroupSalesVal;
		}

		csvOut.Add(dPercent, 2);
	}

	if (TRUE == m_bEstimatedProfit)
	{
		double dPercent = 0.0;
		if (dGroupGP != 0.0)
		{
			dPercent = (dGP * 100.0) / dGroupGP;
		}

		csvOut.Add(dPercent, 2);
	}

	//APPARENT STOCK
	if (m_bApparent == TRUE)
	{
		CString strQty;
		::GetStockLevelString(dApparentQty, nSubUnits, strQty);
		csvOut.Add(strQty);
	}

	if (SALESHISTORY_REPORTMODE_CHART != m_nReportMode)
	{
		m_ReportFile.WriteLine(csvOut.GetLine());
	}
	else
	{
		AddToChart(dSalesQty, dSalesVal, csvOut.GetLine());
	}
}

/**********************************************************************/

void CSalesHistoryReportStock::AppendReportLineFields( CCSV& csvOut, double& dApparentQty, int& nSubUnits )
{
	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		switch ( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_MONTH:			csvOut.Add( m_strLabelMonth );			break;
		case SH_FIELD_WEEKDAY:			csvOut.Add( m_strLabelWeekDay );		break;
		case SH_FIELD_DATE_BUSINESS:	csvOut.Add( m_strLabelDate );			break;
		case SH_FIELD_TIME:				csvOut.Add( m_strLabelTime );			break;
		case SH_FIELD_DBASE:			csvOut.Add( m_strLabelDBase );			break;
		case SH_FIELD_SITE:				csvOut.Add( m_strLabelSite );			break;
		case SH_FIELD_SP:				csvOut.Add( m_strLabelStockpoint );		break;
		case SH_FIELD_CAT_SALE:			csvOut.Add( m_strLabelCategorySale );	break;
		case SH_FIELD_CAT_ITEM:			csvOut.Add( m_strLabelCategoryItem );	break;
		case SH_FIELD_SUPP:				csvOut.Add( m_strLabelSuppName);		break;
		case SH_FIELD_SUPPREF:			csvOut.Add( m_strLabelSuppRef);			break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			{
				int nFieldNo = m_arrayReportFields.GetAt(nPos) + 1 - SH_FIELD_CUSTOM1;
				csvOut.Add( m_strLabelCustomField[ nFieldNo ] );
			}
			break;
			
		case SH_FIELD_WEEK:			
			csvOut.Add( m_nLabelWeekNo );
			csvOut.Add( m_strLabelWeek );
			break;
					
		case SH_FIELD_CODE:
			{
				CString strDescription = "";
				GetStockCodeDisplayInformation( m_strLabelStockCode, strDescription, dApparentQty, nSubUnits );
				csvOut.Add ( ReportHelpers.GetDisplayStockCode ( m_strLabelStockCode ) );
				csvOut.Add ( strDescription );
			}
			break;

		case SH_FIELD_CODE_ONLY:
			csvOut.Add ( ReportHelpers.GetDisplayStockCode ( m_strLabelStockCode ) );
			break;

		case SH_FIELD_DESCRIPTION:
			{
				int nLen = m_strLabelDescription.GetLength();
				if ( nLen >= 2 )
					csvOut.Add( m_strLabelDescription.Right( nLen - 1 ) );
				else
					csvOut.Add( "" );
			}
			break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportStock::GetStockCodeDisplayInformation(const char* szStockCode, CString& strDescription, double& dApparentQty, int& nSubUnits)
{
	dApparentQty = 0.0;
	nSubUnits = 1;

	int nStockIdx;
	if (DataManager.Stock.FindStockItemByCode(szStockCode, nStockIdx) == TRUE)
	{
		strDescription = DataManager.Stock.GetDescription(nStockIdx);
		bool bSubUnits = StockOptions.GetStockSubUnitsFlag();

		//AVOID READING THE WHOLE STOCK RECORD UNLESS WE NEED TO
		if (bSubUnits || m_bApparent)
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt(nStockIdx, StockRecord);

			if (bSubUnits)
			{
				nSubUnits = StockRecord.GetSubUnits(TRUE);
			}

			if (m_bApparent)
			{
				CStockLevelsCSVRecord levels;

				switch (m_nApparentConLevel)
				{
				case NODE_STOCKPOINT:
					DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, m_nApparentTableNo, levels);
					break;

				case NODE_SITE:
					DataManager.Stock.GetSiteTotals(nStockIdx, StockRecord, m_nApparentTableNo, levels);
					break;

				case NODE_DATABASE:
					DataManager.Stock.GetDatabaseTotals(nStockIdx, StockRecord, levels);
					break;
				}

				dApparentQty = levels.GetApparentQty();
			}
		}
	}
	else
	{
		strDescription = "Unknown";
	}
}

/**********************************************************************/

const char* CSalesHistoryReportStock::GetHeaderLine(int nDepth)
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

		case SH_FIELD_DATE_BUSINESS:
			csvHeader.Add ( strSpace + m_strLabelDate );
			break;

		case SH_FIELD_TIME:
			csvHeader.Add ( strSpace + m_strLabelTime );
			break;

		case SH_FIELD_DBASE:
			csvHeader.Add ( strSpace + m_strLabelDBase );
			break;

		case SH_FIELD_CODE:
		case SH_FIELD_CODE_ONLY:
			csvHeader.Add ( strSpace + ReportHelpers.GetDisplayStockCode ( m_strLabelStockCode ) );
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

		case SH_FIELD_SITE:
			csvHeader.Add ( strSpace + m_strLabelSite );
			break;

		case SH_FIELD_SP:
			csvHeader.Add ( strSpace + m_strLabelStockpoint );
			break;

		case SH_FIELD_CAT_SALE:
			csvHeader.Add ( strSpace + m_strLabelCategorySale );
			break;

		case SH_FIELD_CAT_ITEM:
			csvHeader.Add ( strSpace + m_strLabelCategoryItem );
			break;

		case SH_FIELD_SUPP:
			csvHeader.Add ( strSpace + m_strLabelSuppName );
			break;

		case SH_FIELD_SUPPREF:
			csvHeader.Add ( strSpace + m_strLabelSuppRef );
			break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			{
				int nFieldNo = nFieldType + 1 - SH_FIELD_CUSTOM1;
				csvHeader.Add( strSpace + m_strLabelCustomField[ nFieldNo ] );
			}
			break;
		}
	}
	m_strHeader = csvHeader.GetLine();
	return m_strHeader;
}

/**********************************************************************/

void CSalesHistoryReportStock::WriteTotals( int nDepth, CEntityTreeStockSalesHistoryNormalNode& BlockTotalsNode )
{
	double dSalesQty = BlockTotalsNode.GetQty();
	double dSalesVal = BlockTotalsNode.GetTotalVal();
	double dSalesCost = BlockTotalsNode.GetCost();
	double dSalesTax = BlockTotalsNode.GetTaxAmount();
	double dWasteCost = BlockTotalsNode.GetWasteCost();
	
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
			WriteReportLine( line.GetLine() );
		}
		m_arrayBufferedLines.RemoveAll();
		m_arraySalesChart.RemoveAll();
		return;
	}

	//CREATE UNDERLINE FOR TOTALS
	m_ReportFile.WriteLine( m_strUnderline );
	
	CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );

	{
		CString strTotal;
		if ( nDepth == 0 )
			strTotal = "<..>Grand Total";
		else
		{
			strTotal = "<..>Totals (";
			strTotal += GetHeaderLine ( nDepth );
			strTotal += " )";
		}
	
		csvOut.Add ( strTotal );
	}

	for ( int n = 0; n < m_nColumnsBeforeTotal - 1; n++ )
		csvOut.Add ( "" );

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		BlockTotalsNode.AppendDaily( m_nReportMode, 1, csvOut );
		break;
	}

	csvOut.Add( dSalesQty, SysInfo.GetDPQty() );

	switch( m_nReportMode )
	{
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
		BlockTotalsNode.AppendDaily( m_nReportMode, 1, csvOut );
		break;
	}

	csvOut.Add( dSalesVal, SysInfo.GetDPValue() );

	//ESTIMATED PROFIT OR AVERAGE COST
	if ( m_bEstimatedProfit == TRUE )
	{
		csvOut.Add ( dSalesTax, SysInfo.GetDPValue() );
		csvOut.Add ( dSalesCost, SysInfo.GetDPValue() );
		csvOut.Add ( dWasteCost, SysInfo.GetDPValue() );
		csvOut.Add ( dSalesVal - dSalesTax - dSalesCost - dWasteCost, SysInfo.GetDPValue() );
		csvOut.Add ( ReportHelpers.CalcGPPercent ( dSalesVal - dSalesTax, 0.0, dSalesCost + dWasteCost ), 2 );
	}	
	else	
		csvOut.Add ( ReportHelpers.CalcAverage ( dSalesQty, dSalesVal ), SysInfo.GetDPValue() );

	WriteReportLine( csvOut.GetLine() );
	WriteBlankLine( FALSE );
}

/**********************************************************************/

void CSalesHistoryReportStock::AddToChart( double dQty, double dValue, const char* szLine )
{
	double dToInsert = m_chartinfo.IsSortedByValue() ? dValue : dQty;
	
	if ( m_chartinfo.IsBottom100() )
		dToInsert = -dToInsert;
		
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
	else if ( nStartIdx <m_chartinfo.GetChartSize() )
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

void CSalesHistoryReportStock::WriteHeader( int nDepth )
{
	CString strTemp = GetHeaderLine(nDepth);
	if ( strTemp.GetLength() >= 1 ) strTemp = strTemp.Right( strTemp.GetLength() - 1 );
	CString strHeader = "<..>";
	strHeader += strTemp;
	WriteBlankLine( TRUE );
	WriteReportLine( strHeader );
	WriteReportLine( "<LI>" );
}

/**********************************************************************/
