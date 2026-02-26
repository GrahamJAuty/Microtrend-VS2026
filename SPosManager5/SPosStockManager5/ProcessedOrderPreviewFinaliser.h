#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/

class CProcessedOrderPreviewFinaliser 
{
public:
	CProcessedOrderPreviewFinaliser( CWnd* pParentWnd, CLocationSelectorMiniLookup& LocSelMiniLookup, const char* szMonthFrom, const char* szMonthTo );
	
public:
	void CreateDatabaseOrders( CProcessedOrderPreviewCSVArray& OrderArray );
	void CreateStockpointOrders( CProcessedOrderPreviewCSVArray& OrderArray );
	
private:
	void RemoveTemporarySalesFiles( bool bCopy );
	
private:
	CLocationSelectorMiniLookup& m_LocSelMiniLookup;

	CString m_strMonthFrom;
	CString m_strMonthTo;

private:
	CWnd* m_pParentWnd;
};

/**********************************************************************/
#endif
/**********************************************************************/

