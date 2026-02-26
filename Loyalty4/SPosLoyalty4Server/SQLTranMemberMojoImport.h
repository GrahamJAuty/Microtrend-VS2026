#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranMemberMojoImport : public CSQLTranBase
{
public:
	CSQLTranMemberMojoImport();

public:
	bool SetAllAccountsInactive();
	__int64 GetUserIDForQRInfo(CString strQRInfo, __int64& nStartFromUserID);
	bool GetAccountRecord(__int64 nUserID, CSQLRowAccountFull& RowAccount, int& nAdded, int& nModified);
	bool UpdateAccountRecord(CSQLRowAccountFull& RowAccount);
	void SetCanCommitFlag() { m_bCanCommit = TRUE; }
	
private:
	int m_nCurrentRecordAppNo;
};

/**********************************************************************/
