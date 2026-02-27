#pragma once
//**********************************************************************
#include "SQLFieldValidatorBromComItem.h"
//**********************************************************************

class CSQLRowBromComItem
{
public:
	CSQLRowBromComItem();
	
private:
	void Reset();
	
public:
	int GetOrderID() { return m_nOrderID; }
	int GetPersonID() { return m_nPersonID; }
	double GetAmount() { return m_dAmount;  }
	CString GetStatus() { return m_strStatus; }

public:
	void SetOrderID(int n) { vdtr.SetOrderID(n, m_nOrderID); }
	void SetPersonID(int n) { vdtr.SetPersonID(n, m_nPersonID); }
	void SetAmount(double d) { vdtr.SetAmount(d, m_dAmount); }
	void SetStatus(CString str) { vdtr.SetStatus(str, m_strStatus); }

private:
	int m_nOrderID;
	int m_nPersonID;
	double m_dAmount;
	CString m_strStatus;

private:
	CSQLFieldValidatorBromComItem vdtr;
};

//**********************************************************************