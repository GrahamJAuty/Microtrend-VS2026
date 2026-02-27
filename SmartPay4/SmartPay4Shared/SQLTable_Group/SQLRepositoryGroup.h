#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetGroupFull.h"
#include "SQLRowGroupFull.h"
//**********************************************************************

class CSQLRepositoryGroup
{
public:
	CSQLRepositoryGroup();

public:
	CSQLResultInfo SelectRow(CSQLRowGroupFull& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowGroupFull& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowGroupFull& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowGroupFull& SQLRowGroup, CDatabase* pDatabase);
	
	CSQLResultInfo RowExists(int nGroupNo, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);
	CSQLResultInfo CountRowsByRange(int nFrom, int nTo, CDatabase* pDatabase);
};

//**********************************************************************
