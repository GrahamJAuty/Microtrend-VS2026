#pragma once
//**********************************************************************
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
//**********************************************************************
#include "SQLTable_PurchaseControlRule/SQLRowPurchaseControlRule.h"
//**********************************************************************

class CSQLPurchaseControlCache
{
public:
	void Reset()
	{
		m_arrayStatus.RemoveAll();
		m_arrayRules.RemoveAll();
	}

	bool GetPurchaseControlRow(int nRuleID, CSQLRowPurchaseControlRule& Row);
	void InsertRow(CSQLRowPurchaseControlRule& Row);

private:
	CReportConsolidationArray<CSortedIntByInt> m_arrayStatus;
	CArray<CSQLRowPurchaseControlRule> m_arrayRules;
};

//**********************************************************************