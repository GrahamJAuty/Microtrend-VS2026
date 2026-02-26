#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CStockPluRecord : public CCSVRecord
{
public:
	CStockPluRecord(); //fill with default values
	void SetStockCode( const char* szCode ) { m_strStockCode = szCode; }
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv );
	
	const char* GetStockCode() { return m_strStockCode; }

	int GetPluNoCount() { return m_nPluNoArray.GetSize(); }
	__int64 GetPluNo ( int nListPos );
	bool AddPluNo ( __int64 nPluNo );
	bool RemovePluNo ( __int64 szPluNo );

	int GetRecipePluNoCount() { return m_nRecipePluNoArray.GetSize(); }
	__int64 GetRecipePluNo ( int nListPos );
	bool AddRecipePluNo ( __int64 nPluNo );
	bool RemoveRecipePluNo ( __int64 nPluNo );

private:
	CString m_strStockCode;
	CArray<__int64,__int64> m_nPluNoArray;
	CArray<__int64,__int64> m_nRecipePluNoArray;
};

/**********************************************************************/

class CStockPluArray : public CCSVArray
{
public:
	CStockPluArray();
	bool FindStockCode ( const char* szStockCode, int& nIndex );
	const char* GetStockCode ( int nIndex );
	
	int GetPluNoCount ( int nIndex );
	__int64 GetPluNo ( int nIndex, int nListPos );
	bool AddLink ( const char* szStockCode, __int64 nPluNo );
	bool RemoveLink ( const char* szStockCode, __int64 nPluNo );
	
	int GetRecipePluNoCount ( int nIndex );
	__int64 GetRecipePluNo ( int nIndex, int nListPos );
	bool AddRecipeLink ( const char* szStockCode, __int64 nPluNo );
	bool RemoveRecipeLink ( const char* szStockCode, __int64 nPluNo );
	
	int GetTotalLinkCount ( int nIndex );

	void RemoveAll();
	void Rebuild();

	void Invalidate() { m_bIsValid = FALSE; }
	bool IsValid() { return m_bIsValid; }

	void HideProgress() { m_bHideProgress = TRUE; }

public:
	void MergePluNoArray( const char* szStockCode, CReportConsolidationArray<CSortedInt64Item>& PluArray );

private:
	void RebuildInternal();

private:
	CString m_strStockCode;
	bool m_bIsValid;
	bool m_bHideProgress;
};

/**********************************************************************/
#endif				//__STOCKPLUARRAY_H__	
/**********************************************************************/
