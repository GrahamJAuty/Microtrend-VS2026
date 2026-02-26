#pragma once
/**********************************************************************/
#include <afxtempl.h>
/**********************************************************************/
#include "DateRangeOrRegionPluInfo.h"
#include "DepartmentSetCSVArray.h"
#include "DepartmentSorter.h"
#include "ReportCostfinder.h"
#include "SessionDateTimeFilter.h"
/**********************************************************************/
#define SALESHISTORY_DATERANGE_LYCOMPARE 0
#define SALESHISTORY_DATERANGE_TRADING_PLU 1
#define SALESHISTORY_DATERANGE_TRADING_BAND 2
#define SALESHISTORY_DATERANGE_TRADING_DEPT 3
/**********************************************************************/

struct CSalesHistoryDateRangeCell
{
public:
	CSalesHistoryDateRangeCell();

public:
	int m_nTaxDateCode;
	int m_nTaxBand;
	double m_dSalesQty;
	double m_dSalesValue;
	double m_dWasteQty;
	double m_dCost;
	CString m_strExtraData;
};

/**********************************************************************/

class CSalesHistoryDateRangePlu  
{
public:
	CSalesHistoryDateRangePlu();
	~CSalesHistoryDateRangePlu();

	void Configure( CDateRangeOrRegionColumnInfo& RegionInfo, CArray<CDateRangeInfo,CDateRangeInfo>& DateInfo, int nDeptFilter, int nTaxFilter, bool bItemRange, __int64 nItemFrom, __int64 nItemTo, bool bPriceBandSplit, CSessionCSVRecord& Session );
		
	void SetReportType( int n ) { m_nReportType = n; }
	void SetDeptFilter( int n ) { m_DepartmentSet.CreateDepartmentSet(n); }
	void SetZeroSkipType( int n ) { m_nZeroSkipType = n; }
	void SetSaleTimeFlag( bool b ){ m_bSaleTime = b; }

public:
	void Consolidate();
	bool CreateReport();
	int CreateGraph();

	CArray<CSalesHistoryDateRangeCell,CSalesHistoryDateRangeCell>* GetCellArray( int n );

private:
	bool CreateLYCompareReport();
	bool CreateTradingDeptReport();
	bool CreateTradingPluReport();
	int CreateTradingDeptGraph();
	
private:
	const char* GetReportTitle( const char* szReportName );
	
	void RebuildFilter();
	void RebuildTotals();

	void GetFileListPlu ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	void ConsolidateSale( int nLocIdx, int nLevel, int nCol, int nRow, int nTaxDateCode, int nTaxBand, double dSalesQty, double dSalesValue, double dWasteQty );
	void GetQtyValueInfo( int nLevel, int nCol, int nRow, double& dSalesQty, double& dSalesValue, double& dWasteQty, double& dCost, double& dCellTax );
	
private:
	double GetPercentage( double dValue, double dTotalValue );
	void AddToCSV ( CCSV& csv, int nPos, double dToAdd, int nDP );
	const char* GetDeptText( int nDeptNo );
	int GetMinLevelsToDisplay( int nPluIdx );

private:
	bool IsFiltered();
	
private:
	int m_nReportType;
	int m_nZeroSkipType;

private:
	int m_nDeptFilter;
	CDepartmentSetCSVRecord m_DepartmentSet;
	
	bool m_bItemRange;
	__int64 m_nItemFrom;
	__int64 m_nItemTo;
	bool m_bSaleTime;
	
	int m_nTaxFilter;
	
	int m_nDeptSortDbIdx;
	bool m_bInvalidTotals;
	bool m_bInvalidFilter;
	bool m_bInvalidDeptSort;
	bool m_bPriceBandSplit;

	CSessionDateTimeFilter m_DateTimeFilter;

private:
	CString m_strTitle;
	CString m_strColumnName;
	CString m_strPriceText;
	CString m_strDeptText;
	
private:
	CReportFile m_ReportFile;

	CDateRangeOrRegionColumnInfo m_RegionInfo;

	void* m_pArrayCells[ MAX_PLU_PRICES + MAX_SBPLU_PRICES ];

	CEposSelectArray m_SelectArray;
	CArray<CDateRangeInfo,CDateRangeInfo> m_arrayDateRange;
	CArray<CDateRangeOrRegionPluInfo,CDateRangeOrRegionPluInfo> m_arrayPluInfo;
	CDepartmentSorter m_DepartmentSorter;
	CReportCostFinder m_ReportCostFinder;

	int m_nCols;
	int m_nRows;
	int m_nCells;
};

/**********************************************************************/
