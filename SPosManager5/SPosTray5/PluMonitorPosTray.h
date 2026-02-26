#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "PluCSVArray.h"
#include "..\CommonStockTray\RecipeCSVArray.h"
/**********************************************************************/

class CPluMonitorPosTray
{
public:
	CPluMonitorPosTray();
	~CPluMonitorPosTray();

	void Reset();

	//ACCESS THE PLU DATABASE
	int GetSize() { return m_PluDatabase.GetSize(); }
	
	void GetAt( int nPluIdx, CPluCSVRecord& PluRecord ) { m_PluDatabase.GetAt( nPluIdx, PluRecord ); }
	const char* GetAt( int nPluIdx ) { return m_PluDatabase.GetAt ( nPluIdx ); }
	void SetAt( int nPluIdx, CPluCSVRecord& PluRecord ) { m_PluDatabase.SetAt( nPluIdx, PluRecord ); }
	
	__int64 GetPluNoInt ( int nPluIdx ) { return m_PluDatabase.GetPluNoInt( nPluIdx ); }
	const char* GetPluNoString ( int nPluIdx ) { return m_PluDatabase.GetPluNoString( nPluIdx ); }
	
	bool FindPluByNumber ( __int64 nPluNo, int& nIndex ) { return m_PluDatabase.FindPluByNumber ( nPluNo, nIndex ); }
	
	//ACCESS RECIPES
	void GetRecipe( int nMasterIdx, __int64 nPluNo, CRecipeCSVRecord& Recipe );
	
	//HELPER FUNCTIONS
	double GetRecipeCost( CRecipeCSVRecord& RecipeRecord );
	double GetPluPurchaseCost( CPluCSVRecord& PluRecord );
	void GetPluStockCode( __int64 nPluNo, CString& strStockCode, bool bRecipe );
	void GetPluSuppRef( __int64 nPluNo, CString& strSuppRef );
	
private:
	CPluCSVArray m_PluDatabase;
	CRecipeCSVArray m_RecipeDatabase;

	friend class CDataManagerPosTray;
};

/**********************************************************************/
#endif
/**********************************************************************/
