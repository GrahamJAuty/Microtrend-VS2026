#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowDbVersion.h"
//**********************************************************************

enum class RSDataType_DbVersion : int
{
	Normal = 0,
	ExtVersion = 1
};

//**********************************************************************

struct RSParams_DbVersion_NormalNoParams
{
public:
	RSParams_DbVersion_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_DbVersion_ExtVersionNoParams
{
public:
	RSParams_DbVersion_ExtVersionNoParams() {}
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

struct RSParams_DbVersion_ExtVersionByLineId
{
public:
	RSParams_DbVersion_ExtVersionByLineId(int n)
		: nLineId(n) {}

public:
	int nLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataDbVersion : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataDbVersion(RSDataType_DbVersion type);
	void AddParamsByLineID(const int nLineID);

public:
	int m_nLineID;
	int m_nVersion;
	int m_nExternalVersion1;
	
public:
	RSDataType_DbVersion m_type;
};

//**********************************************************************

class CSQLRecordSetDbVersion : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalNoParams params);
	CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_ExtVersionNoParams params);
	CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalByLineId params);
	CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_ExtVersionByLineId params);

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

private:
	bool m_bIncludeExternal;
	RSDataType_DbVersion m_type;
};

//**********************************************************************

