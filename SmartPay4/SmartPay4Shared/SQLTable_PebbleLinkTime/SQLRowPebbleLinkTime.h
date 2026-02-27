#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebbleLinkTime.h"
//**********************************************************************

class CSQLRowPebbleLinkTime
{
public:
	CSQLRowPebbleLinkTime();

private:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	__int64 GetUserID() { return m_nUserID; }
	CString GetUsername() { return m_strUsername; }
	CString GetForename() { return m_strForename; }
	CString GetSurname() { return m_strSurname; }
	int GetGroupNo() { return m_nGroupNo; }
	CString GetPebbleFirstName() { return m_strPebbleFirstName; }
	CString GetPebbleLastName() { return m_strPebbleLastName; }
	int GetProcessType() { return m_nProcessType; }
	int GetMatchType() { return m_nMatchType; }
	CString GetMIS() { return m_strMIS; }
	CString GetUPN() { return m_strUPN; }

public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUsername(CString str) { vdtr.SetUsername(str, m_strUsername); }
	void SetForename(CString str) { vdtr.SetForename(str, m_strForename); }
	void SetSurname(CString str) { vdtr.SetSurname(str, m_strSurname); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetPebbleFirstName(CString str) { vdtr.SetPebbleFirstName(str, m_strPebbleFirstName); }
	void SetPebbleLastName(CString str) { vdtr.SetPebbleLastName(str, m_strPebbleLastName); }
	void SetProcessType(int n) { vdtr.SetProcessType(n, m_nProcessType); }
	void SetMatchType(int n) { vdtr.SetMatchType(n, m_nMatchType); }
	void SetMIS(CString str) { vdtr.SetMIS(str, m_strMIS); }
	void SetUPN(CString str) { vdtr.SetUPN(str, m_strUPN); }
	
private:
	__int64 m_nLineID = 0;
	CString m_strDate = "";	
	CString m_strTime = "";
	__int64 m_nUserID = 0;
	CString m_strUsername = "";
	CString m_strForename = "";
	CString m_strSurname = "";
	int m_nGroupNo = 0;
	CString m_strPebbleFirstName = "";
	CString m_strPebbleLastName = "";
	int m_nProcessType = 0;
	int m_nMatchType = 0;
	CString m_strMIS = "";
	CString m_strUPN = "";

private:
	CSQLFieldValidatorPebbleLinkTime vdtr;
};

//**********************************************************************
