#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowAccountList
{
public:
	CSQLRowAccountList();

public:
	__int64 GetUserID() { return m_nUserID; }
	CString GetUsername();
	CString GetForename() { return m_strForename; }
	CString GetSurname() { return m_strSurname; }
	CString GetExpiryDate() { return m_strExpiryDate; }
	int GetGroupNo() { return m_nGroupNo; }
	bool GetInactive() { return m_bInactive; }
	/*****/
	double GetPurse1Liability() { return m_dPurse1Liability; }
	double GetPurse1Credit() { return m_dPurse1Credit; }
	double GetPurse1Balance() { return m_dPurse1Liability + m_dPurse1Credit; }
	/*****/
	double GetPurse2Balance() { return m_dPurse2Balance; }
	/*****/
	double GetPurse3Liability() { return m_dPurse3Liability; }
	double GetPurse3Credit() { return m_dPurse3Credit; }
	double GetPurse3Balance() { return m_dPurse3Liability + m_dPurse3Credit; }
	/*****/
	int GetPoints() { return m_nPoints; }
	/*****/
	int GetAlertCode() { return m_nAlertCode; }
	/*****/
	CString GetMemberID() { return m_strMemberID;  }
	CString GetInfo1() { return m_strInfo1; }
	/*****/
	bool GetCardPickerFlag() { return m_bCardPickerFlag; }
	
public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetUsername(CString str) { vdtr.SetUsername(str, m_strUsername); }
	void SetForename(CString str) { vdtr.SetForename(str, m_strForename); }
	void SetSurname(CString str) { vdtr.SetSurname(str, m_strSurname); }
	void SetExpiryDate(CString str) { vdtr.SetExpiryDate(str, m_strExpiryDate); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetInactive(bool b) { m_bInactive = b; }
	/*****/
	void SetPurse1Liability(double d) { vdtr.SetPurse1Liability(d, m_dPurse1Liability); }
	void SetPurse1Credit(double d) { vdtr.SetPurse1Credit(d, m_dPurse1Credit); }
	/*****/
	void SetPurse2Balance(double d) { vdtr.SetPurse2Balance(d, m_dPurse2Balance); }
	/*****/
	void SetPurse3Liability(double d) { vdtr.SetPurse3Liability(d, m_dPurse3Liability); }
	void SetPurse3Credit(double d) { vdtr.SetPurse3Credit(d, m_dPurse3Credit); }
	/*****/
	void SetPoints(int n) { vdtr.SetPoints(n, m_nPoints); }
	/*****/
	void SetAlertCode(int n) { vdtr.SetAlertCode(n, m_nAlertCode); }
	/*****/
	void SetMemberID(CString str) { vdtr.SetMemberID(str, m_strMemberID); }
	void SetInfo1(CString str) { vdtr.SetInfo1(str, m_strInfo1); }
	/*****/
	void SetCardPickerFlag(bool b) { m_bCardPickerFlag = b; }

public:
	int GetRecordStatusColour();
	CString GetACListRecordStatusText();
	bool HasUserExpired();
	bool IsAlertSet() { return (GetAlertCode() != 0); }
	CString GetGroupNoString();
	CString GetPurseBalanceString(int nPurseNo);
	CString GetTotalBalanceString();
	CString GetPointsString();

private:
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strForename;
	CString m_strSurname;
	CString m_strExpiryDate;
	int m_nGroupNo;
	bool m_bInactive;
	/*****/
	double m_dPurse1Liability;
	double m_dPurse1Credit;
	/*****/
	double m_dPurse2Balance;
	/*****/
	double m_dPurse3Liability;
	double m_dPurse3Credit;
	/*****/
	int m_nPoints;
	/*****/
	int m_nAlertCode;
	/*****/
	CString m_strMemberID;
	CString m_strInfo1;
	/*****/
	bool m_bCardPickerFlag;

private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************

