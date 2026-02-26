#pragma once
/**********************************************************************/
#include "BritanniaHotelsExportArray.h"
#include "DepartmentSetCSVArray.h"
#include "DepartmentSorter.h"
#include "EntityTree.h"
#include "EposSelectArray.h"
#include "LocationSelectorEntity.h"
#include "PosTrayTask.h"
#include "ReportFilters.h"
#include "ReportPluInfoFinder.h"
#include "SalesHistoryCSVArrayPlu.h"
#include "SessionCSVArray.h"
#include "TableReportHelpers.h"
#include "TermFileListInfo.h"
#include "TimeMap.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/
#define TOTAL_TYPE_NONE 0
#define TOTAL_TYPE_BASEQTY 1
#define TOTAL_TYPE_MODQTY 2
#define TOTAL_TYPE_SALES 3
#define TOTAL_TYPE_DISCOUNT 4
#define TOTAL_TYPE_NET 5
#define TOTAL_TYPE_AVERAGE 6
#define TOTAL_TYPE_TAX 7
#define TOTAL_TYPE_COST 8
#define TOTAL_TYPE_GP 9
#define TOTAL_TYPE_GP_PERCENT 10
#define TOTAL_TYPE_PERCENT_OF_SALE 11
#define TOTAL_TYPE_PERCENT_OF_GP 12
#define TOTAL_TYPE_WASTE 13
#define TOTAL_TYPE_NETOFTAX 14
/**********************************************************************/
#define TOTAL_TYPE_DAILY 15
/**********************************************************************/
#define TOTAL_TYPE_PREMIUM 16
/**********************************************************************/

struct CSalesHistoryReportPluConsolidationOptions
{
public:
	CSalesHistoryReportPluConsolidationOptions();

public:
	bool m_bWantTranServer;
	bool m_bWantItemServer;
	bool m_bWantCustomer;
	bool m_bIsUnsoldItemReport;
	bool m_bWantDeptGrpSale;
	bool m_bWantDeptGrpItem;
	bool m_bWantPluInfo;
	bool m_bWantRepGrp;
	bool m_bWantConGrp;
	bool m_bWantTable;
};

/**********************************************************************/

struct CSalesHistoryReportPluColumn
{
public:
	CSalesHistoryReportPluColumn();
	CSalesHistoryReportPluColumn( const char* szText, int nAlign, int nWidth, int nTotalType = TOTAL_TYPE_NONE, int nBlock = 1 );

public:
	CString m_strText;
	int m_nAlign;
	int m_nWidth;
	int m_nTotalType;
	int m_nBlock;
};

/**********************************************************************/

class CSalesHistoryReportPlu 
{
public:
	CSalesHistoryReportPlu();

#ifndef POSTRAY_UTILITY
	void Reset( CSalesHistoryCSVRecordPlu& ReportRecord, CLocationSelectorEntity& LocSelEntity, bool bEposSales, bool bManualSales, bool bImportSales, bool bGuestToTerminal );
#else
	void Reset( CSalesHistoryCSVRecordPlu& ReportRecord, CPosTrayTask& EmailTask );
#endif
	
	void SetChartDetails( CSalesHistoryCSVRecordPlu& ReportRecord );

