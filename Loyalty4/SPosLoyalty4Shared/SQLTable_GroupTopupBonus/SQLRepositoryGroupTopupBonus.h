#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetGroupTopupBonus.h"
#include "SQLRowGroupTopupBonus.h"
//**********************************************************************

class CSQLRepositoryGroupTopupBonus
{
public:
	CSQLRepositoryGroupTopupBonus();

public:
	CSQLResultInfo SelectRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase);

	CSQLResultInfo RowExists(int nGroupNo, int nPurseNo, int nTopupNo, CDatabase* pDatabase);
};

//**********************************************************************

