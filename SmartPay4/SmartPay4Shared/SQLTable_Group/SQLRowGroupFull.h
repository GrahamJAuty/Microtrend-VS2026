#pragma once
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************
#define nREFRESHTYPE_DISABLED	0				// Purse2 disabled
#define nREFRESHTYPE_DAILY		1				// Daily Refresh
#define nREFRESHTYPE_WEEKLY		2	
#define nREFRESHTYPE_MONTHLY	3
#define nREFRESHTYPE_YEARLY		4
#define nREFRESHTYPE_MANUAL		5				// manual topup
#define nREFRESHTYPE_PERIOD		6				// daily period refresh
#define nREFRESHTYPE_EOD		7				// refresh after EOD
/**********************************************************************/
#define nOLD_GRPSPENDTYPE_DAILY			0			// Single Daily Spend
#define nOLD_GRPSPENDTYPE_RECORD		1			// Record Specific
#define nOLD_GRPSPENDTYPE_PERIOD		2			// Use Audit Periods
#define nGRPSPENDTYPE_DAILY_LOCKED		3			// Cannot override by account
#define nGRPSPENDTYPE_DAILY_RECORD1		4			// Override value by account
#define nGRPSPENDTYPE_DAILY_RECORD2		5			// Full override by account
#define nGRPSPENDTYPE_PERIOD_LOCKED		6			// Cannot override by account
#define nGRPSPENDTYPE_PERIOD_RECORD1	7			// Override value by account
#define nGRPSPENDTYPE_PERIOD_RECORD2	8			// Full override by account
/**********************************************************************/

