#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowGroupFull.h"
//**********************************************************************

struct RSParams_Group_NormalByGroupNo
{
public:
	RSParams_Group_NormalByGroupNo(int n)
		: nGroupNo(n) {}

public:
	int nGroupNo;
};

//**********************************************************************

class CSQLRecordSetMetadataGroupFull : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataGroupFull();
	void AddParamsByGroupNo(const int nGroupNo);

public:
	int m_nGroupNo;
	CString m_strGroupName;
	int m_nPriceLevel;
};

//**********************************************************************

class CSQLRecordSetGroupFull : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_NormalByGroupNo params);

public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromGroupRow(CSQLRowGroupFull& RowGroup);
	void SaveToGroupRow(CSQLRowGroupFull& RowGroup);

public:
	bool StepSelectAll(CSQLRowGroupFull& RowGroup);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataGroupFull m_MD;
};

//**********************************************************************

