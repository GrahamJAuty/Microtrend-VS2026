#pragma once
//**********************************************************************
#include "SQLFieldValidatorExternalAccount.h"
//**********************************************************************

class CSQLRowExternalAccount
{
public:
	CSQLRowExternalAccount();
	
private:
	void Reset();
	
public:
	CString GetAESUsername() { return m_strAESUsername; }
	CString GetPasswordHash() { return m_strPasswordHash; }
	CString GetAESUserID() { return m_strAESUserID; }

public:
	void SetAESUsername(CString str) { vdtr.SetAESUsername(str, m_strAESUsername); }
	void SetPasswordHash(CString str) { vdtr.SetPasswordHash(str, m_strPasswordHash); }
	void SetAESUserID(CString str) { vdtr.SetAESUserID(str, m_strAESUserID); }

private:
	CString m_strAESUsername;
	CString m_strPasswordHash;
	CString m_strAESUserID;

private:
	CSQLFieldValidatorExternalAccount vdtr;
};

//**********************************************************************

