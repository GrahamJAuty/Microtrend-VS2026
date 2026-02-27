#pragma once
//**********************************************************************
#include "SQLFieldValidatorUserText.h"
//**********************************************************************

class CSQLRowUserText
{
public:
	CSQLRowUserText();
	
private:
	void Reset();
	
public:
	CString GetFieldLabel() { return m_strFieldLabel; }
	CString GetUserText() { return m_strUserText; }
	bool GetUseFlag() { return m_bUseFlag; }
	
public:
	void SetFieldLabel(CString str) { vdtr.SetFieldLabel(str, m_strFieldLabel); }
	void SetUserText(CString str) { vdtr.SetUserText(str, m_strUserText); }
	void SetUseFlag(bool b) { m_bUseFlag = b; }
	
private:
	CString m_strFieldLabel;
	CString m_strUserText;
	bool m_bUseFlag;

private:
	CSQLFieldValidatorUserText vdtr;
};

//**********************************************************************

