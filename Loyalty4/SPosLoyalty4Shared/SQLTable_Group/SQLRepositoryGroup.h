#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetGroup.h"
#include "SQLRowGroup.h"
//**********************************************************************

class CSQLRepositoryGroup
{
public:
	CSQLRepositoryGroup();

public:
	CSQLResultInfo SelectRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nGroupNo, CDatabase* pDatabase);
};

//**********************************************************************

