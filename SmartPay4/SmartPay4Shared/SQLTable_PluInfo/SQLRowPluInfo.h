#pragma once
//**********************************************************************
#include "SQLFieldValidatorPluInfo.h"
//**********************************************************************

class CSQLRowPluInfo
{
public:
	CSQLRowPluInfo();
	
private:
	void Reset();
	
public:
	__int64 GetPluNo() { return m_nPluNo; }
	CString GetDescription() { return m_strDescription; }
	int GetDeptNo() { return m_nDeptNo; }
	CString GetTaxCode() { return m_strTaxCode; }
	int GetModType() { return m_nModType; }
	int GetPoints() { return m_nPoints; }
	int GetPCtrlRuleID() { return m_nPCtrlRuleID; }
	int GetStampWeight() { return m_nStampWeight; }

public:
	void SetPluNo(__int64 n) { vdtr.SetPluNo(n, m_nPluNo); }
	void SetDescription(CString str) { vdtr.SetDescription(str, m_strDescription); }
	void SetDeptNo(int n) { vdtr.SetDeptNo(n, m_nDeptNo); }
	void SetTaxCode(CString str) { vdtr.SetTaxCode(str, m_strTaxCode); }
	void SetModType(int n) { vdtr.SetModType(n, m_nModType); }
	void SetPoints(int n) { vdtr.SetPoints(n, m_nPoints); }
	void SetPCtrlRuleID(int n) { vdtr.SetPCtrlRuleID(n, m_nPCtrlRuleID); }
	void SetStampWeight(int n) { vdtr.SetStampWeight(n, m_nStampWeight); }

public:
	CString GetPluNoString();
	static CString FormatPluNo(const char* szPluNo);
	bool IsBasePlu();
	CString GetMicrotrendBasePluNo(bool bPadout);

private:
	__int64 m_nPluNo;
	CString m_strDescription;
	int m_nDeptNo;
	CString m_strTaxCode;
	int m_nModType;
	int m_nPoints;
	int m_nPCtrlRuleID;
	int m_nStampWeight;

private:
	CSQLFieldValidatorPluInfo vdtr;
};

//**********************************************************************

