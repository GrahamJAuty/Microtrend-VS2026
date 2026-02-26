#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonStockTray\RecipeCSVArray.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "DepartmentSetCSVArray.h"
#include "KeyboardFilterPluArray.h"
#include "PluCSVArray.h"
#include "PresetPluFilter.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define PLUFILTER_SORT_FORWARD_PLUNO 0
#define PLUFILTER_SORT_FORWARD_TEXT 1
#define PLUFILTER_SORT_FORWARD_DEPT 2
#define PLUFILTER_SORT_FORWARD_STOCKCODE 3
#define PLUFILTER_SORT_FORWARD_EPOSTEXT 4
/**********************************************************************/
#define PLUFILTER_SORT_REVERSE_PLUNO 100
#define PLUFILTER_SORT_REVERSE_TEXT 101
#define PLUFILTER_SORT_REVERSE_DEPT 102
#define PLUFILTER_SORT_REVERSE_STOCKCODE 103
#define PLUFILTER_SORT_REVERSE_EPOSTEXT 104
/**********************************************************************/

struct CPluIndexItem
{
public:
	CPluIndexItem();
	void Reset();

public:
	int Compare ( CPluIndexItem& source, int nHint = 0 );
	void Add	( CPluIndexItem& source );
	
public:
	CString m_strText;
	CString m_strEPOSText;
	int m_nDeptNo;
	__int64 m_nPluNo;
	CString m_strStockCode;
	
public:
	bool m_bDeletionFlag;
	int m_nIndex;
	int m_nIndexVersion;
};

/**********************************************************************/

struct CPluFilterExtraEcrman
{
public:
	CPluFilterExtraEcrman();

public:
	int m_nSupplierFilter;
	bool m_bShowNonStockItems;
	bool m_bNegativeStock;
	CString m_strSuppRefFilter;
};

/**********************************************************************/

struct CPluFilterExtraStockman
{
public:
	CPluFilterExtraStockman();

public:
	CString m_strStockCode;
	bool m_bShowUnlinkedItems;
	bool m_bShowLinkedItems;
	bool m_bShowRecipeItems;
};

/**********************************************************************/

class CPluFilterArray
{
public:
	CPluFilterArray(int nFilterType);

private:
	void Reset();

public:
	//CRITERIA FOR SELECTING RECORDS INTO SUBSET
	void SetSortType(int nType) { m_nSortType = nType; }
	void SetDepartmentFilter(int nDepartmentFilter = 0);
	void SetAnalysisCategoryFilter(int nCategoryFilter = 9999) { m_nAnalysisCategoryFilter = nCategoryFilter; }
	void SetDescriptionFilter(const char* szDescription, int nSearchType, bool bMatchCase);
	void SetLocalPluLocationFilter(bool bFilter) { m_bLocalPluLocation = bFilter; }
	void SetLocalPluMasterFilter(bool bFilter) { m_bLocalPluMaster = bFilter; }
	void SetLocalPluExcludeFilter(bool bFilter) { m_bLocalPluExclude = bFilter; }
	void SetEANOnlyFilter(bool bFilter) { m_bEANOnly = bFilter; }
	void SetLiteralOnlyFilter(bool bFilter) { m_bLiteralOnly = bFilter; }
	void SetKeyboardFilter(CKeyboardFilterSelectorLoc& Loc, int nKbNo);
	void SetShowActiveItemsFlag(bool b) { m_bShowActiveItems = b; }
	void SetShowInactiveItemsFlag(bool b) { m_bShowInactiveItems = b; }
	void SetPluRangeFlag(bool b) { m_bPluRange = b; }
	void SetPluRangeStart(__int64 n);
	void SetPluRangeEnd(__int64 n);

