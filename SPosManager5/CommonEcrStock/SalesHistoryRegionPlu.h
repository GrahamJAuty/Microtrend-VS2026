#pragma once
/**********************************************************************/
#include <afxtempl.h>
/**********************************************************************/
#include "DateRangeOrRegionPluInfo.h"
#include "DepartmentSetCSVArray.h"
#include "DepartmentSorter.h"
#include "EposSelectArray.h"
#include "ReportConsolidationArray.h"
#include "SalesHistoryRegionLocColMap.h"
#include "SessionCSVArray.h"
#include "SessionDateTimeFilter.h"
#include "TermFileListInfo.h"
/**********************************************************************/
#define SALESHISTORY_REGION_PLU 0
#define SALESHISTORY_REGION_BAND 1
#define SALESHISTORY_REGION_DEPT 2
/**********************************************************************/

struct CSalesHistoryCell
{
	double m_dSalesQty;
	double m_dSalesValue;
	double m_dWasteQty;
};

/**********************************************************************/

class CSalesHistoryRegionPlu  
{
public:
	CSalesHistoryRegionPlu();
	~CSalesHistoryRegionPlu();

	void SetReportType( int n ) { m_nReportType = n; }
	void SetZeroSkipType( int n ) { m_nZeroSkipType = n; }
	void SetSaleTimeFlag( bool b ){ m_bSaleTime = b; }
	void Configure( COleDateTime& dateFrom, COleDateTime& dateTo, CArray<CDateRangeOrRegionColumnInfo,CDateRangeOrRegionColumnInfo>& arrayColumns, bool bByLocation, int nDeptFilter, int nTaxFilter, bool bItemRange, __int64 nItemFrom, __int64 nItemTo, bool bPriceBandSplit, CSessionCSVRecord& Session );
	
public:
	void Consolidate();
	bool CreateReport();
	int CreateGraph();

	CArray<CSalesHistoryCell,CSalesHistoryCell>* GetCellArray( int n );
	
private:
	const char* GetReportTitle( const char* szReportName );
	const char* GetColumnName( int nCol );
	void RebuildFilter();
	void RebuildTotals();
	void GetFileListPlu ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	void ConsolidateSale( int nLevel, int nCol, int nRow, double dQty, double dValue );
	void GetQtyValueInfo( int nLevel, int nCol, int nRow, double& dQty, double& dValue );
	void ProcessDate( int nCol, COleDateTime& date );
	void WriteColumnInfo();

private:
	double GetPercentage( double dValue, double dTotalValue );
	double GetAverage( CCSV& csv, int nValuePos, int nQtyPos );
	void AddToCSV ( CCSV& csv, int nPos, double dToAdd, int nDP );
	const char* GetDeptText( int nLevel );
	int GetMinLevelsToDisplay( int nPluIdx );

private:
	bool IsFiltered();

private:
	bool CreatePluReport();
	bool CreateDeptReport();
	int CreateDeptGraph();

private:
	CArray<CDateRangeOrRegionColumnInfo,CDateRangeOrRegionColumnInfo> m_arrayColumns;
	CSalesHistoryRegionLocColMap m_LocToColumnMap;
	
private:
	int m_nReportType;
	int m_nZeroSkipType;
	
private:
	CSessionDateTimeFilter m_DateTimeFilter;
	
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

private:
	CString m_strTitle;
	CString m_strColumnName;
	CString m_strPriceText;
	CString m_strDeptText;

private:
	CReportFile m_ReportFile;

	void* m_pArrayCells[ MAX_PLU_PRICES + MAX_SBPLU_PRICES ];

	CArray<CDateRangeInfo,CDateRangeInfo> m_arrayDateRange;
	CArray<CDateRangeOrRegionPluInfo,CDateRangeOrRegionPluInfo> m_arrayPluInfo;
	CDepartmentSorter m_DepartmentSorter;

	int m_nCols;
	int m_nRows;
	int m_nCells;
};

/**********************************************************************/
