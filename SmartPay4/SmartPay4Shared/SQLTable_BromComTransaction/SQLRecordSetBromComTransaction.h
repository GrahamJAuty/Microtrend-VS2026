#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowBromComTransaction.h"
//**********************************************************************

struct RSParams_BromComTransaction_NormalNoParams
{
public:
	RSParams_BromComTransaction_NormalNoParams(){}
};

//**********************************************************************

struct RSParams_BromComTransaction_NormalByLineId
{
public:
	RSParams_BromComTransaction_NormalByLineId(CString str) 
		: strLineID(str) {}

public:
	CString strLineID;
};

//**********************************************************************

class CSQLRecordSetMetadataBromComTransaction : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataBromComTransaction(bool bForExistenceCheck = FALSE);
	void AddParamsByLineID(const CString strLineID);

public:
	__int64 m_nLineID;
	int m_nPersonID;
	CString m_strDetails;
	CString m_strAmount;
	CString m_strBalance;
	CString m_strDateTime;
	CString m_strType;
	int m_nTransactionID;
	CString m_strWebComment;
};

//**********************************************************************

class CSQLRecordSetBromComTransaction : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetBromComTransaction(CDatabase* pDatabase, RSParams_BromComTransaction_NormalNoParams param);
	CSQLRecordSetBromComTransaction(CDatabase* pDatabase, RSParams_BromComTransaction_NormalByLineId param, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromBromComTransactionRow(CSQLRowBromComTransaction& RowBC);
	void SaveToBromComTransactionRow(CSQLRowBromComTransaction& RowBC);

public:
	bool StepSelectAll(CSQLRowBromComTransaction& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataBromComTransaction m_MD;
};

//**********************************************************************

