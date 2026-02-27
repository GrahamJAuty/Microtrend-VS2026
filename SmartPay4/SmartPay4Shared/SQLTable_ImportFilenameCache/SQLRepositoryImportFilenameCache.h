#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetImportFilenameCache.h"
//**********************************************************************

class CSQLRepositoryImportFilenameCache
{
public:
	CSQLRepositoryImportFilenameCache();

public:
	CSQLResultInfo SelectRow(CSQLRowImportFilenameCache& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowImportFilenameCache& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowImportFilenameCache& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowImportFilenameCache& SQLRowBC, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nLineID, CDatabase* pDatabase);
	//CSQLResultInfo CountRows(CDatabase* pDatabase);

public:
	bool CheckCache(int nType, CString strFilename, int nHours);
};

//**********************************************************************

