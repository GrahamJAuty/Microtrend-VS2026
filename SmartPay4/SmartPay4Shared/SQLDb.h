#pragma once
//**********************************************************************
#include "afxdb.h"
#include "odbcinst.h"
#include "sqlext.h"
//**********************************************************************
#include "SQLDefines.h"
//**********************************************************************

class CSQLDb
{
public:
	CSQLDb();
	~CSQLDb();

public:
	void Connect(int& nErrorContext,bool bCanCreate);
	int GetDatabaseVersion(int& nError);
	void Close();

public:
	bool IsInUse() { return m_bInUse;  }
	void SetInUseFlag(bool b) { m_bInUse = b; }

public:
	int BackupDatabase();
	int RestoreDatabase();

public:
#ifndef SYSTEMTYPE_SERVER
#ifndef SYSTEMTYPE_BACKGROUND
	void CreateDatabaseSchema(int& nErrorContext, int& nErrorNumber);
#endif
#endif

private:
	bool OpenDatabase(CString strName);
	bool CreateSmartPayDb();
	void FixDatabaseOptions();

public:
	CDatabase* GetDb() { return &m_database; }

private:
	CDatabase m_database;

private:
	bool m_bDoneConnect;
	bool m_bInUse;
	CString m_strRootName;
	CString m_strRootFilename;
	CString m_strSQLFolder;
	CString m_strDatabasePath;
	CString m_strLogPath;
};

//**********************************************************************

