#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebbleLinkTime.h"
//**********************************************************************

struct RSParams_PebbleLinkTime_NormalNoParams
{
public:
	RSParams_PebbleLinkTime_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebbleLinkTime_NormalByLineId
{
public:
	RSParams_PebbleLinkTime_NormalByLineId(CString str)
		: strLineId(str) {}

public:
	CString strLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataPebbleLinkTime : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebbleLinkTime(bool bForExistenceCheck = FALSE);
	void AddParamsByLineID(const CString strLineID);

public:
	void AddTextParam(CString strLabel, const CString strDef = "");
	void AddIntParam(CString strLabel, const int nDef = 0);

public:
	__int64 m_nLineID = 0;
	CString m_strDate = "";
	CString m_strTime = "";
	__int64 m_nUserID = 0;
	CString m_strUsername = "";
	CString m_strForename = "";
	CString m_strSurname = "";
	int m_nGroupNo = 0;
	CString m_strPebbleFirstName = "";
	CString m_strPebbleLastName = "";
	int m_nProcessType = 0;
	int m_nMatchType = 0;
	CString m_strMIS = "";
	CString m_strUPN = "";
};

//**********************************************************************

class CSQLRecordSetPebbleLinkTime : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebbleLinkTime(CDatabase* pDatabase, RSParams_PebbleLinkTime_NormalNoParams params);
	CSQLRecordSetPebbleLinkTime(CDatabase* pDatabase, RSParams_PebbleLinkTime_NormalByLineId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebbleLinkTimeRow(CSQLRowPebbleLinkTime& RowBC);
	void SaveToPebbleLinkTimeRow(CSQLRowPebbleLinkTime& RowBC);

public:
	bool StepSelectAll(CSQLRowPebbleLinkTime& RowBC);
	void UpdateParamCount() { m_nParams = m_MD.GetParamCount(); }

private:
	void CommonInit();
	
public:
	CSQLRecordSetMetadataPebbleLinkTime m_MD;
};

//**********************************************************************
