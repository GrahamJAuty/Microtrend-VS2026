#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPluInfo.h"
//**********************************************************************

class CSQLRepositoryPluInfo
{
public:
	CSQLRepositoryPluInfo();

public:
	CSQLResultInfo SelectRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nPluNo, CDatabase* pDatabase);

public:
	void ImportPluInfo(CString strFilename);
	void GetPluCountsByPCtrlRuleID(CReportConsolidationArray<CSortedIntByInt>& arrayPluCounts);
	void GetPluListByPCtrlRuleID(int nRuleID, CReportConsolidationArray<CSortedStringAndIntByInt64>& arrayPluItems);
};

//**********************************************************************
