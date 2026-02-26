#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CClearHistoryStockSales  
{
public:
	CClearHistoryStockSales( int nYear, int nMonth, int nDay, CLocationSelectorEntity& LocSelEntity );

private:
	void ClearStockSales ( int nSpIdx, const char* szDate );
	void FilterStockSales( CStringArray& FileArray, int nMDay );
};

/**********************************************************************/
#endif
/**********************************************************************/


