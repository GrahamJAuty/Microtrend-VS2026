#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranDeleteAccount : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount,CString strComment);
	void AfterTransaction();

public:
	bool DeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment, CDatabase* pDatabase, CStringArray& arrayAuditBuffer);
	void NonSQLDeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment);

private:
	bool DoDelete(CSQLRowAccountFull& RowAccount, CStringArray& arrayAuditBuffer, bool bFastDelete = FALSE);
	void NonSQLDelete(CSQLRowAccountFull& RowAccount);

private:
	CSQLRowAccountFull m_RowDeletedAccount;
	CString m_strComment;
};

/**********************************************************************/
