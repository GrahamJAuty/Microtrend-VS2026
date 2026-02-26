#pragma once
//**********************************************************************
#include "..\ConsolidationMap.h"
#include "..\ReportConsolidationMap.h"
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
	CSQLResultInfo RowExists(int nDbNo, __int64 nPluNo, CDatabase* pDatabase);

public:
	void ImportPluInfo(int nDbNo, CString strFilename);
	void GetPluCountsByStampOfferID(CReportConsolidationMap<CMapKeyInt, CMapDataSortedInt>& arrayPluCounts);
	void GetPluListByStampOfferID( int nOfferID, CReportConsolidationArray<CSortedStringByIntAndInt64>& arrayPluItems);
};

//**********************************************************************

