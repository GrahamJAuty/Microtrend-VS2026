#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\\SQLConnectionPool.h"
#include "..\\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowCCNo.h"
//**********************************************************************

struct RSParams_CCNo_NormalNoParams
{
public:
	RSParams_CCNo_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_CCNo_NormalByType
{
public:
	RSParams_CCNo_NormalByType(CString str) 
		: strType(str) {}

public:
	CString strType;
};

//**********************************************************************

class CSQLRecordSetMetadataCCNo : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataCCNo(bool bForExistenceCheck = FALSE);
	void AddParamsByType(const CString strType);

public:
	CString m_strType;
	int m_nNumber;
};

//**********************************************************************

class CSQLRecordSetCCNo : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetCCNo(CDatabase* pDatabase, RSParams_CCNo_NormalNoParams params);
	CSQLRecordSetCCNo(CDatabase* pDatabase, RSParams_CCNo_NormalByType params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromCCNoRow(CSQLRowCCNo& RowCCNo);
	void SaveToCCNoRow(CSQLRowCCNo& RowCCNo);

public:
	bool StepSelectAll(CSQLRowCCNo& RowCCNo);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataCCNo m_MD;
};

//**********************************************************************

