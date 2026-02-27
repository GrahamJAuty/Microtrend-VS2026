#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebbleRecentTransaction.h"
//**********************************************************************

struct RSParams_PebbleRecentTransaction_NormalNoParams
{
public:
	RSParams_PebbleRecentTransaction_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebbleRecentTransaction_NormalByTransactionID
{
public:
	RSParams_PebbleRecentTransaction_NormalByTransactionID(CString str)
		: strTransactionID(str) {
	}

public:
	CString strTransactionID;
};

//**********************************************************************

class CSQLRecordSetMetadataPebbleRecentTransaction : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebbleRecentTransaction(bool bForExistenceCheck = FALSE);
	void AddParamsByTransactionID(const CString strTransactionID);

public:
	CString m_strTransactionID;
	CString m_strDateTime;
};

//**********************************************************************

class CSQLRecordSetPebbleRecentTransaction : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebbleRecentTransaction(CDatabase* pDatabase, RSParams_PebbleRecentTransaction_NormalNoParams params);
	CSQLRecordSetPebbleRecentTransaction(CDatabase* pDatabase, RSParams_PebbleRecentTransaction_NormalByTransactionID params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebbleRecentTransactionRow(CSQLRowPebbleRecentTransaction& RowPB);
	void SaveToPebbleRecentTransactionRow(CSQLRowPebbleRecentTransaction& RowBPB);

public:
	bool StepSelectAll(CSQLRowPebbleRecentTransaction& RowBC);

private:
	void CommonInit();	

public:
	CSQLRecordSetMetadataPebbleRecentTransaction m_MD;
};

//**********************************************************************

