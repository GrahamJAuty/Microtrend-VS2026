#pragma once
//*********************************************************************
#include "SQLDb.h"
//*********************************************************************
#define LOCKMANAGER_OK 0
#define LOCKMANAGER_ERROR_NOLOCKS 1
#define LOCKMANAGER_ERROR_NOCONNECT 2
#define LOCKMANAGER_ERROR_INNERLOCK_SQL 3
#define LOCKMANAGER_ERROR_INNERLOCK_TIMEOUT 4
#define LOCKMANAGER_ERROR_INNERLOCK_CANCELLED 5
#define LOCKMANAGER_ERROR_INNERLOCK_DEADLOCK 6
#define LOCKMANAGER_ERROR_INNERLOCK_UNKNOWN 7
#define LOCKMANAGER_ERROR_OUTERLOCK_SQL 8
#define LOCKMANAGER_ERROR_OUTERLOCK_TIMEOUT 9
#define LOCKMANAGER_ERROR_OUTERLOCK_CANCELLED 10
#define LOCKMANAGER_ERROR_OUTERLOCK_DEADLOCK 11
#define LOCKMANAGER_ERROR_OUTERLOCK_UNKNOWN 12
#define LOCKMANAGER_ERROR_BGNDLOCK_SQL 13
#define LOCKMANAGER_ERROR_BGNDLOCK_TIMEOUT 14
#define LOCKMANAGER_ERROR_BGNDLOCK_CANCELLED 15
#define LOCKMANAGER_ERROR_BGNDLOCK_DEADLOCK 16
#define LOCKMANAGER_ERROR_BGNDLOCK_UNKNOWN 17
#define LOCKMANAGER_ERROR_PEBBLE1_SQL 18
#define LOCKMANAGER_ERROR_PEBBLE1_TIMEOUT 19
#define LOCKMANAGER_ERROR_PEBBLE1_CANCELLED 20
#define LOCKMANAGER_ERROR_PEBBLE1_DEADLOCK 21
#define LOCKMANAGER_ERROR_PEBBLE1_UNKNOWN 22
//*********************************************************************

struct CSmartPayLockRequest
{
public:
	CSmartPayLockRequest(){}

public:
	bool m_bWantLockServerInner = FALSE;
	bool m_bWantLockServerOuter = FALSE;
	bool m_bWantLockBgnd = FALSE;
	bool m_bWantLockPebble = FALSE;
	bool m_bGotLockServerInner = FALSE;
	bool m_bGotLockServerOuter = FALSE;
	bool m_bGotLockBgnd = FALSE;
	bool m_bGotLockPebble = FALSE;
	bool m_bOneShot = FALSE;
};

//*********************************************************************

class CSmartPayLockManager
{
public:
	CSmartPayLockManager();
	~CSmartPayLockManager();

public:
	int AcquireLocks(CSmartPayLockRequest& LockRequest, CWnd* pParentWnd = NULL);
	void LogError(int nError);

private:
	int AcquireSingleLock(CString strLockName, CString strPromptText, int nErrorOffset, CWnd* pParentWnd);	
	int AcquireLockSQL(CString strLockName);

private:
	int m_nConnection = -1;
	CDatabase* m_pDb = nullptr;
	CSmartPayLockRequest m_LockRequest;

private:
	static int m_nLastError;
	static long long m_nLastErrorTime;
};

//*********************************************************************
