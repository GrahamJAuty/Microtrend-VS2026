#pragma once
//**********************************************************************
#include "SQLFieldValidatorPurchaseHistoryHeader.h"
//**********************************************************************

class CSQLRowPurchaseHistoryHeader
{
public:
	CSQLRowPurchaseHistoryHeader();

private:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	
public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	
private:
	__int64 m_nLineID = 0;
	CString m_strDate = "";	
	CString m_strTime = "";
	
private:
	CSQLFieldValidatorPurchaseHistoryHeader vdtr;
};

//**********************************************************************
