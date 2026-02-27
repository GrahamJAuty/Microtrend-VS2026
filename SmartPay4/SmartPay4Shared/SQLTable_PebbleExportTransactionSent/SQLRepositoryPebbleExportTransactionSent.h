#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetPebbleExportTransactionSent.h"
//**********************************************************************

class CSQLRepositoryPebbleExportTransactionSent
{
public:
	CSQLRepositoryPebbleExportTransactionSent();

public:
	CSQLResultInfo SelectRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nLineID, CDatabase* pDatabase);
};

//**********************************************************************

