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
	CSQLResultInfo SelectRow(CSQLRowAuditPeriod& SQLRowAP, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowAuditPeriod& SQLRowAP, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowAuditPeriod& SQLRowAP, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowAuditPeriod& SQLRowAP, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nPeriodID, CDatabase* pDatabase);
	bool ImportLegacyAuditPeriod(CDatabase* pDatabase);
};

//**********************************************************************

