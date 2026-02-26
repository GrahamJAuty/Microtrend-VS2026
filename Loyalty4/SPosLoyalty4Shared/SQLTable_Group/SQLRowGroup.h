#pragma once
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************

class CSQLRowGroup
{
public:
	CSQLRowGroup();
	
public:
	void Reset();
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	CString GetGroupName() { return m_strGroupName; }
	int GetSchemeNo() { return m_nSchemeNo; }
	bool GetStampOfferFlag() { return m_bStampOfferFlag; }
	/*****/
	int GetRefreshType() { return m_nRefreshType; }
	int GetRefreshFlags() { return m_nRefreshFlags; }
	double GetRefreshValue() { return m_dRefreshValue; }
	int GetRefreshAppend() { return m_nRefreshAppend; }
	/*****/
	int GetPurse1Allowed() { return m_nPurse1Allowed; }
	int GetPurse2Allowed() { return m_nPurse2Allowed; }
	/*****/
	bool GetNoPluBonusFlag() { return m_bNoPluBonus; }
	bool GetNoDeptBonusFlag() { return m_bNoDeptBonus; }
	/*****/
	int GetMaxSpendType() { return m_nMaxSpendType; }
	/*****/
	double GetMaxSpendPeriod1() { return m_dMaxSpendPeriod1; }
	double GetMaxSpendPeriod2() { return m_dMaxSpendPeriod2; }
	double GetMaxSpendPeriod3() { return m_dMaxSpendPeriod3; }
	double GetMaxSpendPeriod4() { return m_dMaxSpendPeriod4; }
	double GetMaxSpendPeriod5() { return m_dMaxSpendPeriod5; }
	double GetMaxSpendPeriod6() { return m_dMaxSpendPeriod6; }
	/*****/
	double GetMaxOverdraftPeriod1() { return m_dMaxOverdraftPeriod1; }
	double GetMaxOverdraftPeriod2() { return m_dMaxOverdraftPeriod2; }
	double GetMaxOverdraftPeriod3() { return m_dMaxOverdraftPeriod3; }
	double GetMaxOverdraftPeriod4() { return m_dMaxOverdraftPeriod4; }
	double GetMaxOverdraftPeriod5() { return m_dMaxOverdraftPeriod5; }
	double GetMaxOverdraftPeriod6() { return m_dMaxOverdraftPeriod6; }
	/*****/	
	bool GetRewardFlag() { return m_bRewardFlag; }
	bool GetRewardDisable() { return m_bRewardDisable; }
	int GetRewardType() { return m_nRewardType; }
	double GetRewardTrip() { return m_dRewardTrip; }
	int GetRewardPoints() { return m_nRewardPoints; }
	double GetRewardBonus() { return m_dRewardBonus; }
	double GetRewardLimit() { return m_dRewardLimit; }
	double GetRewardTrigger() { return m_dRewardTrigger; }
	/*****/
	bool GetRedeemFlag() { return m_bRedeemFlag; }
	bool GetRedeemDisable() { return m_bRedeemDisable; }
	int GetRedeemType() { return nREDEEM_STANDARD; }
	//int GetRedeemType() { return m_nRedeemType; }
	int GetRedeemTrip() { return m_nRedeemTrip; }
	double GetRedeemValue() { return m_dRedeemValue; }

public:
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetGroupName(CString str) { vdtr.SetGroupName(str, m_strGroupName); }
	void SetSchemeNo(int n) { vdtr.SetSchemeNo(n, m_nSchemeNo); }
	void SetStampOfferFlag(bool b) { m_bStampOfferFlag = b; }
	/*****/
	void SetRefreshType(int n) { vdtr.SetRefreshType(n, m_nRefreshType); }
	void SetRefreshFlags(int n) { vdtr.SetRefreshFlags(n, m_nRefreshFlags); }
	void SetRefreshValue(double d) { vdtr.SetRefreshValue(d, m_dRefreshValue); }
	void SetRefreshAppend(int n) { vdtr.SetRefreshAppend(n, m_nRefreshAppend); }
	/*****/
	void SetPurse1Allowed(int n) { vdtr.SetPurseAllowed(n, m_nPurse1Allowed); }
	void SetPurse2Allowed(int n) { vdtr.SetPurseAllowed(n, m_nPurse2Allowed); }
	/*****/
	void SetNoPluBonusFlag(bool b) { m_bNoPluBonus = b; }
	void SetNoDeptBonusFlag(bool b) { m_bNoDeptBonus = b; }
	/*****/
	void SetMaxSpendType(int n) { vdtr.SetMaxSpendType(n, m_nMaxSpendType); }
	/*****/
	void SetMaxSpendPeriod1(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPeriod1); }
	void SetMaxSpendPeriod2(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPeriod2); }
	void SetMaxSpendPeriod3(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPeriod3); }
	void SetMaxSpendPeriod4(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPeriod4); }
	void SetMaxSpendPeriod5(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPeriod5); }
	void SetMaxSpendPeriod6(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPeriod6); }
	/*****/
	void SetMaxOverdraftPeriod1(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPeriod1); }
	void SetMaxOverdraftPeriod2(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPeriod2); }
	void SetMaxOverdraftPeriod3(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPeriod3); }
	void SetMaxOverdraftPeriod4(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPeriod4); }
	void SetMaxOverdraftPeriod5(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPeriod5); }
	void SetMaxOverdraftPeriod6(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPeriod6); }
	/*****/
	void SetRewardFlag(bool b) { m_bRewardFlag = b; }
	void SetRewardDisable(bool b) { m_bRewardDisable = b; }
	void SetRewardType(int n) { vdtr.SetRewardType(n, m_nRewardType); }
	void SetRewardTrip(double d) { vdtr.SetRewardTrip(d, m_dRewardTrip); }
	void SetRewardPoints(int n) { vdtr.SetRewardPoints(n, m_nRewardPoints); }
	void SetRewardBonus(double d) { vdtr.SetRewardBonus(d, m_dRewardBonus); }
	void SetRewardLimit(double d) { vdtr.SetRewardLimit(d, m_dRewardLimit); }
	void SetRewardTrigger(double d) { vdtr.SetRewardTrigger(d, m_dRewardTrigger); }
	/*****/
	void SetRedeemFlag(bool b) { m_bRedeemFlag = b; }
	void SetRedeemDisable(bool b) { m_bRedeemDisable = b; }
	void SetRedeemType(int n) { vdtr.SetRedeemType(n, m_nRedeemType); }
	void SetRedeemTrip(int n) { vdtr.SetRedeemTrip(n, m_nRedeemTrip); }
	void SetRedeemValue(double d) { vdtr.SetRedeemValue(d, m_dRedeemValue); }

