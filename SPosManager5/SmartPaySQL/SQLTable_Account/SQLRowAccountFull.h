#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowAccountFull
{
public:
	CSQLRowAccountFull();
	CSQLRowAccountFull(const CSQLRowAccountFull& source);
	CSQLRowAccountFull& operator= (const CSQLRowAccountFull& source);
	
private:
	void Reset();
	
public:
	__int64 GetUserID() { return m_nUserID; }
	CString GetUserIDString();
	int GetGroupNo() { return m_nGroupNo; }
	CString GetInfo3() { return m_strInfo3; }
	CString GetInfo4() { return m_strInfo4; }
	
public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUserIDString(CString str) { vdtr.SetUserID(_atoi64(str), m_nUserID); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetInfo3(CString str) { vdtr.SetInfo3(str, m_strInfo3); }
	void SetInfo4(CString str) { vdtr.SetInfo4(str, m_strInfo4); }

private:
	__int64 m_nUserID;
	int m_nGroupNo;
	CString m_strInfo3;
	CString m_strInfo4;

private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************