	//CHECK CURRENT STATUS
	int GetSortType() { return m_nSortType; }
	int GetFilterType() { return m_nFilterType; }
	int GetDepartmentFilter() { return m_nDepartmentFilter; }
	int GetAnalysisCategoryFilter() { return m_nAnalysisCategoryFilter; }
	const char* GetDescription() { return m_strDescription; }
	int GetSearchType() { return m_nSearchType; }
	bool GetMatchCase() { return m_bMatchCase; }
	bool GetLocalPluLocationFilter() { return m_bLocalPluLocation; }
	bool GetLocalPluMasterFilter() { return m_bLocalPluMaster; }
	bool GetLocalPluExcludeFilter() { return m_bLocalPluExclude; }
	void GetKeyboardFilter(CKeyboardFilterSelectorLoc& Loc, int& nKbNo);
	bool GetShowActiveItemsFlag() { return m_bShowActiveItems; }
	bool GetShowInactiveItemsFlag() { return m_bShowInactiveItems; }
	bool GetPluRangeFlag() { return m_bPluRange; }
	__int64 GetPluRangeStart() { return m_nPluRangeStart; }
	__int64 GetPluRangeEnd() { return m_nPluRangeEnd; }
	bool GetFilteredFlag() { return m_bFiltered; }

public:
	//BUILD NORMAL LIST
	void BuildRecordList();
	void ChangeSortOrder(int nType, bool bRefreshFields);
	void QuickSort();

private:
	bool IsExcludedByStockDetail(int nPluIdx, CPluCSVRecord& PluRecord);
	int GetListIdxForNewPluIdx(CPluCSVRecord& PluRecord, int nPluIdx);

private:
	//BUILD NORMAL LIST
	void CreateUnfilteredList();
	void RefreshIndexItems();
	bool CheckDescription(CPluCSVRecord& PluRecord);
	const char* GetStockCodeForSorting(CPluCSVRecord& PluRecord);

public:
	//BUILD SPECIAL LIST FOR DELETION
	void QuickBuild(CByteArray& arrayFlag, bool bOnlyIfActive);

private:
	//BUILD SPECIAL LIST FOR DELETION
	void QuickBuildInternal(CByteArray& arrayFlag, bool bOnlyIfActive);

public:
	void GetLine(int nArrayIdx, int nFilterIdx);

private:
	void GetLine_Init();
	void GetLine_GetStockCode(CPluCSVRecord& PluRecord, CString& strStockCode);
	void GetLine_AppendStockCode(CCSV& csv, CString strStockCode);
	void GetLine_AppendEcrmanStock(CCSV& csv, CPluCSVRecord& PluRecord);
	void GetLine_AppendStockmanSupplier(CCSV& csv, CPluCSVRecord& PluRecord);
	bool GetLine_BrowseStockCodes();

public:
	//GET DISPLAY LINE BASED ON TYPE OF LIST 
	const char* GetDisplayLine(int nArrayIdx);
	const char* GetColumnManagerDisplayLine(CColumnManager& ColumnManager, int nArrayIdx);
	void GetColumnManagerLine(CColumnManager& ColumnManager, int nArrayIdx);

public:
	//CHECK WHETHER LIST IS FILTERED
	bool CheckFiltered();
	bool IsNativeOrder();

private:
	bool CheckFilteredStock();

public:
	void CopyToPreset(CPresetPluFilter& Preset);
	void CopyFromPreset(CPresetPluFilter& Preset);

public:
	//ARRAY SIZE AND NAVIGATION
	int GetSize();
	void SetArrayIdx(int nArrayIdx);
	int GetArrayIdx() { return m_nArrayIdx; }
	void Next();
	void Previous();
	bool HasNext();
	bool HasPrevious();

public:
	//TRANSLATE BETWEEN FILTER ARRAY AND PLU ARRAY
	int GetPluItemIndexAt(int nArrayIdx);
	int GetPluItemIndex() { return GetPluItemIndexAt(m_nArrayIdx); }
	bool FindPluItemByNumber(__int64 nPluNo, int& nArrayIdx);
	void GetPluItemRange(__int64& nFirstPluNo, __int64& nLastPluNo);
	void GetDefaultRange(int nArrayIdx, __int64& nPluFrom, __int64& nPluTo, bool bNoEan = FALSE);
	void GetArrayIdxRange(__int64 nPluFrom, __int64 nPluTo, int& nStartIdx, int& nRange);

public:
	//PLU ARRAY MANAGEMENT
	void DeletePluItemRange(__int64 nFirstPluNo, __int64 nLastPluNo);
	void RemoveIndex(int nIndex);

public:
	//SELECT ITEMS FOR DELETION
	bool GetDeletionFlagAt(int nArrayIdx);
	void SetDeletionFlagAt(int nArrayIdx, bool bFlag);

public:
	const char* GetEcrmanSuppRefFilter() { return m_EcrmanFilters.m_strSuppRefFilter; }
	int GetEcrmanSupplierFilter() { return m_EcrmanFilters.m_nSupplierFilter; }
	bool GetEcrmanShowNonStockFlag() { return m_EcrmanFilters.m_bShowNonStockItems; }
	bool GetEcrmanNegativeStockFlag() { return m_EcrmanFilters.m_bNegativeStock; }

public:
	void SetEcrmanSuppRefFilter(const char* sz) { m_EcrmanFilters.m_strSuppRefFilter = sz; }
	void SetEcrmanSupplierFilter(int n) { m_EcrmanFilters.m_nSupplierFilter = n; }
	void SetEcrmanShowNonStockFlag(bool b) { m_EcrmanFilters.m_bShowNonStockItems = b; }
	void SetEcrmanNegativeStockFlag(bool b) { m_EcrmanFilters.m_bNegativeStock = b; }

public:
	void SetStockmanStockCodeFilter(const char* szCode) { m_StockmanFilters.m_strStockCode = szCode; }
	void SetStockmanShowUnlinkedPluFlag(bool b) { m_StockmanFilters.m_bShowUnlinkedItems = b; }
	void SetStockmanShowLinkedPluFlag(bool b) { m_StockmanFilters.m_bShowLinkedItems = b; }
	void SetStockmanShowRecipePluFlag(bool b) { m_StockmanFilters.m_bShowRecipeItems = b; }

public:
	const char* GetStockmanStockCodeFilter() { return m_StockmanFilters.m_strStockCode; }
	bool GetStockmanShowUnlinkedPluFlag() { return m_StockmanFilters.m_bShowUnlinkedItems; }
	bool GetStockmanShowLinkedPluFlag() { return m_StockmanFilters.m_bShowLinkedItems; }
	bool GetStockmanShowRecipePluFlag() { return m_StockmanFilters.m_bShowRecipeItems; }

public:
	int InsertPluRecord(CPluCSVRecord& NewRecord, bool bExportFlag);

private:
	int InsertPluRecordInternal(CPluCSVRecord& NewRecord, bool bExportFlag);

#ifdef STOCKMAN_SYSTEM

public:
	int InsertPluRecord(CPluCSVRecord& PluRecord, CRecipeCSVRecord& RecipeRecord, bool bExportFlag);
	int InsertPluRecord(CPluCSVRecord& PluRecord, const char* szStockCode, bool bExportFlag);

public:
	void QuickBuildUnlinked(bool bOnlyIfActive);

private:
	void QuickBuildUnlinkedInternal(bool bOnlyIfActive);

#endif

protected:
	//DESCRIPTION FILTER
	CString m_strDescription;
	int m_nSearchType;
	bool m_bMatchCase;

