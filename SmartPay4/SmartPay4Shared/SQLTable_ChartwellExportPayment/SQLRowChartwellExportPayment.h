#pragma once
//**********************************************************************
#include "SQLFieldValidatorChartwellExportPayment.h"
//**********************************************************************

class CSQLRowChartwellExportPayment
{
public:
	CSQLRowChartwellExportPayment();

public:
	void Reset();

public:
	__int64 GetUserID() { return m_nUserID; }
	__int64 GetTransactionID() { return m_nTransactionID; }
	int GetTerminalNo() { return m_nTerminalNo; }
	CString GetDate() { return m_strDate; }
	CString GetCreateDateTime() { return m_strCreateDateTime; }
	int GetGroupNo() { return m_nGroupNo; }
	double GetPaymentPurse1() { return m_dPaymentPurse1; }
	double GetPaymentPurse2() { return m_dPaymentPurse2; }
	double GetPaymentPurse3() { return m_dPaymentPurse3; }

public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetTransactionID(__int64 n) { vdtr.SetTransactionID(n, m_nTransactionID); }
	void SetTerminalNo(int n) { vdtr.SetTerminalNo(n, m_nTerminalNo); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetCreateDateTime(CString str) { vdtr.SetCreateDateTime(str, m_strCreateDateTime); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetPaymentPurse1(double d) { vdtr.SetPaymentPurse(d, m_dPaymentPurse1); }
	void SetPaymentPurse2(double d) { vdtr.SetPaymentPurse(d, m_dPaymentPurse2); }
	void SetPaymentPurse3(double d) { vdtr.SetPaymentPurse(d, m_dPaymentPurse3); }

public:
	void ConsolidatePayment(double dPurse1, double dPurse2, double dPurse3);
	
private:
	__int64 m_nUserID;
	__int64 m_nTransactionID;
	int m_nTerminalNo;
	CString m_strDate;
	CString m_strCreateDateTime;
	int m_nGroupNo;
	double m_dPaymentPurse1;
	double m_dPaymentPurse2;
	double m_dPaymentPurse3;

private:
	CSQLFieldValidatorChartwellExportPayment vdtr;
};

//**********************************************************************
