#pragma once
//**********************************************************************
#include "odbcinst.h"
#include "afxdb.h"
//**********************************************************************

class CSQLResultInfo
{
public:
	CSQLResultInfo();

public:
	__int64 GetSQLResult() { return m_nResult; }
	__int64 GetSQLError() { return m_nError; }

public:
	void SetSQLResult(__int64 n) { m_nResult = n; }
	void SetSQLError(__int64 n) { m_nError = n; }

public:
	__int64 m_nResult;
	__int64 m_nError;
};

//**********************************************************************