	//DEPARTMENT FILTER
	CDepartmentSetCSVRecord m_DepartmentSet;
	int m_nDepartmentFilter;

	//DEPARTMENT FILTER
	int m_nAnalysisCategoryFilter;

	//LOCAL PLU FILTER
	bool m_bLocalPluLocation;
	bool m_bLocalPluMaster;
	bool m_bLocalPluExclude;
	bool m_bEANOnly;
	bool m_bLiteralOnly;

	//ACTIVE ITEM FILTER
	bool m_bShowActiveItems;
	bool m_bShowInactiveItems;

	//PLU RANGE
	bool m_bPluRange;
	__int64 m_nPluRangeStart;
	__int64 m_nPluRangeEnd;

	//KEYBOARD FILTER
	CKeyboardFilterPluArray m_KeyboardFilterPluArray;

protected:
	int m_nFilterType;
	int m_nSortType;
	bool m_bFiltered;
	int m_nIndexVersion;

protected:
	CReportConsolidationArray<CPluIndexItem> m_index;
	int m_nArrayIdx;

protected:
	CString m_strStockCodeForSorting;
	CString m_strDisplayLine;

protected:
	CPluFilterExtraEcrman m_EcrmanFilters;
	CPluFilterExtraStockman m_StockmanFilters;

#ifdef STOCKMAN_SYSTEM

	CRecipeCSVRecord* m_pRecipeRecord;

#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
