#pragma once
/**********************************************************************/
#include "EposSelectArray.h"
/**********************************************************************/

class CAutoExpireHistory  
{
public:
	CAutoExpireHistory();
	void Reset();
	void ExpireReportsNow();

public:
	void Read();

private:
	void Write();

private:
	void CheckEposReports( CEposSelectArray& SelectArray );

#ifdef STOCKMAN_SYSTEM
	void CheckStockSalesHistory();
	void CheckStockAudits();
	void CheckStockRetryFiles();
#else
	void CheckPluDeliveryAudit();
#endif

private:
	void LogDeletion ( const char* szMsg, int nYear, int nMonth );
	void GetDeletionYearAndMonth( int nMonthsToKeep, int& nYear, int& nMonth );

private:
	COleDateTime m_dateMonthLastCheck;
	COleDateTime m_dateMonthStart;
	CString m_strMonthLastCheck;
};

/**********************************************************************/
extern CAutoExpireHistory AutoExpireHistory;
/**********************************************************************/

