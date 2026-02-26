#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLBuilderBind.h"
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************

class CSQLRecordSetAccountUserID : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountUserID(CString strTableName, CDatabase* pDatabase, int nType = ACCOUNTSET_NORMAL, int nBindFields = 0);
	
public:
	virtual CString GetDefaultSQL() { return m_strTableName; }
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void SetSearchBindings(CSQLBuilderBind& BuilderBind);
	bool StepSelect(__int64& nUserID, bool bUpdate = FALSE);

public:
	__int64 m_nUserID;
	CString m_strMemberID;
	CString m_strParamUserIDFrom;
	CString m_strParamUserIDTo;
	/*****/
	CString m_strParamSearch[10];
	/*****/
	CString m_strParamMemberID;

private:
	int m_nRecordSetType;
};

//**********************************************************************

