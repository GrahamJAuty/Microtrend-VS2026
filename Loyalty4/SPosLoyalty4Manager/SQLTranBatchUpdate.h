#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranBatchUpdate : public CSQLTranBase
{
public:
	CSQLTranBatchUpdate();

public:
	void DoWork(int nUpdateType, CArray<CSQLRowAccountList,CSQLRowAccountList>& arraySourceItems, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CString strComment);
	void AfterTransaction();

public:
	int GetCount() { return m_nCount; }

public:
	bool DeleteRow(CSQLRowAccountFull& RowAccount);
	bool UpdateRedeemPoints(CSQLRowAccountFull& RowAccount);

private:
	CArray<CSQLRowAccountFull,CSQLRowAccountFull> m_arrayDeletedAccounts;
	CString m_strComment;
	int m_nCount;
};

/**********************************************************************/
