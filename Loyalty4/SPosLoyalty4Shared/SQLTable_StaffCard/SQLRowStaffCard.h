#pragma once
//**********************************************************************
#include "SQLFieldValidatorStaffCard.h"
//**********************************************************************

class CSQLRowStaffCard
{
public:
	CSQLRowStaffCard();
	
private:
	void Reset();
	
public:
	int GetFolderIndexDbNo() { return m_nFolderIndexDbNo; }
	int GetFolderIndexSetNo() { return m_nFolderIndexSetNo; }
	int GetServerNo() { return m_nServerNo; }
	__int64 GetUserID() { return m_nUserID;  }

public:
	CString GetDisplayUserID() { return m_strDisplayUserID; }
	CString GetDisplayAccountName() { return m_strDisplayAccountName;  }

public:
	void SetFolderIndexDbNo(int n) { vdtr.SetFolderIndexDbNo(n, m_nFolderIndexDbNo); }
	void SetFolderIndexSetNo(int n) { vdtr.SetFolderIndexSetNo(n, m_nFolderIndexSetNo); }
	void SetServerNo(int n) { vdtr.SetServerNo(n, m_nServerNo); }
	void SetUserID( __int64 n ) { vdtr.SetUserID(n, m_nUserID); }

public:
	int GetConsolidationPos() { return ( m_nFolderIndexDbNo * 1000000 ) + (m_nFolderIndexSetNo * 10000) + m_nServerNo; }

public:
	void SetDisplayUserInfo();
	void SetDisplayUserID(CString str) { m_strDisplayUserID = str; }
	void SetDisplayAccountName( CString str) { m_strDisplayAccountName = str; }

private:
	int m_nFolderIndexDbNo;
	int m_nFolderIndexSetNo;
	int m_nServerNo;
	__int64 m_nUserID;

private:
	CString m_strDisplayUserID;
	CString m_strDisplayAccountName;

private:
	CSQLFieldValidatorStaffCard vdtr;
};

//**********************************************************************

