#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
#include "SQLRowAccountFind.h"
#include "SQLRowAccountList.h"
//**********************************************************************
#define GROUPSHIFT_STATUS_VALID 0
#define GROUPSHIFT_STATUS_INVALID 1
#define GROUPSHIFT_STATUS_EXPIRED 2
//**********************************************************************

class CSQLRowAccountFull
{
public:
	CSQLRowAccountFull();
	CSQLRowAccountFull(const CSQLRowAccountFull& source);
	CSQLRowAccountFull& operator= (const CSQLRowAccountFull& source);
	
private:
	void Reset();
	
public:
	int GetLeaverYear() const { return m_nLeaverYear; }
	__int64 GetUserID() const { return m_nUserID; }
	CString GetUserIDString(); 
	CString GetUsername();
	CString GetForename() const { return m_strForename; }
	CString GetSurname() const { return m_strSurname; }
	CString GetDOB() const { return m_strDOB; }
	CString GetExpiryDate() const { return m_strExpiryDate; }
	CString GetGender() const { return m_strGender; }
	int GetGroupNo() const { return m_nGroupNo; }
	CString GetDayGroups() const { return m_strDayGroups; }
	
	int GetGroupShiftStatus() const { return m_nGroupShiftStatus; }
	CString GetGroupShiftStartDate() const { return m_strGroupShiftStartDate; }
	CString GetGroupShiftEndDate() const { return m_strGroupShiftEndDate; }
	int GetGroupShiftFutureGroup() const { return m_nGroupShiftFutureGroup; }
	
