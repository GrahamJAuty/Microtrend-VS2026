#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPebblePaymentType.h"
//**********************************************************************

class CSQLRepositoryPebblePaymentType
{
public:
	CSQLRepositoryPebblePaymentType();

public:
	CSQLResultInfo SelectRow(CSQLRowPebblePaymentType& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebblePaymentType& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebblePaymentType& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPebblePaymentType& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebblePaymentType& SQLRowPB, CDatabase* pDatabase);
	
	CSQLResultInfo RowExists(int nPaymentNo, CDatabase* pDatabase);
};

//**********************************************************************
