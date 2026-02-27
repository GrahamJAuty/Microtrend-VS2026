#pragma once
/**********************************************************************/
#include "SQLAuditRecord.h"
#include "SmartPayAuditFile.h"
#include "SQLTable_ClosingBalance\SQLRowClosingBalance.h"
/**********************************************************************/

class CClosingBalanceBuilder
{
public:
	CClosingBalanceBuilder();

public:
	bool CalculateBalanceForDate(COleDateTime& timeBalance, CSmartPayAuditFile* pAudit);
	
private:
	int CheckAuditLineDate(CSQLAuditRecord* atcRecord);
	void MergeAccountBalanceFromDatabase(const char* szUserID, CSQLRowClosingBalance& RowBalance);
	void MergeAccountBalanceFromAudit(bool bCalcBalance, CSQLRowClosingBalance& RowBalance);

private:
	bool SaveCalculatedBalanceForDate(CSQLRowClosingBalance& RowBalance);

private:
	CSQLAuditRecord m_atc;
	COleDateTime m_oleTimeBalance;
	CSSDate m_dateBalance;

private:
	CSQLRowClosingBalance m_ClosingBalanceCurrent;
};

/**********************************************************************/
