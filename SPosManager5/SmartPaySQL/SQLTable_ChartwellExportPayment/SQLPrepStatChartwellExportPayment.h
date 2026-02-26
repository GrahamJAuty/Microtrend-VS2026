#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowChartwellExportPayment.h"
//**********************************************************************

class CSQLPrepStatChartwellExportPayment
{
public:
	CSQLPrepStatChartwellExportPayment();

public:
	CSQLResultInfo SelectRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase);
};

//**********************************************************************
