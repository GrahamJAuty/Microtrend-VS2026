#pragma once
//**********************************************************************
#include "SQLFieldValidatorDbVersion.h"
//**********************************************************************

class CSQLRowDbVersion
{
public:
	CSQLRowDbVersion();
	
private:
	void Reset();
	
public:
	int GetLineID() { return m_nLineID; }
	int GetVersion() { return m_nVersion; }

public:
	void SetLineID( int n ) { vdtr.SetLineID(n, m_nLineID); }
	void SetVersion(int n) { vdtr.SetVersion(n, m_nVersion); }

private:
	int m_nLineID;
	int m_nVersion;

private:
	CSQLFieldValidatorDbVersion vdtr;
};

//**********************************************************************

