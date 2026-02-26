#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetStaffCard.h"
//**********************************************************************

class CSQLRepositoryStaffCard
{
public:
	CSQLRepositoryStaffCard();

public:
	CSQLResultInfo SelectRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase);
};

//**********************************************************************


