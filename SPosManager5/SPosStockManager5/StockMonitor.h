#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "LocationSelectorEntity.h"
#include "..\CommonStockTray\StockCSVArray.h"
#include "StockFilterArray.h"
#include "..\CommonStockTray\StockLevelsCSVArray.h"
#include "TransferDetailCSVArray.h"
/**********************************************************************/
#define STOCKLEVEL_IMPORT_ACCEPT 0
#define STOCKLEVEL_IMPORT_REJECT_STOCKTAKE 1
#define STOCKLEVEL_IMPORT_REJECT_INACTIVE 2
#define STOCKLEVEL_IMPORT_REJECT_INACTIVE_SOURCE 3
#define STOCKLEVEL_IMPORT_REJECT_INACTIVE_TARGET 4
/**********************************************************************/

struct CStockMonitorInfo
{
	CStockLevelsCSVArray* m_pStockLevels;
	CTransferDetailCSVArray* m_pTransferDetail;
	CArray<double,double>* m_pSalesLevels;
	int m_nDataManagerStatusStockLevels;
	int m_nDataManagerStatusTransferDetail;
};

/**********************************************************************/

class CStockMonitor
{
public:
	CStockMonitor();
	~CStockMonitor();

	//ACCESS THE STOCK DATABASE
	int GetSize() { return m_StockDatabase.GetSize(); }
	
	void GetAt( int nStockIdx, CStockCSVRecord& StockRecord ) { m_StockDatabase.GetAt( nStockIdx, StockRecord ); }
	const char* GetAt( int nStockIdx ) { return m_StockDatabase.GetAt ( nStockIdx ); }
	void SetAt( int nStockIdx, CStockCSVRecord& StockRecord ) { m_StockDatabase.SetAt( nStockIdx, StockRecord ); }
	bool InsertRecord ( CStockCSVRecord& NewRecord, int& nStockIdx );
	void RemoveAt ( int nStockIdx );
	
	int ImportLine ( int nImportMethod, CSSImportFile& import, const char* szImportLine, int nSupplierCount );
	int PurgeSupplier ( int nSuppNo );
	
	const char* GetStockCode ( int nStockIdx ) { return m_StockDatabase.GetStockCode( nStockIdx ); }
	const char* GetDescription ( int nStockIdx ) { return m_StockDatabase.GetDescription( nStockIdx ); }
	int GetCategory( int nStockIdx ) { return m_StockDatabase.GetCategory( nStockIdx ); }
	
	bool FindStockItemByCode ( const char* szCode, int& nIndex ) { return m_StockDatabase.FindStockItemByCode ( szCode, nIndex ); }
	bool IsModified(); 

	//ACCESS STOCK LEVELS
	CStockLevelsCSVArray* GetStockLevels( int nIndex ) { return m_arrayStockMonitorInfo[nIndex].m_pStockLevels; }
	CTransferDetailCSVArray* GetTransferDetail( int nIndex ) { return m_arrayStockMonitorInfo[nIndex].m_pTransferDetail; }
	CArray<double,double>* GetSalesLevels( int nIndex ) { return m_arrayStockMonitorInfo[nIndex].m_pSalesLevels; }
	
