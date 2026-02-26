#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "Consolidation.h"
#include "PluCSVArray.h"
#include "..\CommonStockTray\RecipeCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPluMonitorStockman
{
public:
	CPluMonitorStockman();
	~CPluMonitorStockman();

	void Reset();

	//ACCESS THE PLU DATABASE
	int GetSize() { return m_PluDatabase.GetSize(); }
	
	void GetAt( int nPluIdx, CPluCSVRecord& PluRecord ) { m_PluDatabase.GetAt( nPluIdx, PluRecord ); }
	const char* GetAt( int nPluIdx ) { return m_PluDatabase.GetAt ( nPluIdx ); }
	void SetAt( int nPluIdx, CPluCSVRecord& PluRecord ) { m_PluDatabase.SetAt( nPluIdx, PluRecord ); }
	
	int InsertRecord ( CPluCSVRecord& PluRecord, const char* szStockCode, bool bExportFlag );
	int InsertRecord ( CPluCSVRecord& PluRecord, CRecipeCSVRecord& RecipeRecord, bool bExportFlag );

	void SetModifiedFlag( bool bFlag ) { m_PluDatabase.SetModifiedFlag( bFlag ); }
	void RemoveAt ( int nPluIdx );
	void RemoveAll();
	
	int ImportLine ( int nImportMethod, int& nPos, CSSImportFile& import, const char* szImportLine, bool bCreateStock, bool bWithStockCode = FALSE );
	int ImportLinePriceSet ( int& nPos, CSSImportFile& import, const char* szImportLine, CString& strOldRecord );
	void ImportEODPluLine( CEODPluImportInfo& ImportInfo ){ return m_PluDatabase.ImportEODPluLine( ImportInfo ); }
	
	__int64 GetPluNoInt ( int nPluIdx ) { return m_PluDatabase.GetPluNoInt( nPluIdx ); }
	const char* GetPluNoString ( int nPluIdx ) { return m_PluDatabase.GetPluNoString( nPluIdx ); }
	
	bool FindPluByNumber ( __int64 nPluNo, int& nIndex ) { return m_PluDatabase.FindPluByNumber ( nPluNo, nIndex ); }
	bool IsModified(); 

	bool CopyDefaultPlu( int nDeptNo, CPluCSVRecord& Target ){ return m_PluDatabase.CopyDefaultPlu( nDeptNo, Target ); }

	//ACCESS RECIPES
	void GetRecipe( int nMasterIdx, __int64 nPluNo, CRecipeCSVRecord& Recipe );
	void SetRecipe( int nMasterIdx, CRecipeCSVRecord& Recipe );
	int ImportRecipeLine ( CSSImportFile& import, const char* szImportLine, bool& bIncompleteLine );
	
	//SYNCHRONISE RECIPES WITH PLU ITEMS
	bool Synchronise( bool bOnlyIfFlagged );

	//HELPER FUNCTIONS
	void RemoveDeadStockLinks();
	bool IsRecipe( __int64 nPluNo );
	double GetRecipeCost( CRecipeCSVRecord& RecipeRecord );
	void GetRecipeCSVLine ( int nPluIdx, __int64 nPluNo, CString& strLine );
	int SavePluRecord ( int nPluIdx, CPluCSVRecord& PluRecord, bool bResourceChangeInfo = FALSE, bool bResourceChangePlu = FALSE );
	void UpdatePrices();
	void UpdatePrices( CReportConsolidationArray<CSortedInt64Item>& arrayPluNo );
	void UpdatePluPrice( CPluCSVRecord& Record, double dCost, bool bGetCost );
	double GetPluPurchaseCost( CPluCSVRecord& PluRecord );
	void GetPluStockQty( __int64 nPluNo, double& dQty );
	void GetPluStockCode ( __int64 nPluNo, CString& strStockCode, bool bRecipe );
	void GetPluSuppRef ( __int64 nPluNo, CString& strSuppRef );
	void GetPluSupplier ( __int64 nPluNo, CString& strSupplier );
	void UpdateSalesManCostPrice();

	void PurgeForDownload( int nLocIdx, CPluCSVArray& PluArray, bool bChanges );

private:
	CPluCSVArray m_PluDatabase;
	CRecipeCSVArray m_RecipeDatabase;

	friend class CDataManagerStockman;
};

/**********************************************************************/
#endif
/**********************************************************************/
