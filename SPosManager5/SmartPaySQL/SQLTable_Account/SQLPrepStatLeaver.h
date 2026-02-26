#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
#include "SQLRowAccountFull.h"
#include "SQLRecordSetAccountFull.h"
//**********************************************************************

class CSQLPrepStatLeaver
{
public:
	CSQLPrepStatLeaver( CString strTableName );

public:
	CSQLResultInfo SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase );
	CSQLResultInfo SelectRowFind(CSQLRowAccountFind& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo InsertDirect(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nUserID, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);
	
public:
	void ClearUserIDCache();
	void BuildUserIDCache();
	bool CheckUserIDCache(__int64 nUserID);
	
private:
	CString m_strTableName;
	CString m_strPrimaryKeyFilter;

private:
	CReportConsolidationArray<CSortedInt64Item> m_UserIDCache;
};

//**********************************************************************

