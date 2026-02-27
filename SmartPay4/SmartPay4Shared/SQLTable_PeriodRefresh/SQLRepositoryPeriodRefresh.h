#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetPeriodRefresh.h"
//**********************************************************************

class CSQLRepositoryPeriodRefresh
{
public:
	CSQLRepositoryPeriodRefresh();

public:
	CSQLResultInfo SelectRow(CSQLRowPeriodRefresh& SQLRowPeriod, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPeriodRefresh& SQLRowPeriod, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPeriodRefresh& SQLRowPeriod, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPeriodRefresh& SQLRowPeriod, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPeriodRefresh& SQLRowPeriod, CDatabase* pDatabase);	
	CSQLResultInfo RowExists(__int64 nUserID, CDatabase* pDatabase);
};

//**********************************************************************
