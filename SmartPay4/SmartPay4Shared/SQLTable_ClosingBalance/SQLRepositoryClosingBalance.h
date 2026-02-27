#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowClosingBalance.h"
//**********************************************************************

class CSQLRepositoryClosingBalance
{
public:
	CSQLRepositoryClosingBalance();

public:
	CSQLResultInfo SelectRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo SelectFirstRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo SelectLastRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo SelectPreviousRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo SelectNextRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo SelectNearestRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strDate, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);

private:
	CSQLResultInfo SelectInternal(CString strSQL, CSQLRowClosingBalance& SQLRowClosing, CDatabase* pDatabase);
	CString GetAllFieldNames();

public:
	void ImportClosingBalance(CString strFilename);
};

//**********************************************************************
