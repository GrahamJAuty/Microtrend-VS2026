#pragma once
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowAuditPeriod.h"
//**********************************************************************

class CSQLRepositoryAuditPeriod
{
public:
	CSQLRepositoryAuditPeriod();

public:
	CSQLResultInfo SelectRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nPeriodID, CDatabase* pDatabase);
};

//**********************************************************************
