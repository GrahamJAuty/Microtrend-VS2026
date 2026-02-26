#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "EposSelectArray.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
#include "SystemMap.h"
#include "TableReportHelpers.h"
/**********************************************************************/

class CLocationSelector  
{
public:
	CLocationSelector();
	void SetDatabaseMode( int nMode );
	void SetStockTransferFlag(){ m_bStockTransfer = TRUE; }
	void ExcludeAllTerminals() { m_bExcludeAllTerminals = TRUE; }
	void ExcludeNoComms() { m_bExcludeNoComms = TRUE; }
	void ExcludeWebSales() { m_bExcludeWebSales = TRUE; }
	void SetJournalReportMode() { m_bJournalReportMode = TRUE; }
	void SetExcludeDbIdx ( int nIdx ) { m_nExcludeDbIdx = nIdx; }
	void SetExcludeSpIdx( int nIdx ) { m_nExcludeSpIdx = nIdx; }
	void SetSingleLocIdx( int nLocIdx ) { m_nSingleLocIdx = nLocIdx; }
	void SetSingleSpIdx( int nSpIdx ) { m_nSingleSpIdx = nSpIdx; }
	void SetSingleDbIdx( int nDbIdx ) { m_nSingleDbIdx = nDbIdx; }
	void SetLocDbNoFilter(int nDbNo) { m_nLocDbNoFilter = nDbNo; }
	void SetForceSystemFlag( bool bFlag ) { m_bForceSystem = bFlag; }
	void SetForceGroupByDbFlag( bool bFlag ) { m_bForceGroupByDb = bFlag; }

	void BuildList( CWordArray& arrayTypes );
	void FillLocationCombo ( CSSComboBoxEx* pLocCombo, CSSTabbedComboBox* pEposCombo = NULL, CSSTabbedComboBox* pTableCombo = NULL, CTableReportHelpers* pTableReportHelpers = NULL );
	void FillEposCombo();
	void FillTableCombo();
	
public:
	void ForceSelection ( int nConType, int nIdx );
	void ForceSelection ( CLocationSelectorEntity& LocSelEntity );
	void SelectTableComboFilter( CTableFilter& Filter );
	
	int GetListSize() { return m_arrayListItems.GetSize(); }
	void GetListEntry ( int nPos, CLocationSelectorEntity& LocSelEntity );

	void GetSelectedEntity( CLocationSelectorEntity& LocSelEntity );
	
private:
	void AddToLocCombo ( const char* szText, int nImage );
	void AddDatabases();
	void AddDatabase( int nDbIdx );
	void AddNetworks();
	void AddLocations();
	void AddLocation( int nLocIdx );
	void AddReportpoints();
	void AddLocationSets();
	void AddSystem();
	bool IncludeDbIdx ( int nDbIdx );

#ifdef STOCKMAN_SYSTEM
	void AddSites();
	void AddStockpoints();
#endif

private:
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayListItems;

	CSSComboBoxEx* m_pLocCombo;
	CSSTabbedComboBox* m_pEposCombo;
	CSSTabbedComboBox* m_pTableCombo;
	CTableReportHelpers* m_pTableReportHelpers;
	
	int m_nPreviousSelectionTerminal;
	int m_nPreviousSelectionTable;
	int m_nDatabaseMode;

	bool m_bExcludeNoCommsLoc;
	bool m_bExcludeAllTerminals;
	bool m_bExcludeWebSales;
	bool m_bSelectAllTerminals;
	bool m_bJournalReportMode;
	bool m_bExcludeNoComms;
	bool m_bForceSystem;
	bool m_bForceGroupByDb;
	bool m_bStockTransfer;
	int m_nSingleLocIdx;
	int m_nSingleSpIdx;
	int m_nSingleDbIdx;
	int m_nExcludeDbIdx;
	int m_nExcludeSpIdx;
	int m_nLocDbNoFilter;

	CImageList m_ImageList;
};

/**********************************************************************/
#endif
/**********************************************************************/


