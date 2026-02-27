#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowOperatorName.h"
//**********************************************************************

class CSQLRepositoryOperatorName
{
public:
	CSQLRepositoryOperatorName();

public:
	CSQLResultInfo SelectRow(CSQLRowOperatorName& SQLRowON, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowOperatorName& SQLRowON, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowOperatorName& SQLRowON, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowOperatorName& SQLRowON, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowOperatorName& SQLRowON, CDatabase* pDatabase);
	
	CSQLResultInfo RowExists(int nOperatorNo, CDatabase* pDatabase);

public:
	void ImportOperatorInfo(CString strFilename);
};

//**********************************************************************
