#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebblePaymentType.h"
//**********************************************************************

class CSQLRowPebblePaymentType
{
public:
	CSQLRowPebblePaymentType();
	
private:
	void Reset();
	
public:
	int GetSPOSPaymentType() { return m_nSPOSPaymentType; }
	CString GetPebblePaymentName() { return m_strPebblePaymentName; }
	int GetPebbleTranType() { return m_nPebbleTranType; }

public:
	void SetSPOSPaymentType(int n) { vdtr.SetSPOSPaymentType(n, m_nSPOSPaymentType); }
	void SetPebblePaymentName( CString str) { vdtr.SetPebblePaymentName(str, m_strPebblePaymentName); }
	void SetPebbleTranType(int n) { vdtr.SetPebbleTranType(n, m_nPebbleTranType); }
	
public:
	CString GetPebbleTranTypeName() { return GetPebbleTranTypeName(m_nPebbleTranType); }

public:
	static CString GetPebbleTranTypeName(int n);
	static bool IsValidSPOSPaymentType(int nType, bool bTopup);

private:
	int m_nSPOSPaymentType;
	CString m_strPebblePaymentName;
	int m_nPebbleTranType;

private:
	CSQLFieldValidatorPebblePaymentType vdtr;
};

//**********************************************************************

