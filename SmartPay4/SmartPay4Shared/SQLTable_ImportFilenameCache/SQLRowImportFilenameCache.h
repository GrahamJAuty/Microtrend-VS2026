#pragma once
//**********************************************************************
#include "SQLFieldValidatorImportFilenameCache.h"
//**********************************************************************

class CSQLRowImportFilenameCache
{
public:
	CSQLRowImportFilenameCache();
	
private:
	void Reset();
	
public:
	__int64 GetLineID() { return m_nLineID; }
	int GetType() { return m_nType; }
	CString GetFilename() { return m_strFilename; }
	CString GetDateTime() { return m_strDateTime; }

public:
	void SetLineID(__int64 n) { vdtr.SetLineID(n, m_nLineID); }
	void SetType(int n) { vdtr.SetType(n, m_nType); }
	void SetFilename(CString str) { vdtr.SetFilename(str, m_strFilename); }
	void SetDateTime(CString str) { vdtr.SetDateTime(str, m_strDateTime); }
	
private:
	__int64 m_nLineID;
	int m_nType;
	CString m_strFilename;
	CString m_strDateTime;

private:
	CSQLFieldValidatorImportFilenameCache vdtr;
};

//**********************************************************************
