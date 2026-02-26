#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowAuditPeriod.h"
//**********************************************************************

struct RSParams_AuditPeriod_NormalNoParams
{
public:
	RSParams_AuditPeriod_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_AuditPeriod_NormalById
{
public:
	RSParams_AuditPeriod_NormalById(int n) 
		: nPeriodID(n) {}

public:
	int nPeriodID;
};

//**********************************************************************

class CSQLRecordSetMetadataAuditPeriod : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAuditPeriod(bool bForExistenceCheck = FALSE);
	void AddParamsById(const int nPeriodId);

public:
	int m_nPeriodID;
	CString m_strPeriodName;
	int m_nStartTime;
};

//**********************************************************************

class CSQLRecordSetAuditPeriod : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAuditPeriod(CDatabase* pDatabase, RSParams_AuditPeriod_NormalNoParams params);
	CSQLRecordSetAuditPeriod(CDatabase* pDatabase, RSParams_AuditPeriod_NormalById params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromAuditPeriodRow(CSQLRowAuditPeriod& RowAudit);
	void SaveToAuditPeriodRow(CSQLRowAuditPeriod& RowAudit);

public:
	bool StepSelectAll(CSQLRowAuditPeriod& RowAudit);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataAuditPeriod m_MD;
};

//**********************************************************************

