#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\Smartpay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\Smartpay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranBatchUpdate : public CSQLTranBase
{
public:
	CSQLTranBatchUpdate();
	void DoWork(int nUpdateType, CArray<CSQLRowAccountList, CSQLRowAccountList>& arraySourceItems, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CString strComment, CString strLeaversYear);
	void AfterTransaction();

public:
	int GetCount() { return m_nCount; }

public:
	bool DeleteRow(CSQLRowAccountFull& RowAccount);
	bool UpdateRedeemPoints(CSQLRowAccountFull& RowAccount);

private:
	CArray<CSQLRowAccountFull,CSQLRowAccountFull> m_arrayDeletedAccounts;
	CString m_strLeaversYear;
	CString m_strComment;
	int m_nCount;
};

/**********************************************************************/
