#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowUserText.h"
//**********************************************************************

struct RSParams_UserText_NormalNoParams
{
public:
	RSParams_UserText_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_UserText_NormalByFieldLabel
{
public:
	RSParams_UserText_NormalByFieldLabel(CString str)
		: strFieldLabel(str) {}

public:
	CString strFieldLabel;
};

//**********************************************************************

class CSQLRecordSetMetadataUserText : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataUserText(bool bForExistenceCheck = FALSE);
	void AddParamsByFieldLabel(const CString strFieldLabel);

public:
	CString m_strFieldLabel;
	CString m_strUserText;
	BOOL m_bUseFlag;
};

//**********************************************************************

class CSQLRecordSetUserText : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetUserText(CDatabase* pDatabase, RSParams_UserText_NormalNoParams params);
	CSQLRecordSetUserText(CDatabase* pDatabase, RSParams_UserText_NormalByFieldLabel params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromUserTextRow(CSQLRowUserText& RowAudit);
	void SaveToUserTextRow(CSQLRowUserText& RowAudit);

public:
	bool StepSelectAll(CSQLRowUserText& RowAudit);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataUserText m_MD;
};

//**********************************************************************

