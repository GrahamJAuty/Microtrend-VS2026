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
	int GetExternalVersion1() { return m_nExternalVersion1; }

public:
	void SetLineID( int n ) { vdtr.SetLineID(n, m_nLineID); }
	void SetVersion(int n) { vdtr.SetVersion(n, m_nVersion); }
	void SetExternalVersion1(int n) { vdtr.SetExternalVersion1(n, m_nExternalVersion1); }

private:
	int m_nLineID;
	int m_nVersion;
	int m_nExternalVersion1;

private:
	CSQLFieldValidatorDbVersion vdtr;
};

//**********************************************************************

