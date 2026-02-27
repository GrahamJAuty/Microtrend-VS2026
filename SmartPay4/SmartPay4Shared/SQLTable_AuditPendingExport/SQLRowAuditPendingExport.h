#pragma once
//**********************************************************************
#include "SQLFieldValidatorAuditPendingExport.h"
//**********************************************************************

class CSQLRowAuditPendingExport
{
public:
	CSQLRowAuditPendingExport();

public:
	void Reset();

public:
	__int64 GetLineID() { return m_nLineID; }
	CString GetUserID() { return m_strUserID; }
	CString GetXFerUserID() { return m_strXFerUserID; }
	CString GetUserName() { return m_strUserName; }
	int GetGroupNo() { return m_nGroupNo; }
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	int GetActionType() { return m_nActionType; }
	int GetSourceType() { return m_nSourceType; }
	int GetAppNo() { return m_nAppNo; }
	int GetTerminalNo() { return m_nTerminalNo; }
	int GetCCNo() { return m_nCCNo; }
	CString GetText() { return m_strText; }
	int GetPointsAdded() { return m_nPointsAdded; }
	int GetPointsBalance() { return m_nPointsBalance; }
	double GetPurse1Value() { return m_dPurse1Value; }
	double GetPurse2Value() { return m_dPurse2Value; }
	double GetPurse3Value() { return m_dPurse3Value; }
	double GetCashValue() { return m_dCashValue; }
	double GetPurse1Balance() { return m_dPurse1Balance; }
	double GetPurse2Balance() { return m_dPurse2Balance; }
	double GetPurse3Balance() { return m_dPurse3Balance; }
	CString GetNotes() { return m_strNotes; }

public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetUserID(CString str) { vdtr.SetUserID(str, m_strUserID); }
	void SetXFerUserID(CString str) { vdtr.SetXFerUserID(str, m_strXFerUserID); }
	void SetUserName(CString str) { vdtr.SetUserName(str, m_strUserName); }
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	void SetActionType(int n) { vdtr.SetActionType(n, m_nActionType); }
	void SetSourceType(int n) { vdtr.SetSourceType(n, m_nSourceType); }
	void SetAppNo(int n) { vdtr.SetAppNo(n, m_nAppNo); }
	void SetTerminalNo(int n) { vdtr.SetTerminalNo(n, m_nTerminalNo); }
	void SetCCNo(int n) { vdtr.SetCCNo(n, m_nCCNo); }
	void SetText(CString str) { vdtr.SetText(str, m_strText); }
	void SetPointsAdded(int n) { vdtr.SetPointsAdded(n, m_nPointsAdded); }
	void SetPointsBalance(int n) { vdtr.SetPointsBalance(n, m_nPointsBalance); }
	void SetPurse1Value(double d) { vdtr.SetPurse1Value(d, m_dPurse1Value); }
	void SetPurse2Value(double d) { vdtr.SetPurse2Value(d, m_dPurse2Value); }
	void SetPurse3Value(double d) { vdtr.SetPurse3Value(d, m_dPurse3Value); }
	void SetCashValue(double d) { vdtr.SetCashValue(d, m_dCashValue); }
	void SetPurse1Balance(double d) { vdtr.SetPurse1Balance(d, m_dPurse1Balance); }
	void SetPurse2Balance(double d) { vdtr.SetPurse2Balance(d, m_dPurse2Balance); }
	void SetPurse3Balance(double d) { vdtr.SetPurse3Balance(d, m_dPurse3Balance); }
	void SetNotes(CString str) { vdtr.SetNotes(str, m_strNotes); }

public:
	CString GetXmlDate(int nWebPaymentType);

private:
	__int64 m_nLineID;
	CString m_strUserID;
	CString m_strXFerUserID;
	CString m_strUserName;
	int m_nGroupNo;
	CString m_strDate;
	CString m_strTime;
	int m_nActionType;
	int m_nSourceType;
	int m_nAppNo;
	int m_nTerminalNo;
	int m_nCCNo;
	CString m_strText;
	int m_nPointsAdded;
	int m_nPointsBalance;
	double m_dPurse1Value;
	double m_dPurse2Value;
	double m_dPurse3Value;
	double m_dCashValue;
	double m_dPurse1Balance;
	double m_dPurse2Balance;
	double m_dPurse3Balance;
	CString m_strNotes;

private:
	CSQLFieldValidatorAuditPendingExport vdtr;
};

//**********************************************************************

