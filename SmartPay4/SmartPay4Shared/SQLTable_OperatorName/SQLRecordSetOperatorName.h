#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowOperatorName.h"
//**********************************************************************

struct RSParams_OperatorName_NormalNoParams
{
public:
	RSParams_OperatorName_NormalNoParams(){}
};

//**********************************************************************

struct RSParams_OperatorName_NormalByOperatorNo
{
public:
	RSParams_OperatorName_NormalByOperatorNo(int n)
		: nOperatorNo(n) {}

public:
	int nOperatorNo;
};

//**********************************************************************

class CSQLRecordSetMetadataOperatorName : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataOperatorName(bool bForExistenceCheck = FALSE);
	void AddParamsByOperatorNo(const int nOpNo);

public:
	int m_nOperatorNo;
	CString m_strOperatorName;
};

//**********************************************************************

class CSQLRecordSetOperatorName : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetOperatorName(CDatabase* pDatabase, RSParams_OperatorName_NormalNoParams params);
	CSQLRecordSetOperatorName(CDatabase* pDatabase, RSParams_OperatorName_NormalByOperatorNo params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromOperatorNameRow(CSQLRowOperatorName& RowON);
	void SaveToOperatorNameRow(CSQLRowOperatorName& RowON);

public:
	bool StepSelectAll(CSQLRowOperatorName& RowON);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataOperatorName m_MD;
};

//**********************************************************************

