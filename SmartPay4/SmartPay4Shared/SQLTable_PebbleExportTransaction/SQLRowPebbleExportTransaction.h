#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebbleExportTransaction.h"
//**********************************************************************

class CSQLRowPebbleExportTransaction
{
public:
	CSQLRowPebbleExportTransaction();
	
private:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	__int64 GetUserID() { return m_nUserID; }
	__int64 GetXFerUserID() { return m_nXFerUserID; }
	CString GetMemberID() { return m_strMemberID; }
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	int GetCCNo() { return m_nCCNo; }
	int GetSourceType() { return m_nSourceType; }
	int GetAppNo() { return m_nAppNo; }
	double GetAmount() { return m_dAmount; }
	double GetPurse1() { return m_dPurse1; }
	double GetPurse2() { return m_dPurse2; }
	double GetCash() { return m_dCash; }
	CString GetWebComment() { return m_strWebComment; }

public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetXFerUserID(__int64 n) { vdtr.SetXFerUserID(n, m_nXFerUserID); }
	void SetMemberID(CString str) { vdtr.SetMemberID(str, m_strMemberID); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	void SetCCNo(int n) { vdtr.SetCCNo(n, m_nCCNo); }
	void SetSourceType(int n) { vdtr.SetSourceType(n, m_nSourceType); }
	void SetAppNo(int n) { vdtr.SetAppNo(n, m_nAppNo); }
	void SetAmount(double d) { vdtr.SetAmount(d, m_dAmount); }
	void SetPurse1(double d) { vdtr.SetPurse1(d, m_dPurse1); }
	void SetPurse2(double d) { vdtr.SetPurse2(d, m_dPurse2); }
	void SetCash(double d) { vdtr.SetCash(d, m_dCash); }
	void SetWebComment(CString str) { vdtr.SetWebComment(str, m_strWebComment); }

private:
	__int64 m_nLineID;
	__int64 m_nUserID;
	__int64 m_nXFerUserID;
	CString m_strMemberID;
	CString m_strDate;
	CString m_strTime;
	int m_nCCNo;
	int m_nSourceType;
	int m_nAppNo;
	double m_dAmount;
	double m_dPurse1;
	double m_dPurse2;
	double m_dCash;
	CString m_strWebComment;

private:
	CSQLFieldValidatorPebbleExportTransaction vdtr;
};

//**********************************************************************
