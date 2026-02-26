#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowDeptInfo.h"
//**********************************************************************

class CSQLRepositoryDeptInfo
{
public:
	CSQLRepositoryDeptInfo();

public:
	CSQLResultInfo SelectRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase);
	
public:
	void ImportDeptInfo(int nDbNo, CString strFilename);
};

//**********************************************************************

