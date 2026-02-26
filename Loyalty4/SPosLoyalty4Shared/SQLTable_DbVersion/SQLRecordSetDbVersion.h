#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowDbVersion.h"
//**********************************************************************

struct RSParams_DbVersion_NormalNoParams
{
public:
	RSParams_DbVersion_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_DbVersion_NormalByLineId
{
public:
	RSParams_DbVersion_NormalByLineId(int n) 
		: nLineId(n) {}

public:
	int nLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataDbVersion : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataDbVersion();
	void AddParamsByLineId(const int nLineId);

public:
	int m_nLineID;
	int m_nVersion;
};

//**********************************************************************

class CSQLRecordSetDbVersion : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalNoParams params);
	CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalByLineId params);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromDbVersionRow(CSQLRowDbVersion& RowAccount);
	void SaveToDbVersionRow(CSQLRowDbVersion& RowAccount);

public:
	bool StepSelectAll(CSQLRowDbVersion& RowAccount);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataDbVersion m_MD;
};

//**********************************************************************