	//USED TO IDENTIFY ITEMS THAT SHOULD NOT BE DELETED
	void GetSoldItemsList( int nDbIdx, COleDateTime dateFrom, CByteArray& FlagArray );

public:
	void SetReportDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice ); 
	void SetReportDates ( COleDateTime dateFrom, COleDateTime dateTo, COleDateTime dateFrom2, COleDateTime dateTo2, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice ); 
	void SelectDateBlock(int n);
	void SetTermDateType(int n) { m_nTermDateType = n;  }
		
	void SetPluRangeFlag ( bool bFlag ) { m_bItemRange = bFlag; }
	void SetPluFrom ( __int64 nPluNo ) { m_nItemFrom = nPluNo; }
	void SetPluTo (  __int64 nPluNo ) { m_nItemTo = nPluNo; }
	void SetDepartmentByItemFlag( bool b ) { m_bDepartmentByItem = b; }
	void SetSaleTimeFlag( bool bFlag ) { m_bSaleTime = bFlag; }
	void SetTaxFilter( int nFilter ) { m_nTaxFilter = nFilter; }
	void SetPriceLevelFilter( int nFilter ) { m_nPriceLevelFilter = nFilter; }
	void SetModifierFilter( int nFilter ) { m_nModifierFilter = nFilter; }
	
	void SetEstimatedProfitFlag ( bool bFlag ) { m_bEstimatedProfit = bFlag; }
	void SetPercentSaleFlag ( bool bFlag ) { m_bPercentSale = bFlag; }
	void SetCustomerFlag( bool bFlag ){ m_bCustomer = bFlag; }
	void SetVIPSalesFlag( bool bFlag ){ m_bVIPSales = bFlag; }
	
	void SetConLevelTitleForGraph( const char* szTitle ) { m_strConLevelTitleForGraph = szTitle; }
	void SetSessionTitle( const char* szTitle ) { m_strSessionTitle = szTitle; }
	void WriteTerminalList();

	void SetTableFilter( CTableFilter info );
	
	void ResetInternal( CSalesHistoryCSVRecordPlu& ReportRecord );

	const char* GetEmailSubject();

	bool GetEstimatedProfitFlag() { return m_bEstimatedProfit; }
	bool GetPercentSaleFlag() { return m_bPercentSale; }
	
public:
	bool CreatePluSalesHistoryReport();
	int CreateGraph();
	bool PrepareFields();
	void Consolidate();
	void ConsolidateInternal( int nBlock, CSalesHistoryReportPluConsolidationOptions& ConOptions );
	
public:
	bool GotDataLine() { return m_ReportFile.GotDataLine(); }
	void SetCreateMD5HashFlag(bool b) { m_ReportFile.SetCreateMD5HashFlag(b); }
	CString GetMD5Hash() { return m_ReportFile.GetMD5Hash(); }

private:
	void PrepareReportColumn( CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn>& arrayColumns, const char* szText, int nAlight, int nWidth, int nTotalType = TOTAL_TYPE_NONE );
	void AddReportColumn( CSalesHistoryReportPluColumn& Column );
	void AddReportColumnGroup( int nMasterColumn, int nBlock, CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn>& arraySubColumns );
	bool CreateCustomReportInternal();
	bool CreateTaxReportInternal();
	int CreateDepartmentGraphInternal();
	int CreateDailyGraphInternal();
	int CreateMonthlyGraphInternal();
	int CreateWeeklyGraphInternal();
	bool CreateUnsoldReportInternal();
	bool CreateUnsoldMonthlyReportInternal();

private:
	bool CheckPMSAccountDateRange( CSSFile& fileSales, int nPMSDayFrom, int nPMSDayTo );
	
private:
	void ProcessQtyValueArrayNormal( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryNormalNode BlockTotalsNode, CEntityTreePluSalesHistoryNormalNode PercentTotalsNode );
	void ProcessQtyValueArrayTwoBlock( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryTwoBlockNode BlockTotalsNode, CEntityTreePluSalesHistoryTwoBlockNode PercentTotalsNode );
	void ProcessQtyValueArrayWeeklyOneTotal( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryWeeklyOneTotalNode BlockTotalsNode, CEntityTreePluSalesHistoryWeeklyOneTotalNode PercentTotalsNode );
	void ProcessQtyValueArrayWeeklyTwoTotal(int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryWeeklyTwoTotalNode BlockTotalsNode, CEntityTreePluSalesHistoryWeeklyTwoTotalNode PercentTotalsNode);
	void ProcessQtyValueArrayMonthly( int nDepth, int nBranchIdx, CEntityTreePluSalesHistoryMonthlyNode BlockTotalsNode, CEntityTreePluSalesHistoryMonthlyNode PercentTotalsNode );
	void ProcessQtyValueLine( int nFieldType, int nBranchIdx, int nNodeIdx );
	void CreateReportLineNormal( CEntityTreePluSalesHistoryNormalNode CurrentNode, CEntityTreePluSalesHistoryNormalNode PercentTotalsNode );
	void CreateReportLineTwoBlock( CEntityTreePluSalesHistoryTwoBlockNode CurrentNode, CEntityTreePluSalesHistoryTwoBlockNode PercentTotalsNode );
	void CreateReportLineWeeklyOneTotal( CEntityTreePluSalesHistoryWeeklyOneTotalNode CurrentNode, CEntityTreePluSalesHistoryWeeklyOneTotalNode PercentTotalsNode );
	void CreateReportLineWeeklyTwoTotal(CEntityTreePluSalesHistoryWeeklyTwoTotalNode CurrentNode, CEntityTreePluSalesHistoryWeeklyTwoTotalNode PercentTotalsNode);
	void CreateReportLineMonthly( CEntityTreePluSalesHistoryMonthlyNode CurrentNode, CEntityTreePluSalesHistoryMonthlyNode PercentTotalsNode );
	void AppendReportLineFields( CCSV& csvOut );
		
