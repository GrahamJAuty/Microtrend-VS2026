#pragma once
/**********************************************************************/
#include "CustomerSelectionListAccount.h"
#include "CustomerCSVArray.h"
#include "DepartmentSetCSVArray.h"
#include "DepartmentSorter.h"
#include "CustomerSalesTree.h"
#include "SalesHistoryCSVArrayCustomer.h"
#include "SalesHistoryDateInfo.h"
#include "SalesHistoryReportPlu.h"
#include "SessionCSVarray.h"
#include "TermFileListInfo.h"
#include "TimeMap.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/
#define FUNC_TYPE_NORMAL 0
#define FUNC_TYPE_PERCENT 1
#define FUNC_TYPE_UNKNOWN 2
#define FUNC_TYPE_NOTOTAL 3
/**********************************************************************/
#define SALESHISTCUST_LINE_PLU 0
#define SALESHISTCUST_LINE_CHARGE 1
#define SALESHISTCUST_LINE_RECEIVE 2
#define SALESHISTCUST_LINE_TAX 3
#define SALESHISTCUST_LINE_PAYMENT 4
/**********************************************************************/

struct CAccountHistoryLineInfo
{
public:
	CAccountHistoryLineInfo();

public:
	int m_nLineType;
	int m_nSubType;
	double m_dValue;
};

/**********************************************************************/

struct CCustomerHeaderInfo
{
public:
	CCustomerHeaderInfo();

public:
	CString m_strAccount;
	CString m_strAddress;
	CString m_strTotalName;
	double m_dTotalQty;
	double m_dTotalVal;
	bool m_bDPQty;
};

/**********************************************************************/

class CSalesHistoryReportCustomer 
{
public:
	CSalesHistoryReportCustomer();
	void Reset( CSalesHistoryCSVRecordCustomer& ReportRecord, CLocationSelectorEntity& LocSelEntity );
	
private:
	void Reset ( CLocationSelectorEntity& LocSelEntity );

public:
	void SetDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice );
	
	void SetPluRangeFlag ( bool bFlag ) { m_bItemRange = bFlag; }
	void SetPluFrom ( __int64 nPluNo ) { m_nItemFrom = nPluNo; }
	void SetPluTo (  __int64 nPluNo ) { m_nItemTo = nPluNo; }
	void SetDepartmentByItemFlag( bool b ){ m_bDepartmentByItem = b; }
	void SetTaxFilter( int nFilter ) { m_nTaxFilter = nFilter; }
	void SetPriceLevelFilter( int nFilter ) { m_nPriceLevelFilter = nFilter; }

	void SetShowSalesFlag( bool b ){ m_bShowSales = b; }
	void SetShowRAFlag( bool b ){ m_bShowRA = b; }
	void SetShowChargesFlag( bool b ){ m_bShowCharges = b; }
	void SetShowPayMethFlag( bool b ){ m_bShowPayMeth = b; }
	void SetShowTaxFlag( bool b ){ m_bShowTax = b; }
	void SetNewPagePerCustomerFlag( bool b ){ m_bNewPagePerCustomer = b; }
	
	void SetSessionTitle( const char* szTitle ) { m_strSessionTitle = szTitle; }
	void WriteTerminalList();

public:
	bool CreateReport();
	bool PrepareFields();
	void Consolidate();

private:
	void ConsolidatePluSales();
	void ConsolidateCustomerActivity();
	bool ConsolidateCustomerActivityBuffer( int nCustomerID, CString& strServer, CTermFileListInfo& infoFile, CSalesHistoryDateInfo& infoDate, CString& strTime, CArray<CAccountHistoryLineInfo,CAccountHistoryLineInfo>& arrayBuffer );
	
