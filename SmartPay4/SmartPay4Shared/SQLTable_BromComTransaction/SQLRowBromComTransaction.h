#pragma once
//**********************************************************************
#include "SQLFieldValidatorBromComTransaction.h"
//**********************************************************************

class CSQLRowBromComTransaction
{
public:
	CSQLRowBromComTransaction();
	
private:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	int GetPersonID() { return m_nPersonID; }
	CString GetDetails() { return m_strDetails;  }
	double GetAmount() { return m_dAmount; }
	double GetBalance() { return m_dBalance; }
	CString GetDateTime() { return m_strDateTime; }
	CString GetType() { return m_strType; }
	int GetTransactionID() { return m_nTransactionID; }
	CString GetWebComment() { return m_strWebComment; }

public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetPersonID(int n) { vdtr.SetPersonID(n, m_nPersonID); }
	void SetDetails(CString str) { vdtr.SetDetails(str, m_strDetails); }
	void SetAmount(double d) { vdtr.SetAmount(d, m_dAmount); }
	void SetBalance(double d) { vdtr.SetBalance(d, m_dBalance); }
	void SetDateTime(CString str) { vdtr.SetDateTime(str, m_strDateTime); }
	void SetType(CString str) { vdtr.SetType(str, m_strType); }
	void SetTransactionID(int n) { vdtr.SetTransactionID(n, m_nTransactionID); }
	void SetWebComment(CString str) { vdtr.SetWebComment(str, m_strWebComment); }

private:
	__int64 m_nLineID;
	int m_nPersonID;
	CString m_strDetails;
	double m_dAmount;
	double m_dBalance;
	CString m_strDateTime;
	CString m_strType;
	int m_nTransactionID;
	CString m_strWebComment;

private:
	CSQLFieldValidatorBromComTransaction vdtr;
};

//**********************************************************************
