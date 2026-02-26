/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
 
/**********************************************************************/
#define RECIPEDB_VERSION 4
/**********************************************************************/

namespace Recipe
{
const recInt	SpNo			= { "SpNo", 0, 999 };		
const recString	PluNo			= { "PluNo", 1, 16 };		
const recBool	PluStockLink	= { "PluStockLink" };
const recBool	ReverseSale		= { "ReverseSale" };
const recString	StockCode		= { "StockCode", 1, 16 };	
const recDouble	StockQty		= { "StockQty" };			
}

/**********************************************************************/

struct CRecipeCSVItem
{
public:
	CRecipeCSVItem() { m_strStockCode = ""; m_dStockQty = 0.0; }

public:
	CString m_strStockCode;
	double m_dStockQty;
};

/**********************************************************************/

class CRecipeCSVRecord : public CCSVRecord
{
public:
	CRecipeCSVRecord();
	virtual ~CRecipeCSVRecord();
	
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );
	void RemoveAll();

	static CString GetHeader();
	
	bool ImportLine ( CSSImportFile& import, CCSV& csv, bool& bIncompleteLine );
	CString StripSpaces( const char* szText );
	
	const char* GetRecipePluNo() { return m_strPluNo; }
	int GetSpNo() { return m_nSpNo; }
	bool GetReverseSaleFlag(){ return m_bReverseSale; }
	bool GetPluStockLinkFlag(){ return m_bPluStockLink; }
	const char* GetStockCode ( int nIndex ) { return m_Recipe[nIndex].m_strStockCode; }
	double GetStockQty ( int nIndex ) { return m_Recipe[nIndex].m_dStockQty; }	
	
	void SetRecipePluNo( const char* szPluNo );
	void SetSpNo( int n ){ m_nSpNo = n; }
	void SetReverseSaleFlag( bool b ){ m_bReverseSale = b; }
	void SetStockCode ( int nIndex, const char* szCode ) { m_Recipe[nIndex].m_strStockCode = szCode; }
	void SetStockQty ( int nIndex, double dQty ) { m_Recipe[nIndex].m_dStockQty = dQty; }

	//PLU STOCK LINK
	void EnablePluStockLink();
	void DisablePluStockLink();
	
	bool FindStockCode ( const char* szCode, int& nIndex );
	bool AddStockCode ( const char* szCode, double dQty );
	bool RemoveItem ( int nIndex );

	int GetSize() { return m_Recipe.GetSize(); }
	bool IsRecipe() { return ( GetSize() >= 2 ); }

	bool operator!= ( CRecipeCSVRecord& recipe );

private:
	void ClearRecord();		
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3to4ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	CString m_strPluNo;
	int m_nSpNo;
	bool m_bReverseSale;
	bool m_bPluStockLink;

private:
	CArray<CRecipeCSVItem,CRecipeCSVItem> m_Recipe;
};

/**********************************************************************/

class CRecipeCSVArray : public CSharedCSVArray
{
public:
	CRecipeCSVArray();
	~CRecipeCSVArray();
	
	const char* GetRecipePluNo ( int nIndex );
	bool FindPluByNumber ( int nMasterIdx, const char* szPluNumber, int& nIndex ); 
	
	bool IsInSync() { return m_bInSync; }
	void SetSyncFlag( bool bFlag ) { m_bInSync = bFlag; }

	int ImportLine ( CSSImportFile& import, const char* szImportLine, bool& bIncompleteLine );

private:
	bool m_bInSync;
	CString m_strPluNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
