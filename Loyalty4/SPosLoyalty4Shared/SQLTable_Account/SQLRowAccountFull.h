#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
#include "SQLRowAccountList.h"
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
	__int64 GetUserID() { return m_nUserID; }
	CString GetUserIDString();
	CString GetFullname() { return m_strFullname; }
	CString GetForename() { return m_strForename; }
	CString GetSurname() { return m_strSurname; }
	CString GetDOB() { return m_strDOB; }
	CString GetExpiryDate() { return m_strExpiryDate; }
	CString GetGender() { return m_strGender; }
	int GetGroupNo() { return m_nGroupNo; }
	int GetSchemeNo() { return m_nSchemeNo; }
	CString GetInfo1() { return m_strInfo1; }
	CString GetInfo2() { return m_strInfo2; }
	CString GetInfo3() { return m_strInfo3; }
	CString GetInfo4() { return m_strInfo4; }
	CString GetQRInfo() { return m_strQRInfo; }
	CString GetAddress1() { return m_strAddress1; }
	CString GetAddress2() { return m_strAddress2; }
	CString GetAddress3() { return m_strAddress3; }
	CString GetAddress4() { return m_strAddress4; }
	CString GetAddress5() { return m_strAddress5; }
	CString GetPhone1() { return m_strPhone1; }
	CString GetPhone2() { return m_strPhone2; }
	bool GetInactive() { return m_bInactive; }
	/*****/
	CString GetExternalRef() { return m_strExternalRef; }
	__int64 GetPreviousUserID() { return m_nPreviousUserID; }
	__int64 GetNextUserID() { return m_nNextUserID; }
	/*****/
	double GetPurse1Balance();
	CString GetPurse1LastSpendDate() { return m_strPurse1LastSpendDate; }
	CString GetPurse1LastSpendTime() { return m_strPurse1LastSpendTime; }
	double GetPurse1LastSpend() { return m_dPurse1LastSpend; }
	double GetPurse1SpendToDate() { return m_dPurse1SpendToDate; }
	/*****/
	double GetPurse2Balance() { return m_dPurse2Balance; }
	CString GetPurse2LastSpendDate() { return m_strPurse2LastSpendDate; }
	CString GetPurse2LastSpendTime() { return m_strPurse2LastSpendTime; }
	double GetPurse2LastSpend() { return m_dPurse2LastSpend; }
	double GetPurse2SpendToDate() { return m_dPurse2SpendToDate; }
	CString GetPurse2RefreshedDate() { return m_strPurse2RefreshedDate; }
	CString GetPurse2RefreshedTime() { return m_strPurse2RefreshedTime; }
	/*****/
	CString GetCashLastSpendDate() { return m_strCashLastSpendDate; }
	CString GetCashLastSpendTime() { return m_strCashLastSpendTime; }
	double GetCashLastSpend() { return m_dCashLastSpend; }
	double GetCashSpendToDate() { return m_dCashSpendToDate; }
	/*****/
	int GetPoints() { return m_nPoints; }
	int GetPointsToDate() { return m_nPointsToDate; }
	/*****/
	int GetHotlistCode() { return m_nHotlistCode; }
	CString GetHotlistDate() { return m_strHotlistDate; }
	CString GetHotlistTime() { return m_strHotlistTime; }
	/*****/
	double GetMaxSpend() { return m_dMaxSpend; }
	double GetMaxOverdraft() { return m_dMaxOverdraft; }
	/*****/
	CString GetAlertCodes() { return m_strAlertCodes; }
	/*****/
	
