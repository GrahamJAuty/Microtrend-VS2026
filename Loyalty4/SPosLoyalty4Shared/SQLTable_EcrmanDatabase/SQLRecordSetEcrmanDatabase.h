#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowEcrmanDatabase.h"
//**********************************************************************

struct RSParams_EcrmanDatabase_NormalNoParams
{
public:
	RSParams_EcrmanDatabase_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_EcrmanDatabase_NormalByDbNo
{
public:
	RSParams_EcrmanDatabase_NormalByDbNo(int n) 
		: nDbNo(n) {}

public:
	int nDbNo;
};

//**********************************************************************

class CSQLRecordSetMetadataEcrmanDatabase : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataEcrmanDatabase(bool bForExistenceCheck = FALSE);
	void AddParamsByDbNo(const int nDbNo);

public:
	int m_nDbNo;
	CString m_strDbName;
};

//**********************************************************************

class CSQLRecordSetEcrmanDatabase : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetEcrmanDatabase(CDatabase* pDatabase, RSParams_EcrmanDatabase_NormalNoParams params);
	CSQLRecordSetEcrmanDatabase(CDatabase* pDatabase, RSParams_EcrmanDatabase_NormalByDbNo params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromEcrmanDatabaseRow(CSQLRowEcrmanDatabase& RowOS);
	void SaveToEcrmanDatabaseRow(CSQLRowEcrmanDatabase& RowOS);

public:
	bool StepSelectAll(CSQLRowEcrmanDatabase& RowOS);

private:
	void CommonInit();	

public:
	CSQLRecordSetMetadataEcrmanDatabase m_MD;
};

//**********************************************************************

