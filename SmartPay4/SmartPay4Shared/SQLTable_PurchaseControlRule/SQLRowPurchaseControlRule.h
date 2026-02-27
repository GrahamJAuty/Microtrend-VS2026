#pragma once
//**********************************************************************
#include "SQLFieldValidatorPurchaseControlRule.h"
//**********************************************************************

class CSQLRowPurchaseControlRule
{
public:
	CSQLRowPurchaseControlRule();
	
private:
	void Reset();
	
public:
	int GetRuleID() { return m_nRuleID; }
	bool GetEnableFlag() { return m_bEnabled; }
	int GetThreshold() { return m_nThreshold; }
	int GetExpiryDays() { return m_nExpiryDays; }
	CString GetDescription() { return m_strDescription; }
	int GetPluCount() { return m_nPluCount; }
	
public:
	void SetRuleID(int n) { vdtr.SetRuleID(n, m_nRuleID); }
	void SetEnableFlag(bool b ) { m_bEnabled = b; }
	void SetThreshold(int n) { vdtr.SetThreshold(n, m_nThreshold); }
	void SetExpiryDays(int n) { vdtr.SetExpiryDays(n, m_nExpiryDays); }
	void SetDescription(CString str) { vdtr.SetDescription(str, m_strDescription); }
	void SetPluCount( int n ) { m_nPluCount = n; }

public:
	CString GetDisplayDescription();
	
private:
	int m_nRuleID;
	bool m_bEnabled;
	int m_nThreshold;
	int m_nExpiryDays;
	CString m_strDescription;
	int m_nPluCount;

private:
	CSQLFieldValidatorPurchaseControlRule vdtr;
};

//**********************************************************************

