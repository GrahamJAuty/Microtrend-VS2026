#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetGroupFull.h"
#include "SQLRowGroupFull.h"
//**********************************************************************

class CSQLPrepStatGroup
{
public:
	CSQLPrepStatGroup();

public:
	CSQLResultInfo SelectRow(CSQLRowGroupFull& SQLRowGroup, CDatabase* pDatabase);
};

//**********************************************************************

