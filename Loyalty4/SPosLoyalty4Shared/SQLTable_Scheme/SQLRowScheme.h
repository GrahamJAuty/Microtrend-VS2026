#pragma once
//**********************************************************************
#include "SQLFieldValidatorScheme.h"
//**********************************************************************

class CSQLRowScheme
{
public:
	CSQLRowScheme();
	
private:
	void Reset();
	
public:
	int GetSchemeNo() { return m_nSchemeNo; }
	CString GetSchemeName() { return m_strSchemeName; }
	
public:
	void SetSchemeNo(int n) { vdtr.SetSchemeNo(n, m_nSchemeNo); }
	void SetSchemeName(CString str) { vdtr.SetSchemeName(str, m_strSchemeName); }
	
private:
	int m_nSchemeNo;
	CString m_strSchemeName;

private:
	CSQLFieldValidatorScheme vdtr;
};

//**********************************************************************

