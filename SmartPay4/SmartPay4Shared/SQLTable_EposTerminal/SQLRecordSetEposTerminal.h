#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowEposTerminal.h"
//**********************************************************************

struct RSParams_EposTerminal_NormalNoParams
{
public:
	RSParams_EposTerminal_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_EposTerminal_NormalByTerminalNo
{
public:
	RSParams_EposTerminal_NormalByTerminalNo(int n)
		: nTerminalNo(n) {}

public:
	int nTerminalNo;	
};

//**********************************************************************

class CSQLRecordSetMetadataEposTerminal : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataEposTerminal(bool bForExistenceCheck = FALSE);
	void AddParamsByTerminalNo(const int nTerminalNo);	

public:
	int m_nTerminalNo;
	int m_nTerminalType;
	CString m_strTerminalName;
	CString m_strLastUsedDate;
	CString m_strLastUsedTime;
	int m_nCCNo;
	__int64 m_nUserID;
	CString m_strCardValue;
	int m_nTotalCounter;
	CString m_strTotalInDrawer;
	BOOL m_bDisable;
	BOOL m_bTerminalOn;
	BOOL m_bInService;
	BOOL m_bDoorOpen;
};

//**********************************************************************

class CSQLRecordSetEposTerminal : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetEposTerminal(CDatabase* pDatabase, RSParams_EposTerminal_NormalNoParams params);
	CSQLRecordSetEposTerminal(CDatabase* pDatabase, RSParams_EposTerminal_NormalByTerminalNo params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromEposTerminalRow(CSQLRowEposTerminal& RowEP);
	void SaveToEposTerminalRow(CSQLRowEposTerminal& RowEP);

public:
	bool StepSelectAll(CSQLRowEposTerminal& RowEP);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataEposTerminal m_MD;
};

//**********************************************************************

