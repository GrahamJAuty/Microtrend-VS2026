#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowWebPaymentOptions.h"
//**********************************************************************

class CSQLRepositoryWebPaymentOptions
{
public:
	CSQLRepositoryWebPaymentOptions();

public:
	void LoadRow(CDatabase* pDatabase);
	__int64 SaveRow(CDatabase* pDatabase, bool bCheckVersion = FALSE);
	bool IsSet();
	bool CheckVersionChange();

public:
	CSQLResultInfo SelectRow(CSQLRowWebPaymentOptions& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowWebPaymentOptions& SQLRowPB, CDatabase* pDatabase, CString strVersion = "");
	CSQLResultInfo InsertRow(CSQLRowWebPaymentOptions& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowWebPaymentOptions& SQLRowPB, CDatabase* pDatabase, CString strVersion = "");
	CSQLResultInfo DeleteRow(CSQLRowWebPaymentOptions& SQLRowPB, CDatabase* pDatabase);	
	CSQLResultInfo RowExists(int nLineID, CDatabase* pDatabase);

public:
	bool ImportLegacyOptions(CDatabase* pDatabase);

public:
	CSQLRowWebPaymentOptions m_Row;
};

//**********************************************************************
