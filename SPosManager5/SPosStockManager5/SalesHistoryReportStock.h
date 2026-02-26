#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySetCSVArray.h"
#include "CategorySorter.h"
#include "..\CommonStockTray\CustomField.h"
#include "EntityTree.h"
#include "EposSelectArray.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "ReportFiltersStock.h"
#include "SessionCSVArray.h"
#include "SupplierReportMap.h"
#include "SalesHistoryCSVArrayStock.h"
/**********************************************************************/

class CSalesHistoryReportStock 
{
public:
	CSalesHistoryReportStock();
	void Reset( CSalesHistoryCSVRecordStock& ReportRecord, CLocationSelectorEntity& LocSelEntity, bool bEposSales, bool bManualSales, bool bImportSales );
	void SetChartDetails( CSalesHistoryCSVRecordStock& ReportRecord );

	//USED TO IDENTIFY ITEMS THAT SHOULD NOT BE DELETED
	void GetSoldItemsList( int nDbIdx, COleDateTime dateFrom, CByteArray& FlagArray );

public:
	void SetDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession );

	void SetItemRangeFlag ( bool bFlag ) { m_bItemRange = bFlag; }
	void SetItemFrom ( const char* szItem ) { m_strItemFrom = szItem; }
	void SetItemTo ( const char* szItem ) { m_strItemTo = szItem; }
	void SetCategoryByItemFlag( bool b ) { m_bCategoryByItem = b; }
	void SetSupplierFilter( CWordArray& array );

	void SetEPOSSalesFlag ( bool bFlag ) { m_bEposSales = bFlag; }
	void SetManualSalesFlag ( bool bFlag ) { m_bManualSales = bFlag; }
	void SetImportSalesFlag ( bool bFlag ) { m_bImportSales = bFlag; }
	
	void SetPercentSaleFlag ( bool bFlag ) { m_bPercentSale = bFlag; }
	void SetEstimatedProfitFlag ( bool bFlag ) { m_bEstimatedProfit = bFlag; }
	void SetApparentFlag( bool bFlag ){ m_bApparent = bFlag; }

	void SetConLevelTitleForGraph( const char* szTitle ) { m_strConLevelTitleForGraph = szTitle; }
	void SetSessionTitle( const char* szTitle ) { m_strSessionTitle = szTitle; }
	void ShowStockpoints();

	bool MatchStockpoint( int nSpIdx );

public:
	bool CreateReport();
	int CreateGraph();
	bool PrepareFields();
	void Consolidate( bool bNoCategoryFilter = FALSE );

private:
	bool CreateCustomReportInternal();
	int CreateCategoryGraphInternal();
	int CreateDailyGraphInternal();
	int CreateMonthlyGraphInternal();
	int CreateWeeklyGraphInternal();
	bool CreateUnsoldReportInternal();
	bool CreateUnsoldMonthlyReportInternal();

private:
	void AddWeekdayColumns();
	void AddMonthdayColumns();

private:
	void ProcessQtyValueArrayNormal( int nDepth, int nBranchIdx, CEntityTreeStockSalesHistoryNormalNode BlockTotalsNode, CEntityTreeStockSalesHistoryNormalNode PercentTotalsNode );
	void ProcessQtyValueArrayWeekly( int nDepth, int nBranchIdx, CEntityTreeStockSalesHistoryWeeklyNode BlockTotalsNode, CEntityTreeStockSalesHistoryWeeklyNode PercentTotalsNode );
	void ProcessQtyValueArrayMonthly( int nDepth, int nBranchIdx, CEntityTreeStockSalesHistoryMonthlyNode BlockTotalsNode, CEntityTreeStockSalesHistoryMonthlyNode PercentTotalsNode );
	void ProcessQtyValueLine( int nFieldType, int nBranchIdx, int nNodeIdx );
	void CreateReportLine( CEntityTreeStockSalesHistoryNormalNode& CurrentNode, CEntityTreeStockSalesHistoryNormalNode& PercentTotalsNode );
	void AppendReportLineFields( CCSV& csvOut, double& dApparentQty, int& nSubUnits );
			
	void GetStockCodeDisplayInformation( const char* szStockCode, CString& strDescription, double& dApparentQty, int& nSubUnits );

