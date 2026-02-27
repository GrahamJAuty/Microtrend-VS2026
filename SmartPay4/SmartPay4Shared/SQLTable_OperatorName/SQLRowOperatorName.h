#pragma once
//**********************************************************************
#include "SQLFieldValidatorOperatorName.h"
//**********************************************************************

class CSQLRowOperatorName
{
public:
	CSQLRowOperatorName();
	
private:
	void Reset();
	
public:
	int GetOperatorNo() { return m_nOperatorNo; }
	CString GetOperatorName() { return m_strOperatorName; }

public:
	void SetOperatorNo(int n) { vdtr.SetOperatorNo(n, m_nOperatorNo); }
	void SetOperatorName( CString str) { vdtr.SetOperatorName(str, m_strOperatorName); }
	
private:
	int m_nOperatorNo;
	CString m_strOperatorName;
	
private:
	CSQLFieldValidatorOperatorName vdtr;
};

//**********************************************************************