	void GetTransferTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CTransferDetailCSVRecord& TransferTotals );
	void SetTransferTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CTransferDetailCSVRecord& TransferTotals );
	void RemoveTransferTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo );
	
	void GetStockpointTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CStockLevelsCSVRecord& StockLevels );
	void GetStockpointTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CStockLevelsCSVRecord& StockLevels, CStockFilterMinMaxFlags& MinMaxFlags );
	void SetStockpointTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CStockLevelsCSVRecord& StockLevels );
	void GetSiteTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSiteNo, CStockLevelsCSVRecord& StockLevels );
	void GetSiteTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSiteNo, CStockLevelsCSVRecord& StockLevels, CStockFilterMinMaxFlags& MinMaxFlags );
	void GetDatabaseTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels );
	void GetDatabaseTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, CStockFilterMinMaxFlags& MinMaxFlags );
	void ClearOrders( int nMasterIdx );

	//QUICK ACCESS FOR STOCK SALES HISTORY REPORT
	void SetQuickModeFlag( bool bFlag ) { m_bQuickMode = bFlag; }
	
	//GENERATES A STOCK DATABASE FOR THE SALES BY DATE REPORT
	void GenerateFromAuditedSales ( CLocationSelectorEntity& LocSelEntity, const char* szFrom, const char* szTo, bool bIncludeReorderedItems );
	
	//HANDLE STOCK CODE CHANGES
	bool ChangeStockCodes( CStockFilterArray* pFilterArray );

	//HELPER FUNCTION FOR REORDERING
	void GetReorderInfo ( int nDbIdx, int nStockIdx, CLocationSelectorMiniLookup& LocSelMiniLookup, CStringArray& arrayReorderInfo, bool bFullApparent, bool bAllowOrder = TRUE, bool bRequireOrder = TRUE );
	
	//SYNCHRONISE STOCK LEVELS WITH STOCK DATABASE
	bool SynchroniseStockLevels( int nIndex, int nProgressSoFar, int nProgressTarget, bool bProgress = TRUE );

	//SYNCHRONISE TRANSFER DETAILS WITH STOCK DATABASE
	bool SynchroniseTransferDetail( int nIndex, int nProgressSoFar, int nProgressTarget, bool bProgress = TRUE );

	//UPDATE DETAIL TRANSFER INFORMATION FOR WAREHOUSE / CELLAR SYSTEMS
	void UpdateTransferDetail( int nSpIdxSource, int nSpIdxDest, int nStockIdx, CStockCSVRecord& StockRecord, double dQty );

	//CHECK STOCK LEVELS FOR IMPORT EXCEPTION
	int CheckStockLevelsForImport( int nStockIdx, CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, int nSpNo, int nStocktakeNo = -1 );
	int CheckStockLevelsForTransfer( int nStockIdx, CStockCSVRecord& StockRecord, int nSpNo1, int nSpNo2 );

	//STOCK ITEM STATUS FUNCTIONS THAT RELY ON SYSTEM OPTIONS ETC
	const char* GetStockLevelString( CStockCSVRecord& StockRecord, double dLevel, int nDps = -1 );
	const char* GetStockLevelStringForPluEdit( CStockCSVRecord& StockRecord, double dLevel, int nDps = -1 );
	const char* GetStockLevelStringForPluTab( CStockCSVRecord& StockRecord, double dLevel, int nDps = -1 );
	const char* GetStockSubUnitName( CStockCSVRecord& StockRecord , bool bNoBlank = FALSE );
	const char* GetStockSubUnitNameForPluEdit( CStockCSVRecord& StockRecord );
	int GetSubUnits( CStockCSVRecord& StockRecord );
	double CalculateRetailValue ( CStockCSVRecord& StockRecord, double dStockQty );
	double CalculateTaxContentFromValue( CStockCSVRecord& StockRecord, double dValue );
	double CalculateHistoricalTaxContentFromValue( CStockCSVRecord& StockRecord, double dValue, int nTaxDateCode );
	double GetUnitWastageCost( CStockCSVRecord& StockRecord, int nIndex );
	double CalculateTaxOnCost( CStockCSVRecord& StockRecord, double dValue );

private:
	void GenerateFromAuditedSalesInternal ( int nSpIdx, const char* szFrom, const char* szTo, bool bIncludeReordedItems );
	void DeleteStockLevels();
	void CreateStockLevels( int nStartSpIdx, int nEndSpIdx );

private:
	CStockCSVArray m_StockDatabase;
	CArray<CStockMonitorInfo,CStockMonitorInfo> m_arrayStockMonitorInfo;

private:
	CString m_strBASuppRef;
	CString m_strStockLevel;
	CString m_strStockSubUnitName;
	bool m_bQuickMode;

	friend class CDataManagerStockman;
};

/**********************************************************************/
#endif
/**********************************************************************/
