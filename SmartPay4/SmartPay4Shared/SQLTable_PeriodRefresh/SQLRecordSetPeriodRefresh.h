#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPeriodRefresh.h"
//**********************************************************************

struct RSParams_PeriodRefresh_NormalNoParams
{
public:
	RSParams_PeriodRefresh_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PeriodRefresh_NormalByUserID
{
public:
	RSParams_PeriodRefresh_NormalByUserID(CString str)
		: strUserID(str) {}

public:
	CString strUserID;
};

//**********************************************************************

class CSQLRecordSetMetadataPeriodRefresh : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPeriodRefresh(bool bForExistenceCheck = FALSE);
	void AddParamsByUserID(const CString strUserID);

public:
	__int64 m_nUserID;
	CString m_strPeriod1Date;
	CString m_strPeriod1Balance;
	CString m_strPeriod2Date;
	CString m_strPeriod2Balance;
	CString m_strPeriod3Date;
	CString m_strPeriod3Balance;
	CString m_strPeriod4Date;
	CString m_strPeriod4Balance;
	CString m_strPeriod5Date;
	CString m_strPeriod5Balance;
	CString m_strPeriod6Date;
	CString m_strPeriod6Balance;
};

//**********************************************************************

class CSQLRecordSetPeriodRefresh : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPeriodRefresh(CDatabase* pDatabase, RSParams_PeriodRefresh_NormalNoParams params);
	CSQLRecordSetPeriodRefresh(CDatabase* pDatabase, RSParams_PeriodRefresh_NormalByUserID params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPeriodRefreshRow(CSQLRowPeriodRefresh& RowPeriod);
	void SaveToPeriodRefreshRow(CSQLRowPeriodRefresh& RowPeriod);

public:
	bool StepSelectAll(CSQLRowPeriodRefresh& RowPeriod);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPeriodRefresh m_MD;
};

//**********************************************************************

