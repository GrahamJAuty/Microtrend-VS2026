#pragma once
//**********************************************************************
#include "SQLFieldValidatorCCNo.h"
//**********************************************************************
#define CCNO_TYPE_UNKNOWN 0
#define CCNO_TYPE_SERVER 1
#define CCNO_TYPE_PC 2
#define CCNO_TYPE_POS 3
#define CCNO_TYPE_EXTERNAL 4
//**********************************************************************

namespace CCNoTypeNames
{
	const char* const Server = "server";
	const char* const PC = "pc";
	const char* const POS = "epos";
	const char* const External = "external";
	const char* const Other = "other";
}

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

