#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetStaffGiftException.h"
//**********************************************************************

class CSQLRepositoryStaffGiftException
{
public:
	CSQLRepositoryStaffGiftException();

public:
	CSQLResultInfo SelectRow(CSQLRowStaffGiftException& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowStaffGiftException& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowStaffGiftException& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowStaffGiftException& SQLRowSC, CDatabase* pDatabase);
};

//**********************************************************************


