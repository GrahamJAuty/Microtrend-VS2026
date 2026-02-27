#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetBioRegister.h"
//**********************************************************************

class CSQLRepositoryBioRegister
{
public:
	CSQLRepositoryBioRegister();

public:
	CSQLResultInfo SelectRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase);
	CSQLResultInfo GetSortedUserIDList(CReportConsolidationArray<CSortedInt64Item>& arrayUserIDs, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nUserID, CDatabase* pDatabase);	
};

//**********************************************************************
