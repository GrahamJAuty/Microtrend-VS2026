#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetPebbleExportTransaction.h"
//**********************************************************************

class CSQLRepositoryPebbleExportTransaction
{
public:
	CSQLRepositoryPebbleExportTransaction();

public:
	CSQLResultInfo SelectRow(CSQLRowPebbleExportTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebbleExportTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebbleExportTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebbleExportTransaction& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nLineID, CDatabase* pDatabase);
};

//**********************************************************************

