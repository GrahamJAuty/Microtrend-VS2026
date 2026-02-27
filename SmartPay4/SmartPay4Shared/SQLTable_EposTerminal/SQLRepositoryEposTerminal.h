#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowEposTerminal.h"
//**********************************************************************

class CSQLRepositoryEposTerminal
{
public:
	CSQLRepositoryEposTerminal();

public:
	CSQLResultInfo SelectRow(CSQLRowEposTerminal& SQLRowEP, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowEposTerminal& SQLRowEP, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowEposTerminal& SQLRowEP, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowEposTerminal& SQLRowEP, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowEposTerminal& SQLRowEP, CDatabase* pDatabase);
	
	CSQLResultInfo RowExists(int nTerminalNo, CDatabase* pDatabase);

public:
	bool UpgradeLegacyData(CDatabase* pDatabase, bool bQuitOnFail);
	void ImportTerminalInfo(bool& bDeleteFile, bool& bCommitChanges, CDatabase* pDatabase);
};

//**********************************************************************
