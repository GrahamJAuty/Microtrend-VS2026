#pragma once
/**********************************************************************/
#include "EposSelectArray.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CClearHistoryEposSales  
{
public:
	CClearHistoryEposSales( COleDateTime& dateTo, CLocationSelectorEntity& LocSelEntity, CEposSelectArray& SelectArray, bool bDeletePlu, bool bDeleteSummary, bool bDeleteDetail, bool bIncludeToDate );
	CClearHistoryEposSales( COleDateTime& dateFrom, COleDateTime& dateTo, CLocationSelectorEntity& LocSelEntity, CEposSelectArray& SelectArray, bool bDeletePlu, bool bDeleteSummary, bool bDeleteDetail, bool bIncludeToDate );
	
private:
	void ClearEposSales ( COleDateTime& dateFrom, COleDateTime& dateTo, CLocationSelectorEntity& LocSelEntity, CEposSelectArray& SelectArray, bool bDeletePlu, bool bDeleteSummary, bool bDeleteDetail, bool bIncludeToDate );
	void ClearEposSales( CEposSelectArray& SelectArray, CString& strDateFrom, CString& strDateTo, bool bIncludeToDate );
	
#ifdef STOCKMAN_SYSTEM
	void ClearDatabaseSales( int nDbIdx, CString& strDateFrom, CString& strDateTo );
#endif

private:
	bool m_bDeletePlu;
	bool m_bDeleteSummary;
	bool m_bDeleteDetail;
};

/**********************************************************************/
