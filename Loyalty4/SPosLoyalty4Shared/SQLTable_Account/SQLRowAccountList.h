#pragma once
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowAccountList
{
public:
	CSQLRowAccountList();

public:
	__int64 GetACListUserID() { return m_nUserID; }
	CString GetACListFullname() { return m_strFullname; }
	int GetACListGroupNo() { return m_nGroupNo; }
	CString GetACListExpiryDate() { return m_strExpiryDate; }
	CString GetACListInfo1() { return m_strInfo1; }
	CString GetACListInfo2() { return m_strInfo2; }
	CString GetACListInfo3() { return m_strInfo3; }
	CString GetACListInfo4() { return m_strInfo4; }
	CString GetACListAddress1() { return m_strAddress1; }
	CString GetACListAddress5() { return m_strAddress5; }
	bool GetACListInactive() { return m_bInactive; }
	/*****/
	double GetACListPurse1Balance() { return m_dPurse1Balance; }
	double GetACListPurse1SpendToDate() { return m_dPurse1SpendToDate; }
	/*****/
	double GetACListPurse2Balance() { return m_dPurse2Balance; }
	double GetACListPurse2SpendToDate() { return m_dPurse2SpendToDate; }
	/*****/
	double GetACListCashSpendToDate() { return m_dCashSpendToDate; }
	/*****/
	int GetACListPoints() { return m_nPoints; }
	int GetACListPointsToDate() { return m_nPointsToDate; }
	/*****/
	int GetACListHotlistCode() { return m_nHotlistCode; }
	/*****/
	CString GetACListAlertCodes() { return m_strAlertCodes;  }
	/*****/
	bool GetACListCardPickerFlag() { return m_bCardPickerFlag; }
	bool GetACListDataLoadedFlag() { return m_bDataLoadedFlag; }

public:
	void SetACListUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetACListFullname(CString str) { vdtr.SetFullname(str, m_strFullname); }
	void SetACListGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetACListExpiryDate(CString str) { vdtr.SetExpiryDate(str, m_strExpiryDate); }
	void SetACListInfo1(CString str){ vdtr.SetInfo(str, m_strInfo1); }
	void SetACListInfo2(CString str) { vdtr.SetInfo(str, m_strInfo2); }
	void SetACListInfo3(CString str) { vdtr.SetInfo(str, m_strInfo3); }
	void SetACListInfo4(CString str) { vdtr.SetInfo(str, m_strInfo4); }
	void SetACListAddress1(CString str) { vdtr.SetInfo(str, m_strAddress1); }
	void SetACListAddress5(CString str) { vdtr.SetInfo(str, m_strAddress5); }
	void SetACListInactive(bool b) { m_bInactive = b; }
	/*****/
	void SetACListPurse1Balance(double d) { vdtr.SetPurse1Balance(d, m_dPurse1Balance); }
	void SetACListPurse1SpendToDate(double d) { vdtr.SetPurse1SpendToDate(d, m_dPurse1SpendToDate); }
	/*****/
	void SetACListPurse2Balance(double d) { vdtr.SetPurse2Balance(d, m_dPurse2Balance); }
	void SetACListPurse2SpendToDate(double d) { vdtr.SetPurse2SpendToDate(d, m_dPurse2SpendToDate); }
	/*****/
	void SetACListCashSpendToDate(double d) { vdtr.SetCashSpendToDate(d, m_dCashSpendToDate); }
	/*****/
	void SetACListPoints(int n) { vdtr.SetPoints(n, m_nPoints); }
	void SetACListPointsToDate(int n) { vdtr.SetPointsToDate(n, m_nPointsToDate); }
	/*****/
	void SetACListHotlistCode(int n) { vdtr.SetHotlistCode(n, m_nHotlistCode); }
	/*****/
	void SetACListAlertCodes(CString str) { vdtr.SetAlertCodes(str, m_strAlertCodes); }
	/*****/
	void SetACListCardPickerFlag(bool b) { m_bCardPickerFlag = b; }
	void SetACListDataLoadedFlag(bool b) { m_bDataLoadedFlag = b; }

public:
	int GetACListRecordStatusColour();
	CString GetACListRecordStatusText();
	bool HasACListCardExpired();
	bool IsACListCardHotlisted() { return (GetACListHotlistCode() != 0); }
	bool IsACListAlertSet() { return (m_strAlertCodes != ""); }
	bool IsACListCardUsed();
	CString GetACListGroupNoString();
	CString GetACListPurseBalanceString(int nPurseNo);
	CString GetACListPointsString();
	CString GetACListTotalBalanceString();
	CString GetACListPointsToDateString();
	CString GetACListSpendToDateString();

private:
	__int64 m_nUserID;
	CString m_strFullname;
	int m_nGroupNo;
	CString m_strExpiryDate;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strAddress1;
	CString m_strAddress5;
	bool m_bInactive;
	/*****/
	double m_dPurse1Balance;
	double m_dPurse1SpendToDate;
	/*****/
	double m_dPurse2Balance;
	double m_dPurse2SpendToDate;
	/*****/
	double m_dCashSpendToDate;
	/*****/
	int m_nPoints;
	int m_nPointsToDate;
	/*****/
	int m_nHotlistCode;
	/*****/
	CString m_strAlertCodes;
	/*****/
	bool m_bCardPickerFlag;
	bool m_bDataLoadedFlag;

private:
	CSQLFieldValidatorAccount vdtr;
};

//**********************************************************************