public:
	void ClearRefreshFlags() { SetRefreshFlags(0); }
	bool IsRefreshAllowed(int nDay);
	int GetRefreshWeeklyDayNo(int nDefaultDayNo = 0);
	void SetRefreshWeeklyDayNo(int nDayNo);
	void GetRefreshYearDate(int& nDayNo, int& nMonthNo);
	void SetRefreshYearDate(int nDayNo, int nMonthNo);
	bool ValidateYearlyRefreshFactor(int nFactor);
	void SetRefreshAllowed(int nDay, bool bSet);
	CString GetRefreshYearText();
	CString GetRefreshMonthText();
	CString GetRefreshWeekText();
	CString GetRefreshDaysText();
	CString GetRefreshTypeText();
	bool IsPurseAllowed(int nPurse, int nPeriod);
	void SetPurseAllowed(int nPurseNo, int nPeriodIndex, bool bSet);

//**********************************************************************
#ifdef LOYSERVER
//**********************************************************************
	bool IsPurseAllowedAtTime(int nPurseNo, CSSTime& time);
//**********************************************************************
#endif
//**********************************************************************

public:
	void Add(CSQLRowGroup& source);
	int Compare(CSQLRowGroup& source, int nHint);

private:
	int m_nGroupNo;
	CString m_strGroupName;
	int m_nSchemeNo;
	bool m_bStampOfferFlag;
	/*****/
	int m_nRefreshType;
	int m_nRefreshFlags;
	double m_dRefreshValue;
	int m_nRefreshAppend;
	/*****/
	int m_nPurse1Allowed;
	int m_nPurse2Allowed;
	/*****/
	bool m_bNoPluBonus;
	bool m_bNoDeptBonus;
	/*****/
	int m_nMaxSpendType;
	/*****/
	double m_dMaxSpendPeriod1;
	double m_dMaxSpendPeriod2;
	double m_dMaxSpendPeriod3;
	double m_dMaxSpendPeriod4;
	double m_dMaxSpendPeriod5;
	double m_dMaxSpendPeriod6;
	/*****/
	double m_dMaxOverdraftPeriod1;
	double m_dMaxOverdraftPeriod2;
	double m_dMaxOverdraftPeriod3;
	double m_dMaxOverdraftPeriod4;
	double m_dMaxOverdraftPeriod5;
	double m_dMaxOverdraftPeriod6;
	/*****/
	bool m_bRewardFlag;
	bool m_bRewardDisable;
	int m_nRewardType;
	double m_dRewardTrip;
	int m_nRewardPoints;
	double m_dRewardBonus;
	double m_dRewardLimit;
	double m_dRewardTrigger;
	/*****/
	bool m_bRedeemFlag;
	bool m_bRedeemDisable;
	int m_nRedeemType;
	int m_nRedeemTrip;
	double m_dRedeemValue;
	
private:
	CSQLFieldValidatorGroup vdtr;
};

//**********************************************************************

