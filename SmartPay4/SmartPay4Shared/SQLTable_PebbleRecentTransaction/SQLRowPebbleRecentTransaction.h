#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebbleRecentTransaction.h"
//**********************************************************************

class CSQLRowPebbleRecentTransaction
{
public:
	CSQLRowPebbleRecentTransaction();
	
private:
	void Reset();
	
public:
	CString GetTransactionID() { return m_strTransactionID; }
	CString GetDateTime() { return m_strDateTime; }
	
public:
	void SetTransactionID(CString str) { vdtr.SetTransactionID(str, m_strTransactionID); }
	void SetDateTime( CString str) { vdtr.SetDateTime(str, m_strDateTime); }

private:
	CString m_strTransactionID;
	CString m_strDateTime;
	
private:
	CSQLFieldValidatorPebbleRecentTransaction vdtr;
};

//**********************************************************************