class CSQLRowGroupFull
{
public:
	CSQLRowGroupFull();
	
public:
	void Reset();
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	CString GetGroupName() { return m_strGroupName; }
	int GetUserType() { return m_nUserType; }
	int GetSetNo() { return m_nSetNo; }
	CString GetShiftGroups() { return m_strShiftGroups; }
	int GetPriceLevel() { return m_nPriceLevel;  }
	int GetVatShiftFlag() { return m_nVatShiftFlag; }
	bool GetPurchaseControlFlag() { return m_bPurchaseControlFlag; }
	bool GetAllowRevalKioskFlag() { return m_bAllowRevalKioskFlag; }
	/*****/
	int GetRefreshType() { return m_nRefreshType; }
	int GetRefreshFlags() { return m_nRefreshFlags; }
	double GetRefreshValue() { return m_dRefreshValue; }
	int GetRefreshAppendFlag() { return m_nRefreshAppendFlag; }
	int GetRefreshTypePeriod1() { return m_nRefreshTypePeriod1; }
	int GetRefreshTypePeriod2() { return m_nRefreshTypePeriod2; }
	int GetRefreshTypePeriod3() { return m_nRefreshTypePeriod3; }
	int GetRefreshTypePeriod4() { return m_nRefreshTypePeriod4; }
	int GetRefreshTypePeriod5() { return m_nRefreshTypePeriod5; }
	int GetRefreshTypePeriod6() { return m_nRefreshTypePeriod6; }
	double GetRefreshValuePeriod1() { return m_dRefreshValuePeriod1; }
	double GetRefreshValuePeriod2() { return m_dRefreshValuePeriod2; }
	double GetRefreshValuePeriod3() { return m_dRefreshValuePeriod3; }
	double GetRefreshValuePeriod4() { return m_dRefreshValuePeriod4; }
	double GetRefreshValuePeriod5() { return m_dRefreshValuePeriod5; }
	double GetRefreshValuePeriod6() { return m_dRefreshValuePeriod6; }
	/*****/
	int GetPurse1Allowed() { return m_nPurse1Allowed; }
	int GetPurse2Allowed() { return m_nPurse2Allowed; }
	int GetPurse3Allowed() { return m_nPurse3Allowed; }
	/*****/
	int GetMaxSpendType() { return m_nMaxSpendType; }
	/*****/
	double GetMaxSpendPurse1Period1() { return m_dMaxSpendPurse1Period1; }
	double GetMaxSpendPurse1Period2() { return m_dMaxSpendPurse1Period2; }
	double GetMaxSpendPurse1Period3() { return m_dMaxSpendPurse1Period3; }
	double GetMaxSpendPurse1Period4() { return m_dMaxSpendPurse1Period4; }
	double GetMaxSpendPurse1Period5() { return m_dMaxSpendPurse1Period5; }
	double GetMaxSpendPurse1Period6() { return m_dMaxSpendPurse1Period6; }
	/*****/
	double GetMaxSpendPurse3Period1() { return m_dMaxSpendPurse3Period1; }
	double GetMaxSpendPurse3Period2() { return m_dMaxSpendPurse3Period2; }
	double GetMaxSpendPurse3Period3() { return m_dMaxSpendPurse3Period3; }
	double GetMaxSpendPurse3Period4() { return m_dMaxSpendPurse3Period4; }
	double GetMaxSpendPurse3Period5() { return m_dMaxSpendPurse3Period5; }
	double GetMaxSpendPurse3Period6() { return m_dMaxSpendPurse3Period6; }
	/*****/
	double GetMaxOverdraftPurse1Period1() { return m_dMaxOverdraftPurse1Period1; }
	double GetMaxOverdraftPurse1Period2() { return m_dMaxOverdraftPurse1Period2; }
	double GetMaxOverdraftPurse1Period3() { return m_dMaxOverdraftPurse1Period3; }
	double GetMaxOverdraftPurse1Period4() { return m_dMaxOverdraftPurse1Period4; }
	double GetMaxOverdraftPurse1Period5() { return m_dMaxOverdraftPurse1Period5; }
	double GetMaxOverdraftPurse1Period6() { return m_dMaxOverdraftPurse1Period6; }
	/*****/
	double GetMaxOverdraftPurse3Period1() { return m_dMaxOverdraftPurse3Period1; }
	double GetMaxOverdraftPurse3Period2() { return m_dMaxOverdraftPurse3Period2; }
	double GetMaxOverdraftPurse3Period3() { return m_dMaxOverdraftPurse3Period3; }
	double GetMaxOverdraftPurse3Period4() { return m_dMaxOverdraftPurse3Period4; }
	double GetMaxOverdraftPurse3Period5() { return m_dMaxOverdraftPurse3Period5; }
	double GetMaxOverdraftPurse3Period6() { return m_dMaxOverdraftPurse3Period6; }
	/*****/
	CString GetDeptInhibitPurse1() { return m_strDeptInhibitPurse1; }
	CString GetDeptInhibitPurse2() { return m_strDeptInhibitPurse2; }
	CString GetDeptInhibitPurse3() { return m_strDeptInhibitPurse3; }
	CString GetDeptInhibitCash() { return m_strDeptInhibitCash; }
	/*****/
	double GetLowBalancePurse1() { return m_dLowBalancePurse1; }
	double GetLowBalancePurse2() { return m_dLowBalancePurse2; }
	double GetLowBalancePurse3() { return m_dLowBalancePurse3; }
	/*****/
	int GetPointsAchievement() { return m_nPointsAchievement; }
	/*****/
	bool GetFileRevalueFlag() { return m_bFileRevalueFlag; }
	bool GetRevalueDisable() { return m_bRevalueDisable; }
	int GetRevalueType() { return m_nRevalueType; }
	double GetRevalueTrip1() { return m_dRevalueTrip1; }
	double GetRevalueTrip2() { return m_dRevalueTrip2; }
	int GetRevaluePoints() { return m_nRevaluePoints; }
	double GetRevalueBonus() { return m_dRevalueBonus; }
	/*****/
	bool GetFileRewardFlag() { return m_bFileRewardFlag; }
	bool GetRewardDisable() { return m_bRewardDisable; }
	int GetRewardType() { return m_nRewardType; }
	double GetRewardTrip() { return m_dRewardTrip; }
	int GetRewardPoints() { return m_nRewardPoints; }
	double GetRewardBonus() { return m_dRewardBonus; }
	double GetRewardLimit() { return m_dRewardLimit; }
	double GetRewardTrigger() { return m_dRewardTrigger; }
	/*****/
	bool GetFileRedeemFlag() { return m_bFileRedeemFlag; }
	bool GetRedeemDisable() { return m_bRedeemDisable; }
	int GetRedeemTrip() { return m_nRedeemTrip; }
	double GetRedeemValue() { return m_dRedeemValue; }
	int GetRedeemPurse() { return m_nRedeemPurse; }

