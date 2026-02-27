#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetBromComTransaction.h"
//**********************************************************************

class CSQLRepositoryBromComTransaction
{
public:
	CSQLRepositoryBromComTransaction();

public:
	CSQLResultInfo SelectRow(CSQLRowBromComTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowBromComTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowBromComTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowBromComTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nLineID, CDatabase* pDatabase);
};

//**********************************************************************

