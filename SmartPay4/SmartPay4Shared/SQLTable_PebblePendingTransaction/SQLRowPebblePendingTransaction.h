#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebblePendingTransaction.h"
//**********************************************************************

class CSQLRowPebblePendingTransaction
{
public:
	CSQLRowPebblePendingTransaction();
	
private:
	void Reset();
	
public:
	CString GetTransactionID() { return m_strTransactionID; }
	CString GetMemberID() { return m_strMemberID; }
	CString GetDateTime() { return m_strDateTime; }
	CString GetName() { return m_strName; }
	int GetAmount() { return m_nAmount; }
	bool GetIsSaleFlag() { return m_bIsSale; }
	
public:
	void SetTransactionID(CString str) { vdtr.SetTransactionID(str, m_strTransactionID); }
	void SetMemberID(CString str) { vdtr.SetMemberID(str, m_strMemberID); }
	void SetDateTime( CString str) { vdtr.SetDateTime(str, m_strDateTime); }
	void SetName(CString str) { vdtr.SetName(str, m_strName); }
	void SetAmount(int n) { vdtr.SetAmount(n, m_nAmount); }
	void SetIsSaleFlag(bool b) { m_bIsSale = b; }

private:
	CString m_strTransactionID;
	CString m_strMemberID;
	CString m_strDateTime;
	CString m_strName;
	int m_nAmount;
	bool m_bIsSale;
	
private:
	CSQLFieldValidatorPebblePendingTransaction vdtr;
};

//**********************************************************************