	//ACTIVE FUNCTIONS RETURN LOCKED VALUES FOR PEBBLE MODE
	bool GetActiveRevalueFlag();
	bool GetActiveRewardFlag();
	bool GetActiveRedeemFlag();

public:
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetGroupName(CString str) { vdtr.SetGroupName(str, m_strGroupName); }
	void SetUserType(int n) { vdtr.SetUserType(n, m_nUserType); }
	void SetSetNo(int n) { vdtr.SetSetNo(n, m_nSetNo); }
	void SetShiftGroups(CString str) { vdtr.SetShiftGroups(str, m_strShiftGroups); }
	void SetPriceLevel(int n) { vdtr.SetPriceLevel(n, m_nPriceLevel); }
	void SetVatShiftFlag(int n) { m_nVatShiftFlag = n; }
	void SetPurchaseControlFlag(bool b) { m_bPurchaseControlFlag = b; }
	void SetAllowRevalKioskFlag(bool b) { m_bAllowRevalKioskFlag = b; }
	/*****/
	void SetRefreshType(int n) { vdtr.SetRefreshType(n, m_nRefreshType); }
	void SetRefreshFlags(int n) { vdtr.SetRefreshFlags(n, m_nRefreshFlags); }
	void SetRefreshValue(double d) { vdtr.SetRefreshValue(d, m_dRefreshValue); }
	void SetRefreshAppendFlag(int n) { m_nRefreshAppendFlag = n; }
	void SetRefreshTypePeriod1(int n) { vdtr.SetRefreshTypePeriod(n, m_nRefreshTypePeriod1); }
	void SetRefreshTypePeriod2(int n) { vdtr.SetRefreshTypePeriod(n, m_nRefreshTypePeriod2); }
	void SetRefreshTypePeriod3(int n) { vdtr.SetRefreshTypePeriod(n, m_nRefreshTypePeriod3); }
	void SetRefreshTypePeriod4(int n) { vdtr.SetRefreshTypePeriod(n, m_nRefreshTypePeriod4); }
	void SetRefreshTypePeriod5(int n) { vdtr.SetRefreshTypePeriod(n, m_nRefreshTypePeriod5); }
	void SetRefreshTypePeriod6(int n) { vdtr.SetRefreshTypePeriod(n, m_nRefreshTypePeriod6); }
	void SetRefreshValuePeriod1(double d) { vdtr.SetRefreshValuePeriod(d, m_dRefreshValuePeriod1); }
	void SetRefreshValuePeriod2(double d) { vdtr.SetRefreshValuePeriod(d, m_dRefreshValuePeriod2); }
	void SetRefreshValuePeriod3(double d) { vdtr.SetRefreshValuePeriod(d, m_dRefreshValuePeriod3); }
	void SetRefreshValuePeriod4(double d) { vdtr.SetRefreshValuePeriod(d, m_dRefreshValuePeriod4); }
	void SetRefreshValuePeriod5(double d) { vdtr.SetRefreshValuePeriod(d, m_dRefreshValuePeriod5); }
	void SetRefreshValuePeriod6(double d) { vdtr.SetRefreshValuePeriod(d, m_dRefreshValuePeriod6); }
	/*****/
	void SetPurse1Allowed(int n) { vdtr.SetPurseAllowed(n, m_nPurse1Allowed); }
	void SetPurse2Allowed(int n) { vdtr.SetPurseAllowed(n, m_nPurse2Allowed); }
	void SetPurse3Allowed(int n) { vdtr.SetPurseAllowed(n, m_nPurse3Allowed); }
	/*****/
	void SetMaxSpendType(int n) { vdtr.SetMaxSpendType(n, m_nMaxSpendType); }
	/*****/
	void SetMaxSpendPurse1Period1(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse1Period1); }
	void SetMaxSpendPurse1Period2(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse1Period2); }
	void SetMaxSpendPurse1Period3(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse1Period3); }
	void SetMaxSpendPurse1Period4(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse1Period4); }
	void SetMaxSpendPurse1Period5(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse1Period5); }
	void SetMaxSpendPurse1Period6(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse1Period6); }
	/*****/
	void SetMaxSpendPurse3Period1(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse3Period1); }
	void SetMaxSpendPurse3Period2(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse3Period2); }
	void SetMaxSpendPurse3Period3(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse3Period3); }
	void SetMaxSpendPurse3Period4(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse3Period4); }
	void SetMaxSpendPurse3Period5(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse3Period5); }
	void SetMaxSpendPurse3Period6(double d) { vdtr.SetMaxSpend(d, m_dMaxSpendPurse3Period6); }
	/*****/
	void SetMaxOverdraftPurse1Period1(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse1Period1); }
	void SetMaxOverdraftPurse1Period2(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse1Period2); }
	void SetMaxOverdraftPurse1Period3(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse1Period3); }
	void SetMaxOverdraftPurse1Period4(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse1Period4); }
	void SetMaxOverdraftPurse1Period5(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse1Period5); }
	void SetMaxOverdraftPurse1Period6(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse1Period6); }
	/*****/
	void SetMaxOverdraftPurse3Period1(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse3Period1); }
	void SetMaxOverdraftPurse3Period2(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse3Period2); }
	void SetMaxOverdraftPurse3Period3(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse3Period3); }
	void SetMaxOverdraftPurse3Period4(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse3Period4); }
	void SetMaxOverdraftPurse3Period5(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse3Period5); }
	void SetMaxOverdraftPurse3Period6(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraftPurse3Period6); }
	/*****/
	void SetDeptInhibitPurse1(CString str) { vdtr.SetDeptInhibit(str, m_strDeptInhibitPurse1); }
	void SetDeptInhibitPurse2(CString str) { vdtr.SetDeptInhibit(str, m_strDeptInhibitPurse2); }
	void SetDeptInhibitPurse3(CString str) { vdtr.SetDeptInhibit(str, m_strDeptInhibitPurse3); }
	void SetDeptInhibitCash(CString str) { vdtr.SetDeptInhibit(str, m_strDeptInhibitCash); }
	/*****/
	void SetLowBalancePurse1(double d) { vdtr.SetLowBalance(d, m_dLowBalancePurse1); }
	void SetLowBalancePurse2(double d) { vdtr.SetLowBalance(d, m_dLowBalancePurse2); }
	void SetLowBalancePurse3(double d) { vdtr.SetLowBalance(d, m_dLowBalancePurse3); }
	/*****/
	void SetPointsAchievement(int n) { vdtr.SetPointsAchievement(n, m_nPointsAchievement); }
	/*****/
	void SetFileRevalueFlag(bool b) { m_bFileRevalueFlag = b; }
	void SetRevalueDisable(bool b) { m_bRevalueDisable = b; }
	void SetRevalueType(int n) { vdtr.SetRevalueType(n, m_nRevalueType); }
	void SetRevalueTrip1(double d) { vdtr.SetRevalueTrip(d, m_dRevalueTrip1); }
	void SetRevalueTrip2(double d) { vdtr.SetRevalueTrip(d, m_dRevalueTrip2); }
	void SetRevaluePoints(int n) { vdtr.SetRevaluePoints(n, m_nRevaluePoints); }
	void SetRevalueBonus(double d) { vdtr.SetRevalueBonus(d, m_dRevalueBonus); }
	/*****/
	void SetFileRewardFlag(bool b) { m_bFileRewardFlag = b; }
	void SetRewardDisable(bool b) { m_bRewardDisable = b; }
	void SetRewardType(int n) { vdtr.SetRewardType(n, m_nRewardType); }
	void SetRewardTrip(double d) { vdtr.SetRewardTrip(d, m_dRewardTrip); }
	void SetRewardPoints(int n) { vdtr.SetRewardPoints(n, m_nRewardPoints); }
	void SetRewardBonus(double d) { vdtr.SetRewardBonus(d, m_dRewardBonus); }
	void SetRewardLimit(double d) { vdtr.SetRewardLimit(d, m_dRewardLimit); }
	void SetRewardTrigger(double d) { vdtr.SetRewardTrigger(d, m_dRewardTrigger); }
	/*****/
	void SetFileRedeemFlag(bool b) { m_bFileRedeemFlag = b; }
	void SetRedeemDisable(bool b) { m_bRedeemDisable = b; }
	void SetRedeemTrip(int n) { vdtr.SetRedeemTrip(n, m_nRedeemTrip); }
	void SetRedeemValue(double d) { vdtr.SetRedeemValue(d, m_dRedeemValue); }
	void SetRedeemPurse(int n) { vdtr.SetRedeemPurse(n, m_nRedeemPurse); }

