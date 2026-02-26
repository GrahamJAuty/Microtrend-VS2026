#pragma once
//**********************************************************************
#include "SQLFieldValidatorEcrmanDatabase.h"
//**********************************************************************

class CSQLRowEcrmanDatabase
{
public:
	CSQLRowEcrmanDatabase();
	
private:
	void Reset();
	
public:
	int GetDbNo() { return m_nDbNo; }
	CString GetDbName() { return m_strDbName; }
	
public:
	void SetDbNo(int n) { vdtr.SetDbNo(n, m_nDbNo); }
	void SetDbName(CString str) { vdtr.SetDbName(str, m_strDbName); }
	
private:
	int m_nDbNo;
	CString m_strDbName;

private:
	CSQLFieldValidatorEcrmanDatabase vdtr;
};

//**********************************************************************