private:
	const char* GetRepMonth( COleDateTime date, bool bShortName, bool bFullLine );
	const char* GetReportTitle( const char* szReportName );
	const char* GetLabelTimeString();
	
private:
	const char* GetHeaderLine( int nDepth );
	void WriteTotalsNormal( int nDepth, CEntityTreePluSalesHistoryNormalNode& BlockTotalsNode );
	void WriteTotalsTwoBlock( int nDepth, CEntityTreePluSalesHistoryTwoBlockNode& BlockTotalsNode );
	void WriteTotalsWeeklyOneTotal( int nDepth, CEntityTreePluSalesHistoryWeeklyOneTotalNode& BlockTotalsNode );
	void WriteTotalsWeeklyTwoTotal(int nDepth, CEntityTreePluSalesHistoryWeeklyTwoTotalNode& BlockTotalsNode);
	void WriteTotalsMonthly( int nDepth, CEntityTreePluSalesHistoryMonthlyNode& BlockTotalsNode );
	void AppendTotalsNormal( CCSV& csv, CEntityTreePluSalesHistoryNormalNode& LineTotals, CEntityTreePluSalesHistoryNormalNode& BlockTotals, bool bTotals, int nStartOffset = 0, int nEndOffset = 0 );
	void AppendTotalsTwoBlock( CCSV& csv, CEntityTreePluSalesHistoryTwoBlockNode& LineTotals, CEntityTreePluSalesHistoryTwoBlockNode& BlockTotals, bool bTotals );
	void AppendDateBlockTotals( CCSV& csv, int nBlock, CEntityTreePluSalesHistoryBlock* pLineTotals, CEntityTreePluSalesHistoryBlock* pBlockTotals, bool bTotals, int& nDailyColStart, int nDailyColCount = 0, int nStartOffset = 0, int nEndOffset = 0 );
	void WriteHeader( int nTotal );
	
private:
	void AddToChart( double dQty, double dValue, const char* szLine );
	void CreateDummySales();
	void WriteUnsoldItems();
	
public:
	void ResetForBritannia( CLocationSelectorEntity& LocSelEntity, bool bGuestToTerminal );
	void ConsolidateForBritannia( CBritanniaHotelsExportArray& ExportArray );
	void CreateReportForBritannia( int nLocIdx, CBritanniaHotelsExportArray& ExportArray, CWnd* pParentWnd );

public:
	CReportFilters m_ReportFilters;
	