public:
	void SetNewRecord();
	bool HavePeriodRefresh();
	void SetRefreshAllowed(int nDay, bool bSet);
	CString GetRefreshDaysText();
	CString GetRefreshWeekText();
	int GetRefreshWeeklyDayNo(int nDefaultDayNo = 0);
	void SetRefreshWeeklyDayNo(int nDayNo);
	CString GetRefreshMonthText();
	void GetRefreshYearDate(int& nDayNo, int& nMonthNo);
	void SetRefreshYearDate(int nDayNo, int nMonthNo);
	CString GetRefreshYearText();
	bool IsRefreshAllowed(int nDay);
	void ClearRefreshFlags() { m_nRefreshFlags = 0; }
	CString GetRefreshTypeText();
	double GetRefreshValueByPeriod(int nIndex);
	void SetRefreshValueByPeriod(int nIndex, double dValue);
	int GetRefreshTypeByPeriod(int nIndex);
	void SetRefreshTypeByPeriod(int nIndex, int nType);
	bool IsPurse2Refreshable();
	bool ValidateDeptInhibitList(const char* szText);
	bool IsPurseAllowed(int nPurseNo, int nPeriodIndex);
	void SetPurseAllowed(int nPurseNo, int nPeriodIndex, bool bSet);
	CString GetListboxText();
	CString GetRefreshPeriodTypeText(int nIndex);
	void ValidateShiftGroups(CString strIn, CString& strOut, CString& strError);

