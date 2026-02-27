#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetBromComPendingOrder.h"
//**********************************************************************

class CSQLRepositoryBromComPendingOrder
{
public:
	CSQLRepositoryBromComPendingOrder();

public:
	CSQLResultInfo SelectRow(CSQLRowBromComPendingOrder& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowBromComPendingOrder& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowBromComPendingOrder& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowBromComPendingOrder& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowBromComPendingOrder& SQLRowBC, CDatabase* pDatabase);
	
	CSQLResultInfo RowExists(int nOrderID, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);
	CSQLResultInfo CountRowsByRange(int nFrom, int nTo, CDatabase* pDatabase);
};

//**********************************************************************