	CString GetEmail() const { return m_strEmail; }
	CString GetInfo1() const { return m_strInfo1; }
	CString GetInfo2() const { return m_strInfo2; }
	CString GetInfo3() const { return m_strInfo3; }
	CString GetInfo4() const { return m_strInfo4; }
	CString GetInfo5() const { return m_strInfo5; }
	CString GetInfo6() const { return m_strInfo6; }
	CString GetInfo7() const { return m_strInfo7; }
	CString GetInfo8() const { return m_strInfo8; }
	bool GetTick1() const { return m_bTick1; }
	bool GetTick2() const { return m_bTick2; }
	bool GetTick3() const { return m_bTick3; }
	bool GetTick4() const { return m_bTick4; }
	int GetAllergyCodes() const { return m_nAllergyCodes;  }
	CString GetDietary() const { return m_strDietary; }
	bool GetInactive() const { return m_bInactive; }
	/*****/
	CString GetMemberID() const { return m_strMemberID; }
	CString GetExternalRef1() const { return m_strExternalRef1; }
	CString GetExternalRef2() const { return m_strExternalRef2; }
	__int64 GetPreviousUserID() const { return m_nPreviousUserID; }
	__int64 GetNextUserID() const { return m_nNextUserID; }
	CString GetLegacyPebblePurseID() const { return m_strLegacyPebblePurseID; }
	/*****/
	int GetPurse1Allow() const { return m_nPurse1Allow;  }
	double GetPurse1Liability() const { return m_dPurse1Liability; }
	double GetPurse1Credit() const { return m_dPurse1Credit; }
	CString GetPurse1LastSpendDate() const { return m_strPurse1LastSpendDate; }
	CString GetPurse1LastSpendTime() const { return m_strPurse1LastSpendTime; }
	double GetPurse1LastSpend() const { return m_dPurse1LastSpend; }
	double GetPurse1SpendToDate() const { return m_dPurse1SpendToDate; }
	/*****/
	int GetPurse2Allow() const { return m_nPurse2Allow; }
	double GetPurse2Balance() const { return m_dPurse2Balance; }
	CString GetPurse2LastSpendDate() const { return m_strPurse2LastSpendDate; }
	CString GetPurse2LastSpendTime() const { return m_strPurse2LastSpendTime; }
	double GetPurse2LastSpend() const { return m_dPurse2LastSpend; }
	double GetPurse2SpendToDate() const { return m_dPurse2SpendToDate; }
	int GetPurse2RefreshFlags() const { return m_nPurse2RefreshFlags; }
	CString GetPurse2RefreshedDate() const { return m_strPurse2RefreshedDate; }
	CString GetPurse2RefreshedTime() const { return m_strPurse2RefreshedTime; }
	/*****/
	int GetPurse3Allow() const { return m_nPurse3Allow; }
	double GetPurse3Liability() const { return m_dPurse3Liability; }
	double GetPurse3Credit() const { return m_dPurse3Credit; }
	CString GetPurse3LastSpendDate() const { return m_strPurse3LastSpendDate; }
	CString GetPurse3LastSpendTime() const { return m_strPurse3LastSpendTime; }
	double GetPurse3LastSpend() const { return m_dPurse3LastSpend; }
	double GetPurse3SpendToDate() const { return m_dPurse3SpendToDate; }
	/*****/
	double GetCashSpendToDate() const { return m_dCashSpendToDate; }
	/*****/
	int GetPoints() const { return m_nPoints; }
	int GetPointsToDate() const { return m_nPointsToDate; }
	/*****/
	bool GetOwnMaxSpend() const { return m_bOwnMaxSpend; }
	/*****/
	double GetMaxSpendPurse1Period1() const { return m_dMaxSpendPurse1Period1; }
	double GetMaxSpendPurse1Period2() const { return m_dMaxSpendPurse1Period2; }
	double GetMaxSpendPurse1Period3() const { return m_dMaxSpendPurse1Period3; }
	double GetMaxSpendPurse1Period4() const { return m_dMaxSpendPurse1Period4; }
	double GetMaxSpendPurse1Period5() const { return m_dMaxSpendPurse1Period5; }
	double GetMaxSpendPurse1Period6() const { return m_dMaxSpendPurse1Period6; }
	/*****/
	double GetMaxSpendPurse3Period1() const { return m_dMaxSpendPurse3Period1; }
	double GetMaxSpendPurse3Period2() const { return m_dMaxSpendPurse3Period2; }
	double GetMaxSpendPurse3Period3() const { return m_dMaxSpendPurse3Period3; }
	double GetMaxSpendPurse3Period4() const { return m_dMaxSpendPurse3Period4; }
	double GetMaxSpendPurse3Period5() const { return m_dMaxSpendPurse3Period5; }
	double GetMaxSpendPurse3Period6() const { return m_dMaxSpendPurse3Period6; }
	/*****/
	double GetMaxOverdraftPurse1Period1() const { return m_dMaxOverdraftPurse1Period1; }
	double GetMaxOverdraftPurse1Period2() const { return m_dMaxOverdraftPurse1Period2; }
	double GetMaxOverdraftPurse1Period3() const { return m_dMaxOverdraftPurse1Period3; }
	double GetMaxOverdraftPurse1Period4() const { return m_dMaxOverdraftPurse1Period4; }
	double GetMaxOverdraftPurse1Period5() const { return m_dMaxOverdraftPurse1Period5; }
	double GetMaxOverdraftPurse1Period6() const { return m_dMaxOverdraftPurse1Period6; }
	/*****/
	double GetMaxOverdraftPurse3Period1() const { return m_dMaxOverdraftPurse3Period1; }
	double GetMaxOverdraftPurse3Period2() const { return m_dMaxOverdraftPurse3Period2; }
	double GetMaxOverdraftPurse3Period3() const { return m_dMaxOverdraftPurse3Period3; }
	double GetMaxOverdraftPurse3Period4() const { return m_dMaxOverdraftPurse3Period4; }
	double GetMaxOverdraftPurse3Period5() const { return m_dMaxOverdraftPurse3Period5; }
	double GetMaxOverdraftPurse3Period6() const { return m_dMaxOverdraftPurse3Period6; }
	/*****/
	int GetAlertCode() const { return m_nAlertCode; }
	CString GetAlertDate() const { return m_strAlertDate; }
	CString GetAlertTime() const { return m_strAlertTime; }
	
public:
	void SetLeaverYear( int n ) { vdtr.SetLeaverYear(n, m_nLeaverYear); }
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUserIDString(CString str) { vdtr.SetUserID(_atoi64(str), m_nUserID); }
	void SetUsername(CString str) { vdtr.SetUsername(str, m_strUsername); }
	void SetForename(CString str) { vdtr.SetForename(str, m_strForename); }
	void SetSurname(CString str) { vdtr.SetSurname(str, m_strSurname); }
	void SetDOB(CString str) { vdtr.SetDOB(str, m_strDOB); }
	void SetExpiryDate(CString str) { vdtr.SetExpiryDate(str, m_strExpiryDate); }
	void SetGender(CString str);
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetDayGroups(CString str) { vdtr.SetDayGroups(str, m_strDayGroups); }
	void SetGroupShiftStatus(int n) { vdtr.SetGroupShiftStatus(n, m_nGroupShiftStatus); }
	void SetGroupShiftStartDate(CString str) { vdtr.SetGroupShiftStartDate(str, m_strGroupShiftStartDate); }
	void SetGroupShiftEndDate(CString str) { vdtr.SetGroupShiftEndDate(str, m_strGroupShiftEndDate); }
	void SetGroupShiftFutureGroup(int n) { vdtr.SetGroupShiftFutureGroup(n, m_nGroupShiftFutureGroup); }
	void SetEmail(CString str) { vdtr.SetEmail(str, m_strEmail); }
	void SetInfo1(CString str) { vdtr.SetInfo1(str, m_strInfo1); }
	void SetInfo2(CString str) { vdtr.SetInfo2(str, m_strInfo2); }
	void SetInfo3(CString str) { vdtr.SetInfo3(str, m_strInfo3); }
	void SetInfo4(CString str) { vdtr.SetInfo4(str, m_strInfo4); }
	void SetInfo5(CString str) { vdtr.SetInfo5(str, m_strInfo5); }
	void SetInfo6(CString str) { vdtr.SetInfo6(str, m_strInfo6); }
	void SetInfo7(CString str) { vdtr.SetInfo7(str, m_strInfo7); }
	void SetInfo8(CString str) { vdtr.SetInfo8(str, m_strInfo8); }
	void SetTick1(bool b) { m_bTick1 = b; }
	void SetTick2(bool b) { m_bTick2 = b; }
	void SetTick3(bool b) { m_bTick3 = b; }
	void SetTick4(bool b) { m_bTick4 = b; }
	void SetAllergyCodes(int n) { vdtr.SetAllergyCodes(n, m_nAllergyCodes); }
	void SetDietary(CString str) { vdtr.SetDietary(str, m_strDietary); }
	void SetInactive(bool b) { m_bInactive = b;  }
	/*****/
	void SetMemberID(CString str) { vdtr.SetMemberID(str, m_strMemberID); }
	void SetExternalRef1(CString str) { vdtr.SetExternalRef1(str, m_strExternalRef1); }
	void SetExternalRef2(CString str) { vdtr.SetExternalRef2(str, m_strExternalRef2); }
	void SetPreviousUserID(__int64 n) { vdtr.SetPreviousUserID(n, m_nPreviousUserID); }
	void SetNextUserID(__int64 n) { vdtr.SetNextUserID(n, m_nNextUserID); }
	void SetLegacyPebblePurseID(CString str) { vdtr.SetLegacyPebblePurseID(str, m_strLegacyPebblePurseID); }
	/*****/
	void SetPurse1Allow(int n) { vdtr.SetPurse1Allow(n, m_nPurse1Allow); }
	void SetPurse1Liability(double d) { vdtr.SetPurse1Liability(d, m_dPurse1Liability); }
	void SetPurse1Credit(double d) { vdtr.SetPurse1Credit(d, m_dPurse1Credit); }
	void SetPurse1LastSpendDate(CString str) { vdtr.SetPurse1LastSpendDate(str, m_strPurse1LastSpendDate); }
	void SetPurse1LastSpendTime(CString str) { vdtr.SetPurse1LastSpendTime(str, m_strPurse1LastSpendTime); }
	void SetPurse1LastSpend(double d) { vdtr.SetPurse1LastSpend(d, m_dPurse1LastSpend); }
	void SetPurse1SpendToDate(double d) { vdtr.SetPurse1SpendToDate(d, m_dPurse1SpendToDate); }
	/*****/
	void SetPurse2Allow(int n) { vdtr.SetPurse2Allow(n, m_nPurse2Allow); }
	void SetPurse2Balance(double d) { vdtr.SetPurse2Balance(d, m_dPurse2Balance); }
	void SetPurse2LastSpendDate(CString str) { vdtr.SetPurse2LastSpendDate(str, m_strPurse2LastSpendDate); }
	void SetPurse2LastSpendTime(CString str) { vdtr.SetPurse2LastSpendTime(str, m_strPurse2LastSpendTime); }
	void SetPurse2LastSpend(double d) { vdtr.SetPurse2LastSpend(d, m_dPurse2LastSpend); }
	void SetPurse2SpendToDate(double d) { vdtr.SetPurse2SpendToDate(d, m_dPurse2SpendToDate); }
	void SetPurse2RefreshFlags(int n) { vdtr.SetPurse2RefreshFlags(n, m_nPurse2RefreshFlags); }
	void SetPurse2RefreshedDate(CString str) { vdtr.SetPurse2RefreshedDate(str, m_strPurse2RefreshedDate); }
	void SetPurse2RefreshedTime(CString str) { vdtr.SetPurse2RefreshedTime(str, m_strPurse2RefreshedTime); }
	/*****/
	void SetPurse3Allow(int n) { vdtr.SetPurse3Allow(n, m_nPurse3Allow); }
	void SetPurse3Liability(double d) { vdtr.SetPurse3Liability(d, m_dPurse3Liability); }
	void SetPurse3Credit(double d) { vdtr.SetPurse3Credit(d, m_dPurse3Credit); }
	void SetPurse3LastSpendDate(CString str) { vdtr.SetPurse3LastSpendDate(str, m_strPurse3LastSpendDate); }
	void SetPurse3LastSpendTime(CString str) { vdtr.SetPurse3LastSpendTime(str, m_strPurse3LastSpendTime); }
	void SetPurse3LastSpend(double d) { vdtr.SetPurse3LastSpend(d, m_dPurse3LastSpend); }
	void SetPurse3SpendToDate(double d) { vdtr.SetPurse3SpendToDate(d, m_dPurse3SpendToDate); }
	/*****/
	void SetCashSpendToDate(double d) { vdtr.SetCashSpendToDate(d, m_dCashSpendToDate); }
	/*****/
	void SetPoints(int n) { vdtr.SetPoints(n, m_nPoints); }
	void SetPointsToDate(int n) { vdtr.SetPointsToDate(n, m_nPointsToDate); }
	/*****/
	void SetOwnMaxSpend(bool b) { m_bOwnMaxSpend = b; }
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
	void SetAlertCode(int n) { vdtr.SetAlertCode(n, m_nAlertCode); }
	void SetAlertDate(CString str) { vdtr.SetAlertDate(str, m_strAlertDate); }
	void SetAlertTime(CString str) { vdtr.SetAlertDate(str, m_strAlertTime); }
	/*****/
	void SetRowVersion(CByteArray& arraySource);

public:
	//HELPER FUNCTIONS
	bool HasUserExpired();
	bool IsAlertSet() { return (GetAlertCode() != 0); }
	double GetPurse1Balance() { return GetPurse1Credit() + GetPurse1Liability(); }
	double GetPurse3Balance() { return GetPurse3Credit() + GetPurse3Liability(); }
	bool IsRefreshSet() { return (GetPurse2RefreshFlags() != 0); }
	bool IsRefreshAllowed(int nDay);
	bool IsPurseAllowed(int nPurseNo, int nPeriodIndex);
	void SetPurseAllowed(int nPurseNo, int nPeriodIndex, bool bSet);
	bool GetLastUsed(CSSDate& date, CSSTime& time);
	CString GetGenderText();
	bool IsGenderMale() { return (GetGender() == "M"); }
	bool IsGenderFemale() { return (GetGender() == "F"); }
	bool IsGenderUnknown() { return (GetGender() == ""); }
	int GetDailyGroupNo(int nDayIndex);
	void SetDailyGroupNo(int nDayIndex, int nGroupNo);
	void ClearRefreshFlags() { m_nPurse2RefreshFlags = 0;  }
	void SetRefreshAllowed(int nDay, bool bSet);
	void SetUserAlert(int nAlertCode);
	int GetRecordStatusColour();
	CString GetGroupNoString();
	CString GetPurseBalanceString(int nPurseNo);
	CString GetTotalBalanceString();
	CString GetPointsString();
	bool HavePreviousUserID() { return (GetPreviousUserID() != 0); }
	bool HaveNextUserID() { return (GetNextUserID() != 0); }
	CString GetAccountDisplayName();
	double GetTotalSpendToDate() { return (GetPurse1SpendToDate() + GetPurse2SpendToDate() + GetPurse3SpendToDate() + GetCashSpendToDate()); }
	CString GetAgeNextBirthday();
	int GetAge(bool bNextBDay);
	CString GetDOBDayOfWeek();
	CString GetRefreshTextDays();
	bool HaveAllergies() { return (GetAllergyCodes() == 0) ? FALSE : TRUE; }
	CString GetInfo(int n);
	void SetInfo(int n, const char* szText);
	void AddToPurse1Credit(double dValue) { SetPurse1Credit(GetPurse1Credit() + dValue); }
	void SubtractFromPurse1Credit(double dValue) { SetPurse1Credit(GetPurse1Credit() - dValue); }
	void AddToPurse1Liability(double dValue) { SetPurse1Liability(GetPurse1Liability() + dValue); }
	void SubtractFromPurse1Liability(double dValue) { SetPurse1Liability(GetPurse1Liability() - dValue); }
	void AllocatePurse1Spend(double dValue, double& dLiability, double& dCredit);
	void RevaluePurse2(double dValue, const char* szDate = "", const char* szTime = "");
	void AddToPurse2LastSpend(double dValue) { SetPurse2LastSpend(GetPurse2LastSpend() + dValue); }
	void AddToPurse2SpendToDate(double dValue) { SetPurse2SpendToDate(GetPurse2SpendToDate() + dValue); }
	void SubtractFromPurse2Balance(double dValue) { SetPurse2Balance(GetPurse2Balance() - dValue); }
	void AddToCashSpendToDate(double dValue) { SetCashSpendToDate(GetCashSpendToDate() + dValue); }
	void AddToPurse1LastSpend(double dValue) { SetPurse1LastSpend(GetPurse1LastSpend() + dValue); }
	void AddToPurse1SpendToDate(double dValue) { SetPurse1SpendToDate(GetPurse1SpendToDate() + dValue); }
	void AddToPurse2Balance(double dValue) { SetPurse2Balance(GetPurse2Balance() + dValue); }
	void AddToPurse3Liability(double dValue) { SetPurse3Liability(GetPurse3Liability() + dValue); }
	void SubtractFromPurse3Liability(double dValue) { SetPurse3Liability(GetPurse3Liability() - dValue); }
	void AddToPurse3Credit(double dValue) { SetPurse3Credit(GetPurse3Credit() + dValue); }
	void SubtractFromPurse3Credit(double dValue) { SetPurse3Credit(GetPurse3Credit() - dValue); }
	void AddToPurse3LastSpend(double dValue) { SetPurse3LastSpend(GetPurse3LastSpend() + dValue); }
	void AddToPurse3SpendToDate(double dValue) { SetPurse3SpendToDate(GetPurse3SpendToDate() + dValue); }
	void AllocatePurse3Spend(double dValue, double& dLiability, double& dCredit);
	void UpdatePoints(int nPoints);
	void AddToPoints(int nPoints) { SetPoints(GetPoints() + nPoints); }
	void AddToPointsToDate(int nPoints) { SetPointsToDate(GetPointsToDate() + nPoints); }
	void CopyGroupMaxSpendSettings();
	void SetRefreshRef(const char* szDaysText);
	void SetDailyGroups(const char* szText);
	bool GetTick(int n);
	bool IsStringEmpty(CString strText);
	bool IsRecordEmpty();
	bool CompareRowVersion(CSQLRowAccountFull& RowSource);

private:
	int m_nLeaverYear;
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strForename;
	CString m_strSurname;
	CString m_strDOB;
	CString m_strExpiryDate;
	CString m_strGender;
	int m_nGroupNo;
	CString m_strDayGroups;
	int m_nGroupShiftStatus;
	CString m_strGroupShiftStartDate;
	CString m_strGroupShiftEndDate;
	int m_nGroupShiftFutureGroup;
	CString m_strEmail;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strInfo5;
	CString m_strInfo6;
	CString m_strInfo7;
	CString m_strInfo8;
	bool m_bTick1;
	bool m_bTick2;
	bool m_bTick3;
	bool m_bTick4;
	int m_nAllergyCodes;
	CString m_strDietary;
	bool m_bInactive;
	/*****/
	CString m_strMemberID;
	CString m_strExternalRef1;
	CString m_strExternalRef2;
	__int64 m_nPreviousUserID;
	__int64 m_nNextUserID;
	CString m_strLegacyPebblePurseID;
	/*****/
	int m_nPurse1Allow;
	double m_dPurse1Liability;
	double m_dPurse1Credit;
	CString m_strPurse1LastSpendDate;
	CString m_strPurse1LastSpendTime;
	double m_dPurse1LastSpend;
	double m_dPurse1SpendToDate;
	/*****/
	int m_nPurse2Allow;
	double m_dPurse2Balance;
	CString m_strPurse2LastSpendDate;
	CString m_strPurse2LastSpendTime;
	double m_dPurse2LastSpend;
	double m_dPurse2SpendToDate;
	int m_nPurse2RefreshFlags;
	CString m_strPurse2RefreshedDate;
	CString m_strPurse2RefreshedTime;
	/*****/
	int m_nPurse3Allow;
	double m_dPurse3Liability;
	double m_dPurse3Credit;
	CString m_strPurse3LastSpendDate;
	CString m_strPurse3LastSpendTime;
	double m_dPurse3LastSpend;
	double m_dPurse3SpendToDate;
	/*****/
	double m_dCashSpendToDate;
	/*****/
	int m_nPoints;
	int m_nPointsToDate;
	/*****/
	bool m_bOwnMaxSpend;
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
	int m_nAlertCode;
	CString m_strAlertDate;
	CString m_strAlertTime;
	/*****/
	CByteArray m_arrayRowVersion;

private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************

