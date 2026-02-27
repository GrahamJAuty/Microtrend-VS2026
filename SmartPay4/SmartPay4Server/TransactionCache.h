#pragma once
//*********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
//*********************************************************************

class CTransactionCache
{
public:
	bool CheckCache(CString strTransaction);
	void AddToCache(CString strTransaction);
	void PurgeCache();
	void WriteCacheToFile();

private:
	CReportConsolidationArray<CSortedStringByString> m_arrayTransactions;
};

//*********************************************************************
extern CTransactionCache TransactionCache;
//*********************************************************************
