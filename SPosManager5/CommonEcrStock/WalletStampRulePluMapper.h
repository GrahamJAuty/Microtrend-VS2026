#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CWalletStampRulePluMapper
{
public:
	void BuildMap();
	int GetRuleIDByPlu(__int64 nPluNo);

private:
	CReportConsolidationArray<CSortedIntByInt64> m_PluMap;
	CReportConsolidationArray<CSortedIntItem> m_arrayUsedOfferGroups;
};

/**********************************************************************/
