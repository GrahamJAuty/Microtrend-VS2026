#pragma once
//**********************************************************************
#include "SQLFieldValidatorBioRegister.h"
//**********************************************************************

class CSQLRowBioRegister
{
public:
	CSQLRowBioRegister();
	
private:
	void Reset();
	
public:
	__int64 GetUserID() { return m_nUserID; }
	CString GetUsername() { return m_strUsername; }
	int GetBioCounter() { return m_nBioCounter; }
	CString GetRegDate() { return m_strRegDate; }
	CString GetRegTime() { return m_strRegTime; }
	CString GetUnRegDate() { return m_strUnRegDate; }
	CString GetUnRegTime() { return m_strUnRegTime; }
	bool GetUnRegFlag() { return m_bUnReg; }
	CString GetFirstDate() { return m_strFirstDate; }
	CString GetFirstTime() { return m_strFirstTime; }
	CString GetLastDate() { return m_strLastDate; }
	CString GetLastTime() { return m_strLastTime; }

public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUsername(CString str) { vdtr.SetUsername(str, m_strUsername); }
	void SetBioCounter(int n) { vdtr.SetCounter(n, m_nBioCounter); }
	void SetRegDate(CString str) { vdtr.SetDate(str, m_strRegDate); }
	void SetRegTime(CString str) { vdtr.SetDate(str, m_strRegTime); }
	void SetUnRegDate(CString str) { vdtr.SetDate(str, m_strUnRegDate); }
	void SetUnRegTime(CString str) { vdtr.SetDate(str, m_strUnRegTime); }
	void SetUnRegFlag(bool b) { m_bUnReg = b; }
	void SetFirstDate(CString str) { vdtr.SetDate(str, m_strFirstDate); }
	void SetFirstTime(CString str) { vdtr.SetDate(str, m_strFirstTime); }
	void SetLastDate(CString str) { vdtr.SetDate(str, m_strLastDate); }
	void SetLastTime(CString str) { vdtr.SetDate(str, m_strLastTime); }

public:
	void SetRegDate();
	void SetRegTime();
	void SetUnRegDate();
	void SetUnRegTime();
	void SetFirstDate();
	void SetFirstTime();
	void SetLastDate();
	void SetLastTime();

private:
	void IncrementBioCounter();

private:
	__int64 m_nUserID;
	CString m_strUsername;
	int m_nBioCounter;
	CString m_strRegDate;
	CString m_strRegTime;
	CString m_strUnRegDate;
	CString m_strUnRegTime;
	bool m_bUnReg;
	CString m_strFirstDate;
	CString m_strFirstTime;
	CString m_strLastDate;
	CString m_strLastTime;

private:
	CSQLFieldValidatorBioRegister vdtr;
};

//**********************************************************************