public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUserIDString(CString str) { vdtr.SetUserID(_atoi64(str), m_nUserID); }
	void SetFullname(CString str) { vdtr.SetFullname(str, m_strFullname); }
	void SetForename(CString str) { vdtr.SetForename(str, m_strForename); }
	void SetSurname(CString str) { vdtr.SetSurname(str, m_strSurname); }
	void SetDOB(CString str) { vdtr.SetDOB(str, m_strDOB); }
	void SetExpiryDate(CString str) { vdtr.SetExpiryDate(str, m_strExpiryDate); }
	void SetGender(CString str);
	void SetGender(int nGender);
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetSchemeNo(int n) { vdtr.SetSchemeNo(n, m_nSchemeNo); }
	void SetInfo1(CString str) { vdtr.SetInfo(str, m_strInfo1); }
	void SetInfo2(CString str) { vdtr.SetInfo(str, m_strInfo2); }
	void SetInfo3(CString str) { vdtr.SetInfo(str, m_strInfo3); }
	void SetInfo4(CString str) { vdtr.SetInfo(str, m_strInfo4); }
	void SetQRInfo(CString str) { vdtr.SetQRInfo(str, m_strQRInfo); }
	void SetAddress1(CString str) { vdtr.SetAddress(str, m_strAddress1); }
	void SetAddress2(CString str) { vdtr.SetAddress(str, m_strAddress2); }
	void SetAddress3(CString str) { vdtr.SetAddress(str, m_strAddress3); }
	void SetAddress4(CString str) { vdtr.SetAddress(str, m_strAddress4); }
	void SetAddress5(CString str) { vdtr.SetAddress(str, m_strAddress5); }
	void SetPhone1(CString str) { vdtr.SetPhone(str, m_strPhone1); }
	void SetPhone2(CString str) { vdtr.SetPhone(str, m_strPhone2); }
	void SetInactive(bool b) { m_bInactive = b;  }
	/*****/
	void SetExternalRef(CString str) { vdtr.SetExternalRef(str, m_strExternalRef); }
	void SetPreviousUserID(__int64 n) { vdtr.SetPreviousUserID(n, m_nPreviousUserID); }
	void SetNextUserID(__int64 n) { vdtr.SetNextUserID(n, m_nNextUserID); }
	/*****/
	void SetPurse1Balance(double d) { vdtr.SetPurse1Balance(d, m_dPurse1Balance); }
	void SetPurse1LastSpendDate(CString str) { vdtr.SetPurse1LastSpendDate(str, m_strPurse1LastSpendDate); }
	void SetPurse1LastSpendTime(CString str) { vdtr.SetPurse1LastSpendTime(str, m_strPurse1LastSpendTime); }
	void SetPurse1LastSpend(double d) { vdtr.SetPurse1LastSpend(d, m_dPurse1LastSpend); }
	void SetPurse1SpendToDate(double d) { vdtr.SetPurse1SpendToDate(d, m_dPurse1SpendToDate); }
	/*****/
	void SetPurse2Balance(double d) { vdtr.SetPurse2Balance(d, m_dPurse2Balance); }
	void SetPurse2LastSpendDate(CString str) { vdtr.SetPurse2LastSpendDate(str, m_strPurse2LastSpendDate); }
	void SetPurse2LastSpendTime(CString str) { vdtr.SetPurse2LastSpendTime(str, m_strPurse2LastSpendTime); }
	void SetPurse2LastSpend(double d) { vdtr.SetPurse2LastSpend(d, m_dPurse2LastSpend); }
	void SetPurse2SpendToDate(double d) { vdtr.SetPurse2SpendToDate(d, m_dPurse2SpendToDate); }
	void SetPurse2RefreshedDate(CString str) { vdtr.SetPurse2RefreshedDate(str, m_strPurse2RefreshedDate); }
	void SetPurse2RefreshedTime(CString str) { vdtr.SetPurse2RefreshedTime(str, m_strPurse2RefreshedTime); }
	/*****/
	void SetCashLastSpendDate(CString str) { vdtr.SetCashLastSpendDate(str, m_strCashLastSpendDate); }
	void SetCashLastSpendTime(CString str) { vdtr.SetCashLastSpendTime(str, m_strCashLastSpendTime); }
	void SetCashLastSpend(double d) { vdtr.SetCashLastSpend(d, m_dCashLastSpend); }
	void SetCashSpendToDate(double d) { vdtr.SetCashSpendToDate(d, m_dCashSpendToDate); }
	/*****/
	void SetPoints(int n) { vdtr.SetPoints(n, m_nPoints); }
	void SetPointsToDate(int n) { vdtr.SetPointsToDate(n, m_nPointsToDate); }
	/*****/
	void SetHotlistCode(int n) { vdtr.SetHotlistCode(n, m_nHotlistCode); }
	void SetHotlistDate(CString str) { vdtr.SetHotlistDate(str, m_strHotlistDate); }
	void SetHotlistTime(CString str) { vdtr.SetHotlistTime(str, m_strHotlistTime); }
	/*****/
	void SetMaxSpend(double d) { vdtr.SetMaxSpend(d, m_dMaxSpend); }
	void SetMaxOverdraft(double d) { vdtr.SetMaxOverdraft(d, m_dMaxOverdraft); }
	/*****/
	void SetAlertCodes(CString str) { vdtr.SetAlertCodes(str, m_strAlertCodes); }
	/*****/
	void SetRowVersion(CByteArray& arraySource);
	
