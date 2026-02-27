#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetExternalAccount.h"
//**********************************************************************

class CSQLRepositoryExternalAccount
{
public:
	CSQLRepositoryExternalAccount();

public:
	CSQLResultInfo SelectRow(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo SelectRowByUserID(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo DeleteRowByUserID(CSQLRowExternalAccount& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strAESUserID, CDatabase* pDatabase);
};

//**********************************************************************
