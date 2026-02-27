#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetBromComPendingItem.h"
//**********************************************************************

class CSQLRepositoryBromComPendingItem
{
public:
	CSQLRepositoryBromComPendingItem();

public:
	CSQLResultInfo SelectRow(CSQLRowBromComPendingItem& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowBromComPendingItem& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowBromComPendingItem& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowBromComPendingItem& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeletePendingItemOrder(int nOrderID, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nOrderID, int nPersonID, CDatabase* pDatabase);
};

//**********************************************************************

