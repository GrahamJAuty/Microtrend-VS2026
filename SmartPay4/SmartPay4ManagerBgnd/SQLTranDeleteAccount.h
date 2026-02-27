#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranDeleteAccount : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CString strComment, CString strLeaversYear);
	void AfterTransaction();

public:
	bool DeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment, CString strLeaversYear, CDatabase* pDatabase, CSQLAuditRecord_base& atc);
	void NonSQLDeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment, CString strLeaversYear);

private:
	bool DoDelete(CSQLRowAccountFull& RowAccount, CSQLAuditRecord_base& atc);
	void NonSQLDelete(CSQLRowAccountFull& RowAccount);

private:
	CSQLRowAccountFull m_RowDeletedAccount;
	CString m_strComment;
	CString m_strLeaversYear;
};

/**********************************************************************/