private:
	void GetFileListPluSales ( CArray<CTermFileListInfo,CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	void GetFileListCustomer ( CArray<CTermFileListInfo,CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	int CheckConsolidationDateAndTime( CSalesHistoryDateInfo& infoDate, CString& strTime );
	
private:
	bool CreateCustomReportInternal();
	
private:
	void ProcessCustomerSalesArray(int nArray, int nDepth, CEposReportConsolCustomerHistory& infoConsol );
	void CreateReportLine( CEposReportConsolCustomerHistory& infoNode);
	
private:
	const char* GetHeaderLine( int nDepth );
	void WriteHeader( int nDepth );
	void WriteTotals( int nDepth, CEposReportConsolCustomerHistory& infoConsol );

private:
	const char* GetReportTitle( const char* szReportName );
	const char* GetRepMonth( COleDateTime date, bool bShortName, bool bFullLine );
	const char* GetLabelTimeString();
	
private:
	void WriteReportLine( const char* szLine );
	void WriteBlankLines( int nLineCount );
	void WriteTransactionItems( int nStart, int nEnd, bool bGrandTotal );
	void WriteCustomerHeader( CCustomerSelectionListAccountInfo& infoCustomer, CEposReportConsolCustomerHistory& infoNode, bool bOneOff );
	void AddTotalsToCustomerHeader( CArray<CCustomerHeaderInfo,CCustomerHeaderInfo>& arrayHeaderInfo, int& nPos, const char* szTotal, double dQty, double dVal, bool bDPQty );
	void FinishTaxSection( int nTaxLines, double dSalesTotal, double dTaxTotal );

public:
	CReportFilters m_ReportFilters;

private:
	//TIME AND DATE FILTERS
	CTimeSliceMapCSVRecord m_TimeSliceMap;

	//SORT BY TIME SLOT
	CTimeMap m_TimeMap;
	
	//PLU AND DEPARTMENT FILTERS
	bool m_bItemRange;
	__int64 m_nItemFrom;
	__int64 m_nItemTo;
	bool m_bDepartmentByItem;
	int m_nTaxFilter;
	int m_nPriceLevelFilter;

	//CUSTOMER ACTIVITY FILTERS
	bool m_bShowSales;
	bool m_bShowRA;
	bool m_bShowCharges;
	bool m_bShowPayMeth;
	bool m_bShowTax;
	
	//LOCATION AND SALES TO INCLUDE	
	CEposSelectArray m_SelectArray;
	
	//STOCKMAN NEEDS THIS FOR SELECTING STOCKPOINTS
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;

	//REPORT TYPE AND MISC SETTINGS
	int m_nReportType;
	CString m_strReportName;
	
	//REPORT FIELDS
	CWordArray m_arrayReportFields;
	CByteArray m_arrayTotalFlags;

	//CONSOLIDATED SALES
	CCustomerSalesTable m_CustomerSalesTable;

	//DEPT SORTER FOR THE UNSOLD ITEMS REPORT
	CDepartmentSorter m_DepartmentSorter;

	//THE ACTUAL REPORT FILE
	CReportFile m_ReportFile;

	//INTELLIGENCE FOR AVOIDING MULTIPLE BLANK LINES
	int m_nBlankLineCount;

	//VARIOUS TITLES
	CString m_strSessionTitle;
	CString m_strTitle;
	CString m_strRepMonth;
	CString m_strHeader;	

	//PROGRESS MONITORING
	int m_nProgress;

	//LABEL DECODING
	CString m_strLabelMonth;
	int m_nLabelWeekNo;
	CString m_strLabelWeek;
	CString m_strLabelDate;
	CString m_strLabelTimeString;
	int m_nLabelTimeHour;
	int m_nLabelTimeMinute;
	int m_nLabelTimeSecond;
	int m_nLabelTimeStars;
	CString m_strLabelWeekDay;
	CString m_strLabelDBase;
	int m_nLabelDbIdx;
	int m_nLabelLocIdx;
	CString m_strLabelLoc;
	CString m_strLabelEcr;
	int m_nLabelTerminal;
	int m_nLabelTaxBand;
	int m_nLabelTranServerNo;
	CString m_strLabelTranServerName;
	CString m_strLabelCustomerName;

	CString m_strWeekDays[7];
	
	//CUSTOMER SELECTION
	bool m_bLastFieldIsCustomer;
	bool m_bNewPagePerCustomer;
	bool m_bFirstCustomer;
};

/**********************************************************************/
