#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetBromComItem.h"
//**********************************************************************

class CSQLRepositoryBromComItem
{
public:
	CSQLRepositoryBromComItem();

public:
	CSQLResultInfo SelectRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nOrderID, int nPersonID, CDatabase* pDatabase);
};

//**********************************************************************

