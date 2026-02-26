#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
//**********************************************************************
#include "SQLRowAuditPeriod.h"
//**********************************************************************

class CSQLRowSetAuditPeriod
{
public:
	CSQLRowSetAuditPeriod();

public:
	void LoadPeriods(CDatabase* pDatabase);
	void SavePeriods(CDatabase* pDatabase);

public:
	CString GetPeriodName(int n);
	int GetStartTime(int n);
	
public:
	void SetPeriodName(int n, CString strName);
	void SetStartTime(int n, int nTime);

public:
	int GetPeriodIndex(CSSTime* pTime = NULL);
	int GetPeriodIndex(CString strTime);
	CString FormatStartTime(int n);
	bool IsPeriodSet(int n);
	bool GetTimeRange(const char* szLabel, int& nStartHour, int& nStartMins, int& nEndHour, int& nEndMins);
	int GetPeriodCount();

private:
	bool IsValidPeriodIndex(int n);

private:
	CSQLRowAuditPeriod m_arrayPeriods[nMAXPERIODS];
};

//**********************************************************************
extern CSQLRowSetAuditPeriod SQLRowSetAuditPeriod;
//**********************************************************************
