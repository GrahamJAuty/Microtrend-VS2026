#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetBromComBalanceChanges.h"
//**********************************************************************

class CSQLRepositoryBromComBalanceChanges
{
public:
	CSQLRepositoryBromComBalanceChanges();

public:
	CSQLResultInfo SelectRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nPersonID, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);
};

//**********************************************************************

