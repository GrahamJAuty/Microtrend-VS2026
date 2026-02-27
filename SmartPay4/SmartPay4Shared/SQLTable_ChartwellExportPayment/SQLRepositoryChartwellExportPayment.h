#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowChartwellExportPayment.h"
//**********************************************************************

class CSQLRepositoryChartwellExportPayment
{
public:
	CSQLRepositoryChartwellExportPayment();

public:
	CSQLResultInfo SelectRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase);
	
public:
	CSQLResultInfo RowExists(__int64 nUserID, __int64 nTransactionID, int nTerminalNo, CString strDate, CDatabase* pDatabase);
};

//**********************************************************************
