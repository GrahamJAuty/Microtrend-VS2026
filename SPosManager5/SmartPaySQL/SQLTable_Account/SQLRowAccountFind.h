#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowAccountFind
{
public:
	CSQLRowAccountFind();

public:
	int GetLeaverYear() { return m_nLeaverYear; }
	__int64 GetUserID() { return m_nUserID; }
	CString GetUsername();
	CString GetForename() { return m_strForename; }
	CString GetSurname() { return m_strSurname; }
	CString GetExpiryDate() { return m_strExpiryDate; }
	int GetGroupNo() { return m_nGroupNo; }
	bool GetInactive() { return m_bInactive; }
	/*****/
	CString GetMemberID() { return m_strMemberID; }
	CString GetInfo1() { return m_strInfo1; }
	CString GetInfo2() { return m_strInfo2; }
	CString GetInfo3() { return m_strInfo3; }
	CString GetInfo4() { return m_strInfo4; }
	CString GetInfo5() { return m_strInfo5; }
	CString GetInfo6() { return m_strInfo6; }
	CString GetInfo7() { return m_strInfo7; }
	CString GetInfo8() { return m_strInfo8; }
	int GetAlertCode() { return m_nAlertCode; }

public:
	void SetLeaverYear(int n) { vdtr.SetLeaverYear(n, m_nLeaverYear); }
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUsername(CString str) { vdtr.SetUsername(str, m_strUsername); }
	void SetForename(CString str) { vdtr.SetForename(str, m_strForename); }
	void SetSurname(CString str) { vdtr.SetSurname(str, m_strSurname); }
	void SetExpiryDate(CString str) { vdtr.SetExpiryDate(str, m_strExpiryDate); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetInactive(bool b) { m_bInactive = b; }
	/*****/
	void SetMemberID(CString str) { vdtr.SetMemberID(str, m_strMemberID); }
	void SetInfo1(CString str) { vdtr.SetInfo1(str, m_strInfo1); }
	void SetInfo2(CString str) { vdtr.SetInfo2(str, m_strInfo2); }
	void SetInfo3(CString str) { vdtr.SetInfo3(str, m_strInfo3); }
	void SetInfo4(CString str) { vdtr.SetInfo4(str, m_strInfo4); }
	void SetInfo5(CString str) { vdtr.SetInfo5(str, m_strInfo5); }
	void SetInfo6(CString str) { vdtr.SetInfo6(str, m_strInfo6); }
	void SetInfo7(CString str) { vdtr.SetInfo7(str, m_strInfo7); }
	void SetInfo8(CString str) { vdtr.SetInfo8(str, m_strInfo8); }
	void SetAlertCode(int n) { vdtr.SetAlertCode(n, m_nAlertCode); }

public:
	bool HasUserExpired();
	int GetRecordStatusColour();
	bool IsAlertSet() { return (GetAlertCode() != 0); }

private:
	int m_nLeaverYear;
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strForename;
	CString m_strSurname;
	CString m_strExpiryDate;
	int m_nGroupNo;
	bool m_bInactive;
	/*****/
	CString m_strMemberID;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strInfo5;
	CString m_strInfo6;
	CString m_strInfo7;
	CString m_strInfo8;
	int m_nAlertCode;

private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************

