#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetPebbleLinkTime.h"
//**********************************************************************

class CSQLRepositoryPebbleLinkTime
{
public:
	CSQLRepositoryPebbleLinkTime();

public:
	CSQLResultInfo SelectRow(CSQLRowPebbleLinkTime& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebbleLinkTime& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebbleLinkTime& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebbleLinkTime& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nLineID, CDatabase* pDatabase);
};

//**********************************************************************

