#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowScheme.h"
//**********************************************************************

struct RSParams_Scheme_NormalNoParams
{
public:
	RSParams_Scheme_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_Scheme_NormalBySchemeNo
{
public:
	RSParams_Scheme_NormalBySchemeNo(int n) 
		: nSchemeNo(n) {}

public:
	int nSchemeNo;
};

//**********************************************************************

class CSQLRecordSetMetadataScheme : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataScheme( bool bForExistenceCheck = FALSE);
	void AddParamsBySchemeNo(const int nSchemeNo);

public:
	int m_nSchemeNo;
	CString m_strSchemeName;
};

//**********************************************************************

class CSQLRecordSetScheme : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetScheme(CDatabase* pDatabase, RSParams_Scheme_NormalNoParams params);
	CSQLRecordSetScheme(CDatabase* pDatabase, RSParams_Scheme_NormalBySchemeNo params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromSchemeRow(CSQLRowScheme& RowSC);
	void SaveToSchemeRow(CSQLRowScheme& RowSC);

public:
	bool StepSelectAll(CSQLRowScheme& RowSC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataScheme m_MD;
};

//**********************************************************************

