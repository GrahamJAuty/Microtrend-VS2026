#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowAccountFind
{
public:
	CSQLRowAccountFind();

public:
	__int64 GetUserID() { return m_nUserID; }
	CString GetFullname() { return m_strFullname; }
	CString GetForename() { return m_strForename; }
	CString GetSurname() { return m_strSurname; }
	CString GetExpiryDate() { return m_strExpiryDate; }
	int GetGroupNo() { return m_nGroupNo; }
	bool GetInactive() { return m_bInactive; }
	/*****/
	CString GetInfo1() { return m_strInfo1; }
	CString GetInfo2() { return m_strInfo2; }
	CString GetInfo3() { return m_strInfo3; }
	CString GetInfo4() { return m_strInfo4; }
	CString GetAddress1() { return m_strAddress1; }
	CString GetAddress2() { return m_strAddress2; }
	CString GetAddress3() { return m_strAddress3; }
	CString GetAddress4() { return m_strAddress4; }
	CString GetAddress5() { return m_strAddress5; }
	CString GetPhone1() { return m_strPhone1; }
	CString GetPhone2() { return m_strPhone2; }
	CString GetAlertCodes() { return m_strAlertCodes; }
	/*****/
	double GetPurse1SpendToDate() { return m_dPurse1SpendToDate; }
	double GetPurse2SpendToDate() { return m_dPurse2SpendToDate; }
	double GetCashSpendToDate() { return m_dCashSpendToDate; }
	int GetPointsToDate() { return m_nPointsToDate; }
	/*****/
	int GetHotlistCode() { return m_nHotlistCode; }

public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetFullname(CString str) { vdtr.SetFullname(str, m_strFullname); }
	void SetForename(CString str) { vdtr.SetForename(str, m_strForename); }
	void SetSurname(CString str) { vdtr.SetSurname(str, m_strSurname); }
	void SetExpiryDate(CString str) { vdtr.SetExpiryDate(str, m_strExpiryDate); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetInactive(bool b) { m_bInactive = b; }
	/*****/
	void SetInfo1(CString str) { vdtr.SetInfo(str, m_strInfo1); }
	void SetInfo2(CString str) { vdtr.SetInfo(str, m_strInfo2); }
	void SetInfo3(CString str) { vdtr.SetInfo(str, m_strInfo3); }
	void SetInfo4(CString str) { vdtr.SetInfo(str, m_strInfo4); }
	void SetAddress1(CString str) { vdtr.SetAddress(str, m_strAddress1); }
	void SetAddress2(CString str) { vdtr.SetAddress(str, m_strAddress2); }
	void SetAddress3(CString str) { vdtr.SetAddress(str, m_strAddress3); }
	void SetAddress4(CString str) { vdtr.SetAddress(str, m_strAddress4); }
	void SetAddress5(CString str) { vdtr.SetAddress(str, m_strAddress5); }
	void SetPhone1(CString str) { vdtr.SetPhone(str, m_strPhone1); }
	void SetPhone2(CString str) { vdtr.SetPhone(str, m_strPhone2); }
	/*****/
	void SetAlertCodes(CString str) { vdtr.SetAlertCodes(str, m_strAlertCodes); }
	/*****/
	void SetHotlistCode(int n) { vdtr.SetHotlistCode(n, m_nHotlistCode); }
	/*****/
	void SetPurse1SpendToDate(double d) { vdtr.SetPurse1SpendToDate(d, m_dPurse1SpendToDate); }
	void SetPurse2SpendToDate(double d) { vdtr.SetPurse2SpendToDate(d, m_dPurse2SpendToDate); }
	void SetCashSpendToDate(double d) { vdtr.SetCashSpendToDate(d, m_dCashSpendToDate); }
	void SetPointsToDate(int n) { vdtr.SetPointsToDate(n, m_nPointsToDate); }

public:
	bool HasUserExpired();
	int GetRecordStatusColour();
	bool IsAlertSet() { return (m_strAlertCodes != ""); }
	bool IsCardHotlisted() { return (GetHotlistCode() != 0); }
	bool IsCardUsed();

private:
	__int64 m_nUserID;
	CString m_strFullname;
	CString m_strForename;
	CString m_strSurname;
	CString m_strExpiryDate;
	int m_nGroupNo;
	bool m_bInactive;
	/*****/
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strAddress4;
	CString m_strAddress5;
	CString m_strPhone1;
	CString m_strPhone2;
	CString m_strAlertCodes;
	/*****/
	int m_nHotlistCode;
	/*****/
	double m_dPurse1SpendToDate;
	double m_dPurse2SpendToDate;
	double m_dCashSpendToDate;
	int m_nPointsToDate;

private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************