private:
	const char* GetHeaderLine( int nDepth );
	void WriteTotals( int nDepth, CEntityTreeStockSalesHistoryNormalNode& BlockTotalsNode );
	void WriteHeader( int nDepth );

private:
	const char* GetReportTitle( const char* szReportName );
	const char* GetRepMonth( COleDateTime date, bool bShortName, bool bFullLine );
	
private:
	void AddToChart( double dQty, double dValue, const char* szLine );
	void CreateDummySales();
	void WriteUnsoldItems();

private:
	void WriteReportLine( const char* szLine );
	void WriteBlankLine( bool bOnlyIfNeeded );

public:
	CReportFiltersStock m_ReportFilters;

private:
	//STOCK CODE AND CATEGORY FILTERS
	bool m_bItemRange;
	CString m_strItemFrom;
	CString m_strItemTo;	
	bool m_bCategoryByItem;
	
	//SUPPLIER FILTER
	CWordArray m_SupplierIdxArray;
	CSupplierReportMap m_SupplierReportMap;
	
	//LOCATION AND SALES TO INCLUDE	
	bool m_bEposSales;
	bool m_bManualSales;
	bool m_bImportSales;
	
	//STOCKMAN NEEDS THIS FOR SELECTING STOCKPOINTS
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;

	//REPORT TYPE AND MISC SETTINGS
	int m_nReportType;
	int m_nReportMode;
	CString m_strReportName;
	bool m_bPercentSale;
	int m_nPercentDepth;
	bool m_bEstimatedProfit;
	
	bool m_bApparent;

	//STOCK LEVEL BY LINE
	int m_nApparentConLevel;
	int m_nApparentTableNo;

	//CHART INFO
	CSalesHistoryCSVRecordStock m_chartinfo;

	//REPORT FIELDS
	CWordArray m_arrayReportFields;
	CByteArray m_arrayHeaderFlags;
	CByteArray m_arrayTotalFlags;
	CString m_strUnderline;
	int m_nColumnsBeforeTotal;

	//CONSOLIDATED SALES
	CEntityTreeStockSalesHistory m_QtyValueTable;

	//SPECIAL CASE ACCUMULATORS 
	CArray<double,double> m_arraySalesChart;
	CArray<bool,bool> m_arrayUnsoldItems;
	
	//CATEGORY SORTER FOR THE UNSOLD ITEMS REPORT
	CCategorySorter m_CategorySorter;

	//BUFFER FOR PERCENTAGE SALES AND SALES CHARTS
	CStringArray m_arrayBufferedLines;
	
	//THE ACTUAL REPORT FILE
	CReportFile m_ReportFile;

	//NAMES OF WEEKDAYS
	CString m_strWeekDays[7];

	//INTELLIGENCE FOR AVOIDING MULTIPLE BLANK LINES
	bool m_bLastLineWasBlank;

	//VARIOUS TITLES
	CString m_strTitle;
	CString m_strSessionTitle;
	CString m_strConLevelTitleForGraph;
	CString m_strRepMonth;
	CString m_strHeader;	

	//PROGRESS MONITORING
	int m_nProgressTarget;
	int m_nProgressSoFar;

	//LABEL DECODING
	CString m_strLabelMonth;
	int m_nLabelWeekNo;
	CString m_strLabelWeek;
	CString m_strLabelWeekDay;
	CString m_strLabelDate;
	CString m_strLabelTime;
	CString m_strLabelDBase;
	CString m_strLabelStockCode;
	CString m_strLabelDescription;
	CString m_strLabelSite;
	CString m_strLabelStockpoint;
	CString m_strLabelCategorySale;
	CString m_strLabelCategoryItem;
	CString m_strLabelCustomField[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	CString m_strLabelSuppName;
	CString m_strLabelSuppRef;
	COleDateTime m_oleWeekStart;
	
private:
	__int64 m_nProgress;
};

/**********************************************************************/
#endif
/**********************************************************************/
