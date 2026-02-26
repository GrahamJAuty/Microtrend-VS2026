#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

class CPluMonitorEcrman
{
public:
	CPluMonitorEcrman();
	~CPluMonitorEcrman();

	//ACCESS THE PLU DATABASE
	int GetSize() { return m_PluDatabase.GetSize(); }
	
	void GetAt( int nPluIdx, CPluCSVRecord& PluRecord ) { m_PluDatabase.GetAt( nPluIdx, PluRecord ); }
	const char* GetAt( int nPluIdx ) { return m_PluDatabase.GetAt ( nPluIdx ); }
	void SetAt( int nPluIdx, CPluCSVRecord& PluRecord ) { m_PluDatabase.SetAt( nPluIdx, PluRecord ); }
	int InsertRecord ( CPluCSVRecord& NewRecord, bool bExportFlag );
	void SetModifiedFlag( bool bFlag ) { m_PluDatabase.SetModifiedFlag( bFlag ); }
	void RemoveAt ( int nPluIdx );
	void RemoveAll();
	
	int ImportLine ( int nImportMethod, int& nPos, CSSImportFile& import, const char* szImportLine );
	int ImportLinePriceSet ( int& nPos, CSSImportFile& import, const char* szImportLine, CString& strOldRecord );
	void ImportEODPluLine( CEODPluImportInfo& ImportInfo ){ return m_PluDatabase.ImportEODPluLine( ImportInfo ); }
	
	__int64 GetPluNoInt ( int nPluIdx ) { return m_PluDatabase.GetPluNoInt( nPluIdx ); }
	const char* GetPluNoString ( int nPluIdx ) { return m_PluDatabase.GetPluNoString( nPluIdx ); }
	
	bool FindPluByNumber ( __int64 nPluNo, int& nIndex ){ return m_PluDatabase.FindPluByNumber( nPluNo, nIndex ); }
	bool IsModified(); 

	bool CopyDefaultPlu( int nDeptNo, CPluCSVRecord& Target ){ return m_PluDatabase.CopyDefaultPlu( nDeptNo, Target ); }

	//HELPER FUNCTIONS
	int SavePluRecord ( int nPluIdx, CPluCSVRecord& PluRecord, bool bResourceChangeInfo = FALSE, bool bResourceChangePlu = FALSE );
	void UpdatePrices();
	void UpdatePluPrice( CPluCSVRecord& Record, double dCost, bool bGetCost );
	double GetPluPurchaseCost( CPluCSVRecord& PluRecord ) { return PluRecord.GetPurchaseCost(); }
	void GetPluStockQty( __int64 nPluNo, double& dQty ) { dQty = 0.0; }
	void GetPluStockCode ( __int64 nPluNo, CString& strStockCode, bool bRecipe ) { strStockCode = ""; }
	void GetPluSuppRef ( __int64 nPluNo, CString& strSuppRef ) { strSuppRef = ""; }
	
	void PurgeForDownload( int nLocIdx, CPluCSVArray& PluArray, bool bChanges );

private:
	CPluCSVArray m_PluDatabase;
	
	friend class CDataManagerEcrman;
};

/**********************************************************************/
#endif
/**********************************************************************/