private:
	//TIME AND DATE FILTERS
	bool m_bEODMode;
	CTimeSliceMapCSVRecord m_TimeSliceMap;
	COleDateTime m_oleDateFromBlock1;
	COleDateTime m_oleDateToBlock1;
	COleDateTime m_oleDateFromBlock2;
	COleDateTime m_oleDateToBlock2;
	CSessionCSVRecord m_ReportSession;
	
	//PLU AND DEPARTMENT FILTERS
	bool m_bItemRange;
	__int64 m_nItemFrom;
	__int64 m_nItemTo;
	bool m_bDepartmentByItem;
	bool m_bSaleTime;
	int m_nTaxFilter;
	int m_nPriceLevelFilter;
	int m_nModifierFilter;

	//LOCATION AND SALES TO INCLUDE	
	CEposSelectArray m_SelectArray;
	bool m_bEposSales;
	bool m_bManualSales;
	bool m_bImportSales;
	bool m_bGuestToTerminal;
	int m_nTermDateType;
	
	//STOCKMAN NEEDS THIS FOR DATABASE/SYSTEM SALES
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;

	//REPORT TYPE AND MISC SETTINGS
	int m_nReportType;
	int m_nReportMode;
	CString m_strReportName;
	bool m_bEstimatedProfit;
	bool m_bPercentSale;
	int m_nPercentDepth;
	bool m_bHasModifierField;
	bool m_bCustomer;
	bool m_bVIPSales;
	
	//CHART INFO
	CSalesHistoryCSVRecordPlu m_chartinfo;

	//CONSOLIDATED SALES	
	CEntityTreePluSalesHistory m_QtyValueTable;
		
	//REPORT FIELDS
	CWordArray m_arrayReportFields;
	CByteArray m_arrayHeaderFlags;
	CByteArray m_arrayTotalFlags;

	//COLUMN TYPES
	CArray<CSalesHistoryReportPluColumn,CSalesHistoryReportPluColumn> m_arrayReportColumns;
	CCSV m_csvUnderline;
	int m_nFirstTotalColumn;

	//SPECIAL CASE ACCUMULATORS 
	CArray<double,double> m_arraySalesChart;
	CArray<bool,bool> m_arrayUnsoldItems;
	
	//DEPT SORTER FOR THE UNSOLD ITEMS REPORT
	CDepartmentSorter m_DepartmentSorter;

	//BUFFER FOR PERCENTAGE SALES AND SALES CHARTS
	CStringArray m_arrayBufferedLines;
	
	//THE ACTUAL REPORT FILE
	CEposReportFile m_ReportFile;

	//NAMES OF WEEKDAYS
	CString m_strWeekDays[7];

	//CHECK THAT LABELS ARE THE RIGHT LENGTH BEFORE DECODING
	int m_nLabelLength;

	//VARIOUS TITLES
	CString m_strSessionTitle;
	CString m_strTitle;
	CString m_strConLevelTitleForGraph;
	CString m_strRepMonth;
	CString m_strHeader;
	CString m_strEmailSubject;

	//PROGRESS MONITORING
	int m_nProgress;

	//SORT BY TIME SLOT
	CTimeMap m_TimeMap;

	//LABEL DECODING
	CString m_strLabelMonth;
	int m_nLabelWeekNo;
	CString m_strLabelWeek;
	CString m_strLabelWeekDay;
	CString m_strLabelBusinessDate;
	CString m_strLabelCalendarDate;
	int m_nLabelTimeHour;
	int m_nLabelTimeMinute;
	int m_nLabelTimeSecond;
	int m_nLabelTimeStars;
	CString m_strLabelTimeString;
	int m_nLabelSeqNo;
	CString m_strLabelDBase;
	CString m_strLabelLoc;
	CString m_strLabelTerminal;
	int m_nLabelTableNo;
	CString m_strLabelTableName;
	CString m_strLabelTableGrpName;
	int m_nLabelDeptNoSale;
	CString m_strLabelDeptNameSale;
	int m_nLabelDeptNoItem;
	CString m_strLabelDeptNameItem;
	int m_nLabelDeptGrpNoSale;
	CString m_strLabelDeptGrpNameSale;
	int m_nLabelDeptGrpNoItem;
	CString m_strLabelDeptGrpNameItem;
	int m_nLabelRepGrpNo;
	CString m_strLabelRepGrpName;
	int m_nLabelConGrpNo;
	CString m_strLabelConGrpName;
	__int64 m_nLabelPluNo;
	CString m_strLabelDescription;
	CString m_strLabelCashRSPText;
	CString m_strLabelAttributeText1;
	CString m_strLabelSortCode;
	int m_nLabelTranServerNo;
	CString m_strLabelTranServerName;
	int m_nLabelItemServerNo;
	CString m_strLabelItemServerName;
	CString m_strLabelCustomerType;
	CString m_strLabelCustomerName;
	CString m_strLabelPluSupp;
	int m_nLabelACatNoSale;
	CString m_strLabelACatSale;
	int m_nLabelACatNoItem;
	CString m_strLabelACatItem;
	int m_nLabelLevel;
	int m_nLabelTaxBand;
	int m_nLabelModifier;
	CString m_strLabelRepPoint;
	COleDateTime m_oleWeekStart;

private:
	CReportPluInfoFinder m_PluInfoFinder;
	CTableReportHelpers m_TableReportHelpers;
	CTableFilter m_TableFilter;
	bool m_bWantTableName;
};

/**********************************************************************/
