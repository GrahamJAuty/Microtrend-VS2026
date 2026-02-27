#pragma once
//**********************************************************************
#include "SQLFieldValidatorBromComPendingOrder.h"
//**********************************************************************

class CSQLRowBromComPendingOrder
{
public:
	CSQLRowBromComPendingOrder();
	
private:
	void Reset();
	
public:
	int GetOrderID() { return m_nOrderID;  }
	CString GetStatus() { return m_strStatus; }
	CString GetLastModified() { return m_strLastModified; }
	bool GetNewOrderFlag() { return m_bNewOrder; }
	
public:
	void SetOrderID(int n) { vdtr.SetOrderID(n, m_nOrderID); }
	void SetStatus(CString str) { vdtr.SetStatus(str, m_strStatus); }
	void SetLastModified(CString str) { vdtr.SetLastModified(str, m_strLastModified); }
	void SetNewOrderFlag(bool b) { m_bNewOrder = b; }
	
private:
	int m_nOrderID;
	CString m_strStatus;
	CString m_strLastModified;
	bool m_bNewOrder;
	
private:
	CSQLFieldValidatorBromComPendingOrder vdtr;
};

//**********************************************************************

