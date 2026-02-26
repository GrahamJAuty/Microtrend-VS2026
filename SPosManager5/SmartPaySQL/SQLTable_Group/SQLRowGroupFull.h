#pragma once
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************

class CSQLRowGroupFull
{
public:
	CSQLRowGroupFull();
	
public:
	void Reset();
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	CString GetGroupName() { return m_strGroupName; }
	int GetPriceLevel() { return m_nPriceLevel;  }
	
public:
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetGroupName(CString str) { vdtr.SetGroupName(str, m_strGroupName); }
	void SetPriceLevel(int n) { vdtr.SetPriceLevel(n, m_nPriceLevel); }
	
private:
	int m_nGroupNo;
	CString m_strGroupName;
	int m_nPriceLevel;
	
private:
	CSQLFieldValidatorGroup vdtr;
};

//**********************************************************************

