#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPebblePendingTransaction.h"
//**********************************************************************

class CSQLRepositoryPebblePendingTransaction
{
public:
	CSQLRepositoryPebblePendingTransaction(bool bUnmatched);

public:
	CSQLResultInfo SelectRow(CSQLRowPebblePendingTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebblePendingTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebblePendingTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPebblePendingTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebblePendingTransaction& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strTransactionID, CDatabase* pDatabase);
	
private:
	bool m_bUnmatched;
};

//**********************************************************************
