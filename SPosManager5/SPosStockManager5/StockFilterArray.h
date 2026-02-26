#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySetCSVArray.h"
#include "ColumnManager.h"
#include "ReportConsolidationArray.h"
#include "..\CommonStockTray\StockCSVArray.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#define STOCKFILTER_SORT_FORWARD_STOCKCODE 0
#define STOCKFILTER_SORT_FORWARD_DESCRIPTION 1
#define STOCKFILTER_SORT_FORWARD_CATEGORY 2
#define STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE 3
/**********************************************************************/
#define STOCKFILTER_SORT_REVERSE_STOCKCODE 100
#define STOCKFILTER_SORT_REVERSE_DESCRIPTION 101
#define STOCKFILTER_SORT_REVERSE_CATEGORY 102
#define STOCKFILTER_SORT_REVERSE_NEWSTOCKCODE 103
/**********************************************************************/
#define STOCKFILTER_MINMAX_ALL 0
#define STOCKFILTER_MINMAX_BELOW_MIN 1
#define STOCKFILTER_MINMAX_ABOVE_MAX 2
/**********************************************************************/

struct CStockIndexItem
{
public:
	CStockIndexItem();
	void Reset();

public:
	int Compare ( CStockIndexItem& source, int nHint = 0 );
	void Add	( CStockIndexItem& source );
	
public:
	CString m_strDescription;
	int m_nCategory;
	CString m_strStockCode;
	CString m_strNewStockCode;
	
public:
	bool m_bDeletionFlag;
	int m_nIndex;
	int m_nIndexVersion;
};

/**********************************************************************/

struct CStockFilterMinMaxFlags
{
public:
	CStockFilterMinMaxFlags( bool bAllowCheck );
	void ResetResults( bool bTrueIfNoCheck );

public:
	bool CheckMin(){ return m_bCheckMin; }
	bool CheckMax(){ return m_bCheckMax; }
	
public:
	bool BelowMin(){ return m_bBelowMin; }
	bool AboveMax(){ return m_bAboveMax; }

public:
	void SetCheckMin( bool b ){ m_bCheckMin = b; }
	void SetCheckMax( bool b ){ m_bCheckMax = b; }
	void SetBelowMin( bool b ){ m_bBelowMin = b; }
	void SetAboveMax( bool b ){ m_bAboveMax = b; }

private:
	bool m_bCheckMin;
	bool m_bCheckMax;
	bool m_bBelowMin;
	bool m_bAboveMax;
};

/**********************************************************************/

class CStockFilterArray 
{
public:
	CStockFilterArray( int nFilterType ); 

private:
	void Reset();

public:
	//CRITERIA FOR SELECTING RECORDS INTO SUBSET
	void SetSortType( int nType ){ m_nSortType = nType; }
	void SetFilterType ( int nType ) { m_nFilterType = nType; }
	void SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase );
	void SetCategoryFilter ( int nCategoryFilter = 0 );
	void SetSupplierFilter ( int nSupplierFilter = 0 );
	void SetStocktakeFilter (int nStocktakeFilter = -1 ) { m_nStocktakeFilter = nStocktakeFilter; }
	void SetActiveItemOnlyFlag( bool b ){ m_bActiveItemOnly = b; }
	void SetActiveStockpointsArray( CArray<int,int>& arrayStockpoints );
	void SetNoPluLinkFlag( bool b ) { m_bNoPluLink = b; }
	void SetSinglePluLinkFlag( bool b ) { m_bSinglePluLink = b; }
	void SetMultiPluLinkFlag( bool b ) { m_bMultiPluLink = b; }
	void SetMinMaxFilter( int n = 0 );
	void SetCheckApparentFlag( bool b ){ m_bCheckApparent = b; }

	//CHECK CURRENT STATUS
	int GetSortType(){ return m_nSortType; }
	int GetFilterType(){ return m_nFilterType; }
	const char* GetDescription() { return m_strDescription; }
	int GetSearchType() { return m_nSearchType; }
	bool GetMatchCase() { return m_bMatchCase; }
	int GetCategoryFilter() { return m_nCategoryFilter; }
	int GetSupplierFilter() { return m_nSupplierFilter; }
	int GetStocktakeFilter() { return m_nStocktakeFilter; }
	bool GetActiveItemOnlyFlag(){ return m_bActiveItemOnly; }
	bool GetNoPluLinkFlag() { return m_bNoPluLink; }
	bool GetSinglePluLinkFlag() { return m_bSinglePluLink; }
	bool GetMultiPluLinkFlag() { return m_bMultiPluLink; }
	int GetMinMaxFilter(){ return m_nMinMaxFilter; }
	bool GetCheckApparentFlag() { return m_bCheckApparent; }
	bool GetFilteredFlag(){ return m_bFiltered; }

	//FIRST AVAILABLE CATEGORY FOR NEW RECORDS
	int GetDefaultCategory();
	
	//STOCKPOINTS AND DATABASES
	void SetSpIdx( int nSpIdx, int nSpIdx2 );
	void SetSpIdx( CStockFilterArray& source );
	int GetDbNo() { return m_nDbNo; }
	int GetSpIdx() { return m_nSpIdx; }
	int GetSpIdx2() { return m_nSpIdx2; }
	int GetSpNo() { return m_nSpNo; }
	int GetSpNo2() { return m_nSpNo2; }

	int GetStocktakeExcludeCount() { return m_nStocktakeExclude; }
	int GetInactiveExcludeCount() { return m_nInactiveExclude; }

