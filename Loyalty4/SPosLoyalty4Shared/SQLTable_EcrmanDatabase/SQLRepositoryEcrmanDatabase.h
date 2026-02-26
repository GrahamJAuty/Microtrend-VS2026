#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowEcrmanDatabase.h"
//**********************************************************************

class CSQLRepositoryEcrmanDatabase
{
public:
	CSQLRepositoryEcrmanDatabase();

public:
	CSQLResultInfo SelectRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase);
	
public:
	void GetDatabaseNameList(CStringArray& arrayDbNames, CDatabase* pDatabase);

public:
	void ImportEcrmanDatabase(CString strFilename);
};

//**********************************************************************
