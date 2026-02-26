#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ArrayIndexer.h"
#include "LocationSelectorEntity.h"
#include "..\CommonStockTray\SalesCostCSVArray.h"
#include "StockFilterArray.h"
#include "TransferDetailCSVArray.h"
/**********************************************************************/

class CStockLevelBranchNew
{
private:
	CStockLevelBranchNew();
	~CStockLevelBranchNew();
	
	void Clear();
	void DeleteTransferDetail();
	void DeleteSalesCost();

	bool FindNodeInt( int nLabelToFind, int& nNodeIdx );	
	int ConsolidateInt( int n, CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord* pTransferDetail, CSalesCostCSVRecord* pSalesCost, bool bEndNode, int nNextArrayIdx, __int64& nNodeCount );
	
private:
	CArray<int,int> m_arrayKeys;
	CArray<CStockLevelsCSVRecord,CStockLevelsCSVRecord> m_arrayStockLevels;
	CPtrArray m_arrayTransferDetail;
	CPtrArray m_arraySalesCost;
	CArray<int,int> m_arrayNextArrayIdx;
	
friend class CStockLevelTreeNew;
};

/**********************************************************************/

class CStockLevelTreeNew : public CPtrArray
{
public:
	CStockLevelTreeNew();
	~CStockLevelTreeNew();

public:
	void SetStockFilterArray( CStockFilterArray* p ) { m_pStockFilterArray = p; }
	void GetReportFields( CWordArray& array );
	void GetSuppIdxArray( CWordArray& array );
	void Consolidate();
	
public:
	void Configure( int nSortType, CStockFilterArray* pFilterArray, CLocationSelectorEntity& LocSelEntity, int nStocktakeNo, CWordArray& arraySortOrder, bool bShowItems, int nCatFilter, const char* szFrom, const char* szTo, bool bExcludeNonMovers, bool bItemFilter, CWordArray& arraySuppIdx, bool bAllSupp, bool bWantTransferDetail, bool bWantSalesCost );
	void Invalidate() { m_bInvalid = TRUE; }
	
public:
	bool GetSummaryFlag() { return m_bSummary; }
	int GetDbIdx() { return m_nDbIdx; }
	int GetConLevel() { return m_nConLevel; }
	int GetTableNo() { return m_nTableNo; }
	bool MatchStockpoint( int nSpIdx );

public:
	__int64 GetNodeCount() { return m_nNodeCount; }
	CStockLevelsCSVRecord GetTotalStockLevels() { return m_totalLevels; }

	int GetArraySize( int nArrayIdx );
	
	int GetLabelInt( int nArrayIdx, int nNodeIdx );
	CStockLevelsCSVRecord GetStockLevels( int nArrayIdx, int nNodeIdx );
	void GetTransferDetail( int nArrayIdx, int nNodeIdx, CTransferDetailCSVRecord& Detail );
	void GetSalesCost( int nArrayIdx, int nNodeIdx, CSalesCostCSVRecord& SalesCost );
	int GetNextArrayIdx( int nArrayIdx, int nNodeIdx );
	
private:
	void ConsolidateOneSupplierPerItem();
	void ConsolidateMultiSuppliersPerItem();

private:
	void Clear();
	void ConsolidateStockLevels( CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord* pTransferDetail, CSalesCostCSVRecord* pSalesCost );
	CStockLevelBranchNew* GetBranch( int nArrayIdx ) { return (CStockLevelBranchNew*) GetAt(nArrayIdx); }
	
private:
	__int64 m_nNodeCount;
	CArray<int,int> m_bufferInt;
	CWordArray m_arrayReportFields;
	CWordArray m_arrayReportFieldTypes;
	CStockLevelsCSVRecord m_totalLevels;

private:
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;
	int m_nStocktakeNo;
	
	bool m_bSummary;
	int m_nCategoryFilter;

	CWordArray m_arraySuppIdx;
	bool m_bAllSupp;

	bool m_bWantTransferDetail;
	bool m_bGotTransferDetail;

	bool m_bWantSalesCost;
	bool m_bGotSalesCost;
	
	CString m_strStockCodeFrom;
	CString m_strStockCodeTo;
	bool m_bExcludeNonMovers;
	bool m_bItemFilter;

	int m_nSortType;

	bool m_bInvalid;

private:
	CStockFilterArray* m_pStockFilterArray;
	CStockArrayIndexer m_StockArrayIndexer;

	friend class CStockTreeReportNew;
};

/**********************************************************************/
#endif
/**********************************************************************/