public:
	//BUILD NORMAL LIST
	void BuildRecordList();
	void ChangeSortOrder( int nType, bool bRefreshIndexItems );
	void QuickSort();

private:
	//BUILD NORMAL LIST
	void CreateUnfilteredList();
	void RefreshIndexItems();
	bool CheckDescription( CStockCSVRecord& StockRecord );
	bool CheckSupplier( CStockCSVRecord& StockRecord );

public:
	//BUILD SPECIAL LIST FOR DELETION
	void QuickBuild( bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales );
	void QuickBuild( CByteArray& FlagArray, bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales );
	
private:
	//BUILD SPECIAL LIST FOR DELETION
	void QuickBuildInternal( bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales );
	void QuickBuildInternal( CByteArray& FlagArray, bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales );

public:	
	//CHECK WHETHER LIST IS FILTERED
	bool CheckFiltered();
	bool IsNativeOrder();
	bool HasAllRecords();

public:
	//ARRAY SIZE AND NAVIGATION
	int GetSize(); 
	void SetArrayIdx( int nArrayIdx ); 	
	int GetArrayIdx(){ return m_nArrayIdx; }	
	void Next();
	void Previous();
	bool HasNext();
	bool HasPrevious();

public:
	//TRANSLATE BETWEEN FILTER ARRAY AND STOCK ARRAY
	int GetStockItemIndexAt( int nArrayIdx );
	int GetStockItemIndex() { return GetStockItemIndexAt ( m_nArrayIdx ); }
	bool FindStockItemByCode ( const char* szCode, int& nArrayIdx );
	void GetStockItemRange( CString& strFirstStockCode, CString& strLastStock );
	void GetDefaultRange ( int nArrayIdx, CString& strStockFrom, CString& strStockTo );
	void GetArrayIdxRange( const char* szFrom, const char* szTo, int& nStartIdx, int& nRange );

public:
	//STOCK ARRAY MANAGEMENT
	int InsertRecord( CStockCSVRecord& StockRecord );
	int DeleteStockItemRange ( const char* szFrom, const char* szTo );
	void RemoveIndex( int nArrayIdx );

private:
	bool CheckStockOnOrder( int nStockIdx );
	
public:
	//GET VARIOUS LINES FOR DISPLAY
	const char* GetDisplayLine ( int nArrayIdx );
	void GetLine( int nStockIdx, int nArrayIdx );
	const char* GetColumnManagerDisplayLine ( CColumnManager& ColumnManager, int nArrayIdx );
	void GetColumnManagerLine( CColumnManager& ColumnManager, int nArrayIdx );

public:
	bool GetNegativeStockFlag() { return m_bNegativeStock; }
	bool GetBelowMinStockFlag() { return m_MinMaxFlags.BelowMin(); }
	bool GetAboveMaxStockFlag() { return m_MinMaxFlags.AboveMax(); }
	
public:
	//SELECT ITEMS FOR DELETION
	bool GetDeletionFlagAt( int nArrayIdx );
	void SetDeletionFlagAt( int nArrayIdx, bool bFlag );
			
public:
	//HELPER FUNCTIONS FOR CHANGING STOCK CODES
	void CreateNewStockCodeArray();
	const char* GetNewStockCode ( int nStockIdx ); 
	void SetNewStockCode ( int nStockIdx, const char* szCode );

	//HELPER FUNCTIONS FOR ORDER PREVIEW
	void SetOrderPreviewConLevel ( int nLevel ) { m_nOrderPreviewConLevel = nLevel; }
	void SetOrderPreviewTableNo ( int nTableNo ) { m_nOrderPreviewTableNo = nTableNo; }
	
private:
	int m_nSortType;
	int m_nFilterType;
	CString m_strDescription;
	int m_nSearchType;
	bool m_bMatchCase;
	int m_nCategoryFilter;
	int m_nSupplierFilter;
	int m_nStocktakeFilter;
	bool m_bActiveItemOnly;
	CArray<int,int> m_arrayActiveStockpoints;
	bool m_bNoPluLink;
	bool m_bSinglePluLink;
	bool m_bMultiPluLink;
	int m_nMinMaxFilter;
	bool m_bCheckApparent;
	
private:
	int m_nSpIdx;
	int m_nSpIdx2;
	int m_nSpNo;
	int m_nSpNo2;
	int m_nDbNo;
	
private:
	bool m_bFiltered;
	int m_nStocktakeExclude;
	int m_nInactiveExclude;
	
private:
	CString m_strRecord;
	CString m_strStockCode;
	CString m_strNewStockCode;
	
private:
	int m_nOrderPreviewConLevel;
	int m_nOrderPreviewTableNo;
	
private:
	CByteArray m_SupplierNoArray;
	CStringArray m_arrayNewStockCodes;
	CCategorySetCSVRecord m_CategorySet;
	bool m_bNegativeStock;
	CStockFilterMinMaxFlags m_MinMaxFlags;
	
private:
	int m_nIndexVersion;
	CReportConsolidationArray<CStockIndexItem> m_index;
	int m_nArrayIdx;
};

/**********************************************************************/
#endif	
/**********************************************************************/
