#pragma once
//**********************************************************************
#include "SQLFieldValidatorAuditPeriod.h"
//**********************************************************************

class CSQLRowAuditPeriod
{
public:
	CSQLRowAuditPeriod();
	
private:
	void Reset();
	
public:
	int GetPeriodID() { return m_nPeriodID; }
	CString GetPeriodName() { return m_strPeriodName; }
	int GetStartTime() { return m_nStartTime; }

public:
	void SetPeriodID(int n) { vdtr.SetPeriodID(n, m_nPeriodID); }
	void SetPeriodName(CString str) { vdtr.SetPeriodName(str, m_strPeriodName); }
	void SetStartTime(int n) { vdtr.SetStartTime(n, m_nStartTime); }

public:
	CString FormatStartTime();

private:
	int m_nPeriodID;
	CString m_strPeriodName;
	int m_nStartTime;

private:
	CSQLFieldValidatorAuditPeriod vdtr;
};

//**********************************************************************

