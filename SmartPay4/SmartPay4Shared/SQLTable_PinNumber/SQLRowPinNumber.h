#pragma once
//**********************************************************************
#include "SQLFieldValidatorPINNumber.h"
//**********************************************************************

class CSQLRowPINNumber
{
public:
	CSQLRowPINNumber();
	
private:
	void Reset();
	
public:
	CString GetPINNumber() { return m_strPINNumber; }
	CString GetUserID() { return m_strUserID; }
	CString GetUsername() { return m_strUsername; }
	CString GetFirstDate() { return m_strFirstDate; }
	CString GetFirstTime() { return m_strFirstTime; }
	CString GetLastDate() { return m_strLastDate; }
	CString GetLastTime() { return m_strLastTime; }
	int GetUseCounter() { return m_nUseCounter; }

public:
	void SetPINNumber(CString str) { vdtr.SetPINNumber(str, m_strPINNumber); }
	void SetUserID(CString str) { vdtr.SetUserID(str, m_strUserID); }
	void SetUsername(CString str) { vdtr.SetUsername(str, m_strUsername); }
	void SetFirstDate(CString str) { vdtr.SetDate(str, m_strFirstDate); }
	void SetFirstTime(CString str) { vdtr.SetDate(str, m_strFirstTime); }
	void SetLastDate(CString str) { vdtr.SetDate(str, m_strLastDate); }
	void SetLastTime(CString str) { vdtr.SetDate(str, m_strLastTime); }
	void SetUseCounter(int n) { vdtr.SetUseCounter(n, m_nUseCounter); }

public:
	void SetFirstDate();
	void SetFirstTime();
	void SetLastDate();
	void SetLastTime();

public:
	static bool IsValidPINNumber(const CString strPINNumber);

private:
	CString m_strPINNumber = "0000";
	CString m_strUserID = "";
	CString m_strUsername = "";
	CString m_strFirstDate = "";
	CString m_strFirstTime = "";
	CString m_strLastDate = "";
	CString m_strLastTime = "";
	int m_nUseCounter = 0;

private:
	CSQLFieldValidatorPINNumber vdtr;
};

//**********************************************************************

