#pragma once
//**********************************************************************
#include "SQLFieldValidatorPurchaseControlStamp.h"
//**********************************************************************

class CSQLRowPurchaseControlStamp
{
public:
	CSQLRowPurchaseControlStamp();

public:
	bool Compare(CSQLRowPurchaseControlStamp& source);
	
private:
	void Reset();
	
public:
	__int64 GetUserID() { return m_nUserID; }
	int GetRuleID() { return m_nRuleID; }
	CString GetAwardDate() { return m_strAwardDate; }
	CString GetExpireDate() { return m_strExpireDate; }
	int GetStampCount() { return m_nStampCount; }
	bool IsNeverExpire() { return (m_strExpireDate == GetNeverExpireDate()); }
	
public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetRuleID(int n) { vdtr.SetRuleID(n, m_nRuleID); }
	void SetAwardDate(CString str) { vdtr.SetAwardDate(str, m_strAwardDate); }
	void SetExpireDate(CString str) { vdtr.SetExpireDate(str, m_strExpireDate); }
	void SetStampCount(int n) { vdtr.SetStampCount(n, m_nStampCount); }
	void SetNeverExpire() { m_strExpireDate = GetNeverExpireDate(); }

public:
	static CString GetNeverExpireDate() { return "99999999"; }

private:
	__int64 m_nUserID;
	int m_nRuleID;
	CString m_strAwardDate;
	CString m_strExpireDate;
	int m_nStampCount;

private:
	CSQLFieldValidatorPurchaseControlStamp vdtr;
};

//**********************************************************************

