#pragma once
//**********************************************************************
#include "SQLFieldValidatorBromComPendingItem.h"
//**********************************************************************

class CSQLRowBromComPendingItem
{
public:
	CSQLRowBromComPendingItem();
	
private:
	void Reset();
	
public:
	int GetOrderID() { return m_nOrderID; }
	int GetPersonID() { return m_nPersonID; }
	double GetAmount() { return m_dAmount;  }

public:
	void SetOrderID(int n) { vdtr.SetOrderID(n, m_nOrderID); }
	void SetPersonID(int n) { vdtr.SetPersonID(n, m_nPersonID); }
	void SetAmount(double d) { vdtr.SetAmount(d, m_dAmount); }
	
private:
	int m_nOrderID;
	int m_nPersonID;
	double m_dAmount;

private:
	CSQLFieldValidatorBromComPendingItem vdtr;
};

//**********************************************************************

