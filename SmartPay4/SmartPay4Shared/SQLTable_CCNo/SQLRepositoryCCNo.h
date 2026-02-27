#pragma once
//**********************************************************************
#include "..\\SQLResultInfo.h"
#include "..\\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetCCNo.h"
//**********************************************************************

class CSQLRepositoryCCNo
{
public:
	CSQLRepositoryCCNo();

public:
	CSQLResultInfo SelectRow(CSQLRowCCNo& SQLRowCC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowCCNo& SQLRowCC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowCCNo& SQLRowCC, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowCCNo& SQLRowCC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowCCNo& SQLRowCC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strType, CDatabase* pDatabase);
	bool ImportLegacyCCNo(CDatabase* pDatabase);
	int GetNextCCNoForPC(CDatabase* pDatabase);
};

//**********************************************************************


