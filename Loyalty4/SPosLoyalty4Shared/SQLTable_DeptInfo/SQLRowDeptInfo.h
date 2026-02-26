#pragma once
//**********************************************************************
#include "SQLFieldValidatorDeptInfo.h"
//**********************************************************************

class CSQLRowDeptInfo
{
public:
	CSQLRowDeptInfo();
	
private:
	void Reset();
	
public:
	int GetDbNo() { return m_nDbNo; }
	int GetDeptNo() { return m_nDeptNo; }
	CString GetName() { return m_strName; }
	double GetLoyaltyBonusTrip() { return m_dLoyaltyBonusTrip; }
	int GetLoyaltyBonusType() { return m_nLoyaltyBonusType; }
	int GetLoyaltyBonusPoints() { return m_nLoyaltyBonusPoints; }
	double GetLoyaltyBonusValue() { return m_dLoyaltyBonusValue; }
	int GetLoyaltyBonusPercent() { return m_nLoyaltyBonusPercent; }
	bool GetGeneralSpendBonusFlag() { return m_bGeneralSpendBonusFlag; }

public:
	void SetDbNo(int n) { vdtr.SetDbNo(n, m_nDbNo); }
	void SetDeptNo(int n) { vdtr.SetDeptNo(n, m_nDeptNo); }
	void SetName(CString str) { vdtr.SetName(str, m_strName); }
	void SetLoyaltyBonusTrip(double d) { vdtr.SetLoyaltyBonusTrip(d, m_dLoyaltyBonusTrip); }
	void SetLoyaltyBonusType(int n) { vdtr.SetLoyaltyBonusType(n, m_nLoyaltyBonusType); }
	void SetLoyaltyBonusPoints(int n) { vdtr.SetLoyaltyBonusPoints(n, m_nLoyaltyBonusPoints); }
	void SetLoyaltyBonusValue(double d) { vdtr.SetLoyaltyBonusValue(d, m_dLoyaltyBonusValue); }
	void SetLoyaltyBonusPercent(int n) { vdtr.SetLoyaltyBonusPercent(n, m_nLoyaltyBonusPercent); }
	void SetGeneralSpendBonusFlag(bool b) { m_bGeneralSpendBonusFlag = b; }
	
private:
	int m_nDbNo;
	int m_nDeptNo;
	CString m_strName;
	double m_dLoyaltyBonusTrip;
	int m_nLoyaltyBonusType;
	int m_nLoyaltyBonusPoints;
	double m_dLoyaltyBonusValue;
	int m_nLoyaltyBonusPercent;
	bool m_bGeneralSpendBonusFlag;

private:
	CSQLFieldValidatorDeptInfo vdtr;
};

//**********************************************************************

