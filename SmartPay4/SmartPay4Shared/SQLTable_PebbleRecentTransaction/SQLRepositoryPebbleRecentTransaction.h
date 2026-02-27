#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPebbleRecentTransaction.h"
//**********************************************************************

class CSQLRepositoryPebbleRecentTransaction
{
public:
	CSQLRepositoryPebbleRecentTransaction();

public:
	CSQLResultInfo SelectRow(CSQLRowPebbleRecentTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebbleRecentTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebbleRecentTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPebbleRecentTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebbleRecentTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strTransactionID, CDatabase* pDatabase);
};

//**********************************************************************