#ifndef SYSTEMTYPE_SERVER
	void DisplayDeptListError();
#endif

private:
	int m_nGroupNo;
	CString m_strGroupName;
	int m_nUserType;
	int m_nSetNo;
	CString m_strShiftGroups;
	int m_nPriceLevel;
	int m_nVatShiftFlag;
	bool m_bPurchaseControlFlag;
	bool m_bAllowRevalKioskFlag;
	/*****/
	int m_nRefreshType;
	int m_nRefreshFlags;
	double m_dRefreshValue;
	int m_nRefreshAppendFlag;
	int m_nRefreshTypePeriod1;
	int m_nRefreshTypePeriod2;
	int m_nRefreshTypePeriod3;
	int m_nRefreshTypePeriod4;
	int m_nRefreshTypePeriod5;
	int m_nRefreshTypePeriod6;
	double m_dRefreshValuePeriod1;
	double m_dRefreshValuePeriod2;
	double m_dRefreshValuePeriod3;
	double m_dRefreshValuePeriod4;
	double m_dRefreshValuePeriod5;
	double m_dRefreshValuePeriod6;
	/*****/
	int m_nPurse1Allowed;
	int m_nPurse2Allowed;
	int m_nPurse3Allowed;
	/*****/
	int m_nMaxSpendType;
	/*****/
	double m_dMaxSpendPurse1Period1;
	double m_dMaxSpendPurse1Period2;
	double m_dMaxSpendPurse1Period3;
	double m_dMaxSpendPurse1Period4;
	double m_dMaxSpendPurse1Period5;
	double m_dMaxSpendPurse1Period6;
	/*****/
	double m_dMaxSpendPurse3Period1;
	double m_dMaxSpendPurse3Period2;
	double m_dMaxSpendPurse3Period3;
	double m_dMaxSpendPurse3Period4;
	double m_dMaxSpendPurse3Period5;
	double m_dMaxSpendPurse3Period6;
	/*****/
	double m_dMaxOverdraftPurse1Period1;
	double m_dMaxOverdraftPurse1Period2;
	double m_dMaxOverdraftPurse1Period3;
	double m_dMaxOverdraftPurse1Period4;
	double m_dMaxOverdraftPurse1Period5;
	double m_dMaxOverdraftPurse1Period6;
	/*****/
	double m_dMaxOverdraftPurse3Period1;
	double m_dMaxOverdraftPurse3Period2;
	double m_dMaxOverdraftPurse3Period3;
	double m_dMaxOverdraftPurse3Period4;
	double m_dMaxOverdraftPurse3Period5;
	double m_dMaxOverdraftPurse3Period6;
	/*****/
	CString m_strDeptInhibitPurse1;
	CString m_strDeptInhibitPurse2;
	CString m_strDeptInhibitPurse3;
	CString m_strDeptInhibitCash;
	/*****/
	double m_dLowBalancePurse1;
	double m_dLowBalancePurse2;
	double m_dLowBalancePurse3;
	/*****/
	int m_nPointsAchievement;
	/*****/
	bool m_bFileRevalueFlag;
	bool m_bRevalueDisable;
	int m_nRevalueType;
	double m_dRevalueTrip1;
	double m_dRevalueTrip2;
	int m_nRevaluePoints;
	double m_dRevalueBonus;
	/*****/
	bool m_bFileRewardFlag;
	bool m_bRewardDisable;
	int m_nRewardType;
	double m_dRewardTrip;
	int m_nRewardPoints;
	double m_dRewardBonus;
	double m_dRewardLimit;
	double m_dRewardTrigger;
	/*****/
	bool m_bFileRedeemFlag;
	bool m_bRedeemDisable;
	int m_nRedeemTrip;
	double m_dRedeemValue;
	int m_nRedeemPurse;

private:
	CSQLFieldValidatorGroup vdtr;
};

//**********************************************************************

