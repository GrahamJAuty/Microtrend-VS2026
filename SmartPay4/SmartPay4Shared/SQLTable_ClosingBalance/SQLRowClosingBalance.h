#pragma once
//**********************************************************************
#include "SQLFieldValidatorClosingBalance.h"
//**********************************************************************

class CSQLRowClosingBalance
{
public:
	CSQLRowClosingBalance();

public:
	void Reset();

public:
	CString GetKeyDate() { return m_strKeyDate; }
	/*****/
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	/*****/
	CString GetArchiveDate() { return m_strArchiveDate; }
	CString GetArchiveTime() { return m_strArchiveTime; }
	/*****/
	double GetPurse1LiabilityAudit() { return m_dPurse1LiabilityAudit; }
	double GetPurse1CreditAudit() { return m_dPurse1CreditAudit; }
	/*****/
	double GetPurse2BalanceAudit() { return m_dPurse2BalanceAudit; }
	/*****/
	double GetPurse3LiabilityAudit() { return m_dPurse3LiabilityAudit; }
	double GetPurse3CreditAudit() { return m_dPurse3CreditAudit; }
	/*****/
	bool GetHaveAdjustFlag() { return m_bHaveAdjust; }
	/*****/
	double GetPurse1LiabilityAdjust() { return m_dPurse1LiabilityAdjust; }
	double GetPurse1CreditAdjust() { return m_dPurse1CreditAdjust; }
	/*****/
	double GetPurse3LiabilityAdjust() { return m_dPurse3LiabilityAdjust; }
	double GetPurse3CreditAdjust() { return m_dPurse3CreditAdjust; }

public:
	double GetPurse1LiabilityDatabase() { return m_dPurse1LiabilityAudit + m_dPurse1LiabilityAdjust; }
	double GetPurse1CreditDatabase() { return m_dPurse1CreditAudit + m_dPurse1CreditAdjust; }
	double GetPurse3LiabilityDatabase() { return m_dPurse3LiabilityAudit + m_dPurse3LiabilityAdjust; }
	double GetPurse3CreditDatabase() { return m_dPurse3CreditAudit + m_dPurse3CreditAdjust; }

public:
	bool IsArchived() { return GetArchiveDate() != ""; }

public:
	void AddPurse1LiabilityAudit(double d) { m_dPurse1LiabilityAudit += d;  }
	void AddPurse1CreditAudit(double d) { m_dPurse1CreditAudit += d; }
	void AddPurse2BalanceAudit(double d) { m_dPurse2BalanceAudit += d; }
	void AddPurse3LiabilityAudit(double d) { m_dPurse3LiabilityAudit += d; }
	void AddPurse3CreditAudit(double d) { m_dPurse3CreditAudit += d; }

public:
	void SetKeyDate(CString str) { vdtr.SetKeyDate(str, m_strKeyDate); }
	/*****/
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	/*****/
	void SetArchiveDate(CString str) { vdtr.SetArchiveDate(str, m_strArchiveDate); }
	void SetArchiveTime(CString str) { vdtr.SetArchiveTime(str, m_strArchiveTime); }
	/*****/
	void SetPurse1LiabilityAudit(double d) { vdtr.SetPurse1LiabilityAudit(d, m_dPurse1LiabilityAudit); }
	void SetPurse1CreditAudit(double d) { vdtr.SetPurse1CreditAudit(d, m_dPurse1CreditAudit); }
	/*****/
	void SetPurse2BalanceAudit(double d) { vdtr.SetPurse2BalanceAudit(d, m_dPurse2BalanceAudit); }
	/*****/
	void SetPurse3LiabilityAudit(double d) { vdtr.SetPurse3LiabilityAudit(d, m_dPurse3LiabilityAudit); }
	void SetPurse3CreditAudit(double d) { vdtr.SetPurse3CreditAudit(d, m_dPurse3CreditAudit); }
	/*****/
	void SetHaveAdjustFlag(bool b) { m_bHaveAdjust = b; }
	/*****/
	void SetPurse1LiabilityAdjust(double d) { vdtr.SetPurse1LiabilityAdjust(d, m_dPurse1LiabilityAdjust); }
	void SetPurse1CreditAdjust(double d) { vdtr.SetPurse1CreditAdjust(d, m_dPurse1CreditAdjust); }
	/*****/
	void SetPurse3LiabilityAdjust(double d) { vdtr.SetPurse3LiabilityAdjust(d, m_dPurse3LiabilityAdjust); }
	void SetPurse3CreditAdjust(double d) { vdtr.SetPurse3CreditAdjust(d, m_dPurse3CreditAdjust); }

public:
	void SetArchived();

public:
	static CString MakeKeyDate(const COleDateTime& date);
	static CString MakeDbDate(const COleDateTime& date);
	static CString MakeDbTime(const COleDateTime& date);
	static CString MakeKeyDate(const CString strDate);
	static CString MakeKeyDate(CSSDate& date);

private:
	CString m_strKeyDate;
	/*****/
	CString m_strDate;
	CString m_strTime;
	/*****/
	CString m_strArchiveDate;
	CString m_strArchiveTime;
	/*****/
	double m_dPurse1LiabilityAudit;
	double m_dPurse1CreditAudit;
	/*****/
	double m_dPurse2BalanceAudit;
	/*****/
	double m_dPurse3LiabilityAudit;
	double m_dPurse3CreditAudit;
	/*****/
	bool m_bHaveAdjust;
	/*****/
	double m_dPurse1LiabilityAdjust;
	double m_dPurse1CreditAdjust;
	/*****/
	double m_dPurse3LiabilityAdjust;
	double m_dPurse3CreditAdjust;

private:
	CSQLFieldValidatorClosingBalance vdtr;
};

//**********************************************************************

