#pragma once
//**********************************************************************
#include "SQLFieldValidatorBromComBalanceChanges.h"
//**********************************************************************

class CSQLRowBromComBalanceChanges
{
public:
	CSQLRowBromComBalanceChanges();
	
private:
	void Reset();
	
public:
	int GetPersonID() { return m_nPersonID; }
	__int64 GetAccountID() { return m_nAccountID; }

public:
	void SetPersonID(int n) { vdtr.SetPersonID(n, m_nPersonID); }
	void SetAccountID(__int64 n) { vdtr.SetAccountID(n, m_nAccountID); }
	
private:
	int m_nPersonID;
	__int64 m_nAccountID;

private:
	CSQLFieldValidatorBromComBalanceChanges vdtr;
};

//**********************************************************************

