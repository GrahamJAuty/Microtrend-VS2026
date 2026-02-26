#pragma once
//*********************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//*********************************************************************

class CEmptyLogFileCache
{
public:
	bool CheckCache(CSortedIntAndStringItem& item);
	void AddToCache(CSortedIntAndStringItem& item);
	void RemoveFromCache(CSortedIntAndStringItem& item);
	void WriteCacheToFile();

private:
	CReportConsolidationArray<CSortedIntAndStringItem> m_arrayTransactions;
};

//*********************************************************************

