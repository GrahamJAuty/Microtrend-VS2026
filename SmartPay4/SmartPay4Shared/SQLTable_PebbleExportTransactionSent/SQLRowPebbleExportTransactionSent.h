#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebbleExportTransactionSent.h"
//**********************************************************************

class CSQLRowPebbleExportTransactionSent
{
public:
	CSQLRowPebbleExportTransactionSent();
	
private:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	__int64 GetTranLineID() { return m_nTranLineID; }
	
public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetTranLineID(__int64 n) { vdtr.SetTranLineID(n, m_nTranLineID); }

private:
	__int64 m_nLineID;
	__int64 m_nTranLineID;
	
private:
	CSQLFieldValidatorPebbleExportTransactionSent vdtr;
};

//**********************************************************************
