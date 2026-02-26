#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowGroupLegacy.h"
//**********************************************************************

struct RSParams_GroupLegacy_NormalNoParams
{
public:
	RSParams_GroupLegacy_NormalNoParams() {}
};

//**********************************************************************

class CSQLRecordSetMetadataGroupLegacy : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataGroupLegacy();

public:
	int m_nGroupNo;
	/****/
	BOOL m_bRevalueFlag[LEGACY_REVALUE_PLUNO_COUNT + 1];
	BOOL m_bRevalueDisable[LEGACY_REVALUE_PLUNO_COUNT + 1];
	int m_nRevalueType[LEGACY_REVALUE_PLUNO_COUNT + 1];
	CString m_strRevalueTrip[LEGACY_REVALUE_PLUNO_COUNT + 1];
	int m_nRevaluePoints[LEGACY_REVALUE_PLUNO_COUNT + 1];
	CString m_strRevalueBonus[LEGACY_REVALUE_PLUNO_COUNT + 1];
	int m_nRevaluePercent[LEGACY_REVALUE_PLUNO_COUNT + 1];
	/*****/
	int m_nParamGroupNo;
};

//**********************************************************************

class CSQLRecordSetGroupLegacy : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetGroupLegacy(CDatabase* pDatabase, RSParams_GroupLegacy_NormalNoParams params);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromGroupRow(CSQLRowGroupLegacy& RowGroup);
	void SaveToGroupRow(CSQLRowGroupLegacy& RowGroup);

public:
	bool StepSelectAll(CSQLRowGroupLegacy& RowGroup);

public:
	CSQLRecordSetMetadataGroupLegacy m_MD;
};

//**********************************************************************

