#pragma once
//**********************************************************************
#include "SQLFieldValidatorCCNo.h"
//**********************************************************************
#define CCNO_TYPE_UNKNOWN 0
#define CCNO_TYPE_SERVER 1
#define CCNO_TYPE_PC 2
#define CCNO_TYPE_POS 3
#define CCNO_TYPE_KIOSK 4
#define CCNO_TYPE_EODBALANCE 5
#define CCNO_TYPE_VENDING 6
#define CCNO_TYPE_EXTERNAL 7
//**********************************************************************

namespace CCNoTypeNames
{
	const char* const Server = "Server";
	const char* const PC = "PC";
	const char* const POS = "POS";
	const char* const Kiosk = "Kiosk";
	const char* const EODBalance = "EODBalance";
	const char* const Vending = "Vending";
	const char* const External = "External";
	const char* const Other = "Other";
};

//**********************************************************************

class CSQLRowCCNo
{
public:
	CSQLRowCCNo();
	
private:
	void Reset();
	
public:
	int GetType() { return m_nType; }
	CString GetTypeString();
	int GetNumber() { return m_nNumber; }
	
public:
	void SetType(int n) { vdtr.SetTypeInt(n, m_nType); }
	void SetTypeString(CString strType);
	void SetNumber(int n) { vdtr.SetNumber(n, m_nNumber); }

public:
	void IncrementCCNo();

private:
	int m_nType = CCNO_TYPE_UNKNOWN;
	int m_nNumber = 0;

private:
	CSQLFieldValidatorCCNo vdtr;
};

//**********************************************************************