public:
	bool IsStringEmpty(const char* szText);
	bool GetLastUsed(CSSDate& date, CSSTime& time);
	bool HasCardExpired();
	bool HaveNextUserID() { return (GetNextUserID() != 0); }
	bool HavePreviousUserID() { return (GetPreviousUserID() != 0); }
	void DivideCardName(const char* szCardName, CString& strFirstName, CString& strSecondName);
	void RefreshPurse2(double dValue, const char* szDate, const char* szTime);
	bool IsBirthdayToday();
	CString GetAgeNextBirthday();
	int GetAge(bool bNextBDay = FALSE );
	CString GetDOBDayOfWeek();
	double GetTotalSpendToDate() { return (m_dPurse1SpendToDate + m_dPurse2SpendToDate + m_dCashSpendToDate); }
	bool GetBoolData(const char* szData);
	void UpdatePoints(int nPoints);
	bool IsGenderMale() { return (GetGender() == "M"); }
	bool IsGenderFemale() { return (GetGender() == "F"); }
	bool IsGenderUnknown() { return (GetGender() == ""); }
	CString GetGenderText();
	bool IsCardHotlisted() { return (GetHotlistCode() != 0); }
	void SetHotlistCard(int nHotlistCode);
	bool IsRecordEmpty();
	CString GetGroupNoString();
	bool CompareRowVersion(CSQLRowAccountFull& RowSource);

public:
	void AddToPurse1(double dValue) { SetPurse1Balance(GetPurse1Balance() + dValue); }
	void SubtractFromPurse1(double dValue) { SetPurse1Balance(GetPurse1Balance() - dValue); }
	void AddToPurse1LastSpend(double dValue) { SetPurse1LastSpend(GetPurse1LastSpend() + dValue); }
	void AddToPurse1SpendToDate(double dValue) { SetPurse1SpendToDate(GetPurse1SpendToDate() + dValue); }
	void AddToPurse2(double dValue) { SetPurse2Balance(GetPurse2Balance() + dValue); }
	void SubtractFromPurse2(double dValue) { SetPurse2Balance(GetPurse2Balance() - dValue); }
	void AddToPurse2LastSpend(double dValue) { SetPurse2LastSpend(GetPurse2LastSpend() + dValue); }
	void AddToPurse2SpendToDate(double dValue) { SetPurse2SpendToDate(GetPurse2SpendToDate() + dValue); }
	void AddToPoints(int nPoints) { SetPoints(GetPoints() + nPoints); }
	void AddToPointsToDate(int nPoints) { SetPointsToDate(GetPointsToDate() + nPoints); }
	void AddToCashLastSpend(double dValue) { SetCashLastSpend(GetCashLastSpend() + dValue); }
	void AddToCashSpendToDate(double dValue) { SetCashSpendToDate(GetCashSpendToDate() + dValue); }

private:
	__int64 m_nUserID;
	CString m_strFullname;
	CString m_strForename;
	CString m_strSurname;
	CString m_strDOB;
	CString m_strExpiryDate;
	CString m_strGender;
	int m_nGroupNo;
	int m_nSchemeNo;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strQRInfo;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strAddress4;
	CString m_strAddress5;
	CString m_strPhone1;
	CString m_strPhone2;
	bool m_bInactive;
	/*****/
	CString m_strExternalRef;
	__int64 m_nPreviousUserID;
	__int64 m_nNextUserID;
	/*****/
	double m_dPurse1Balance;
	CString m_strPurse1LastSpendDate;
	CString m_strPurse1LastSpendTime;
	double m_dPurse1LastSpend;
	double m_dPurse1SpendToDate;
	/*****/
	double m_dPurse2Balance;
	CString m_strPurse2LastSpendDate;
	CString m_strPurse2LastSpendTime;
	double m_dPurse2LastSpend;
	double m_dPurse2SpendToDate;
	CString m_strPurse2RefreshedDate;
	CString m_strPurse2RefreshedTime;
	/*****/
	CString m_strCashLastSpendDate;
	CString m_strCashLastSpendTime;
	double m_dCashLastSpend;
	double m_dCashSpendToDate;
	/*****/
	int m_nPoints;
	int m_nPointsToDate;
	/*****/
	int m_nHotlistCode;
	CString m_strHotlistDate;
	CString m_strHotlistTime;
	/*****/
	double m_dMaxSpend;
	double m_dMaxOverdraft;
	/*****/
	CString m_strAlertCodes;
	/*****/
	CByteArray m_arrayRowVersion;
	
private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************

