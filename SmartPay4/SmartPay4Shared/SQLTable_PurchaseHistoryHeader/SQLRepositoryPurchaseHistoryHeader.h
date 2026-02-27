#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetPurchaseHistoryHeader.h"
//**********************************************************************

class CSQLRepositoryPurchaseHistoryHeader
{
public:
	CSQLRepositoryPurchaseHistoryHeader();

public:
	CSQLResultInfo SelectRow(CSQLRowPurchaseHistoryHeader& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPurchaseHistoryHeader& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPurchaseHistoryHeader& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPurchaseHistoryHeader& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRowAndGetID(CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nLineID, CDatabase* pDatabase);
};

//**********************************************************************

