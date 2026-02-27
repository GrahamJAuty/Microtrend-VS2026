#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPebbleExportPayment.h"
//**********************************************************************

class CSQLRepositoryPebbleExportPayment
{
public:
	CSQLRepositoryPebbleExportPayment();

public:
	CSQLResultInfo SelectRow(CSQLRowPebbleExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebbleExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebbleExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPebbleExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebbleExportPayment& SQLRowPB, CDatabase* pDatabase);
	
public:
	CSQLResultInfo RowExists(__int64 nUserID, CString strDate, CString strTime, int nCCNo, CDatabase* pDatabase);
};

//**********************************************************************
