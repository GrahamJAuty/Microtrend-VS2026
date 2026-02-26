#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowScheme.h"
//**********************************************************************

class CSQLRepositoryScheme
{
public:
	CSQLRepositoryScheme();

public:
	CSQLResultInfo SelectRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(int nSchemeNo, CDatabase* pDatabase);
	
public:
	void GetSchemeNameList(CReportConsolidationArray<CSortedStringByInt>& arrayNames, int nCurrentNo, bool bAutoFirst10, CDatabase* pDatabase);
	void SaveSchemeNameList(CReportConsolidationArray<CSortedStringByInt>& arrayNames);

public:
	void ImportScheme(CString strFilename);
};

//**********************************************************************

