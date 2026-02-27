#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebblePendingTransaction.h"
//**********************************************************************

struct RSParams_PebblePendingTransaction_NormalNoParams
{
public:
	RSParams_PebblePendingTransaction_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebblePendingTransaction_NormalByTransactionId
{
public:
	RSParams_PebblePendingTransaction_NormalByTransactionId(CString str)
		: strTransactionId(str) {
	}

public:
	CString strTransactionId;
};

//**********************************************************************

class CSQLRecordSetMetadataPebblePendingTransaction : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebblePendingTransaction(bool bCheckExistence = FALSE);
	void AddParamsByTransactionID(const CString strTransactionID);

public:
	CString m_strTransactionID;
	CString m_strMemberID;
	CString m_strDateTime;
	CString m_strName;
	int m_nAmount;
	BOOL m_bIsSale;
};

//**********************************************************************

class CSQLRecordSetPebblePendingTransaction : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebblePendingTransaction(CDatabase* pDatabase, RSParams_PebblePendingTransaction_NormalNoParams params, bool bUnmatched);
	CSQLRecordSetPebblePendingTransaction(CDatabase* pDatabase, RSParams_PebblePendingTransaction_NormalByTransactionId params, bool bUnmatched, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebblePendingTransactionRow(CSQLRowPebblePendingTransaction& RowPB);
	void SaveToPebblePendingTransactionRow(CSQLRowPebblePendingTransaction& RowBPB);

public:
	bool StepSelectAll(CSQLRowPebblePendingTransaction& RowBC);

public:
	CSQLRecordSetMetadataPebblePendingTransaction m_MD;

private:
	void CommonInit();

private:
	bool m_bUnmatched;
};

//**********************************************************************

