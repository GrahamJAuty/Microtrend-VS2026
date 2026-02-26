#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
#include "..\SQLDefines.h"
//**********************************************************************
#include "SQLRowAccountFull.h"
//**********************************************************************

struct RSParams_AccountFull_ById
{
public:
	RSParams_AccountFull_ById(CString str) : 
		strUserID(str) {}

public:
	CString strUserID;
};

//**********************************************************************

class CSQLRecordSetMetadataAccountFull : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountFull();
	~CSQLRecordSetMetadataAccountFull(){}
	void AddParamsByUserID(const CString strUserID);
	
public:
	__int64 m_nUserID;
	int m_nGroupNo;
	CString m_strInfo3;
	CString m_strInfo4;
};

//**********************************************************************

class CSQLRecordSetAccountFull : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ById params);
	
public:
	virtual CString GetDefaultSQL() { return m_strMyDefaultSQL; }
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void LoadFromAccountRow(CSQLRowAccountFull& RowAccount);
	void SaveToAccountRow(CSQLRowAccountFull& RowAccount);

public:
	bool StepSelectAll(CSQLRowAccountFull& RowAccount);
	
private:
	void CommonInit();

public:
	CSQLRecordSetMetadataAccountFull m_MD;

private:
	CString m_strAccountTableName;
	CString m_strMyDefaultSQL;
};

//**********************************************************************

