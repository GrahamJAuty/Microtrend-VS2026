#pragma once
//**********************************************************************
#include "SQLFieldValidatorStaffGiftException.h"
//**********************************************************************

class CSQLRowStaffGiftException
{
public:
	CSQLRowStaffGiftException();
	
public:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	__int64 GetCustomerID() { return m_nCustomerID; }
	int GetServerNo() { return m_nServerNo; }
	int GetFolderIndexDbNo() { return m_nFolderIndexDbNo; }
	int GetFolderIndexSetNo() { return m_nFolderIndexSetNo; }
	__int64 GetServerID() { return m_nServerID; }
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	double GetValue() { return m_dValue; }
	int GetErrorCode() { return m_nErrorCode; }

public:
	CString GetDisplayCustomerID() { return m_strDisplayCustomerID; }
	CString GetDisplayCustomerName() { return m_strDisplayCustomerName; }
	CString GetDisplayServerID() { return m_strDisplayServerID; }
	CString GetDisplayServerName() { return m_strDisplayServerName; }

public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetCustomerID(__int64 n) { vdtr.SetCustomerID(n, m_nCustomerID); }
	void SetServerNo(int n) { vdtr.SetServerNo(n, m_nServerNo); }
	void SetFolderIndexDbNo( int n ) { vdtr.SetFolderIndexDbNo(n, m_nFolderIndexDbNo); }
	void SetFolderIndexSetNo(int n) { vdtr.SetFolderIndexSetNo(n, m_nFolderIndexSetNo); }
	void SetServerID(__int64 n) { vdtr.SetServerID(n, m_nServerID); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	void SetValue(double d) { vdtr.SetValue(d, m_dValue); }
	void AddToValue(double d) { vdtr.SetValue( m_dValue + d, m_dValue); }
	void SetErrorCode(int n) { vdtr.SetErrorCode(n, m_nErrorCode); }

public:
	void SetDisplayCustomerID(CString str) { m_strDisplayCustomerID = str; }
	void SetDisplayCustomerName(CString str) { m_strDisplayCustomerName = str; }
	void SetDisplayServerID(CString str) { m_strDisplayServerID = str; }
	void SetDisplayServerName(CString str) { m_strDisplayServerName = str; }

public:
	void SetDisplayUserInfo();

private:
	__int64 m_nLineID;
	__int64 m_nCustomerID;
	int m_nServerNo;
	int m_nFolderIndexDbNo;
	int m_nFolderIndexSetNo;
	__int64 m_nServerID;
	CString m_strDate;
	CString m_strTime;
	double m_dValue;
	int m_nErrorCode;

private:
	CString m_strDisplayCustomerID;
	CString m_strDisplayCustomerName;
	CString m_strDisplayServerID;
	CString m_strDisplayServerName;

private:
	CSQLFieldValidatorStaffGiftException vdtr;
};

//**********************************************************************

