#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CSuppRefFinderItem
{
public:
	CSuppRefFinderItem();

public:
	void Reset();
	int Compare ( CSuppRefFinderItem& source, int nHint = 0 );
	void Add	( CSuppRefFinderItem& source );
	
public:
	CString m_strKey;
	CString m_strStockCode;
	int m_nStockSuppIdx;
};

/*************************************************************************/

class CSuppRefFinder  
{
public:
	CSuppRefFinder();
	void Rebuild( int nSuppNo );
	void Invalidate(){ m_bIsInSync = FALSE; }

public:
	bool FindItemBySuppRef( CString& strSuppRef, int& nStockIdx, int& nStockSuppIdx );
	bool FindItemByWrapper( CString& strWrapper, int& nStockIdx, int& nStockSuppIdx );

public:
	void HandlePreferredSupplierChange( int nStockIdx );

private:
	bool m_bIsInSync;
	int m_nSuppNo;

private:
	CReportConsolidationArray<CSuppRefFinderItem> m_arraySuppRef;
	CReportConsolidationArray<CSuppRefFinderItem> m_arrayWrapper;
};

/**********************************************************************/
#endif
/**********************************************************************/
