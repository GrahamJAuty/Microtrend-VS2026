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
//*********************************************************************

struct CLoyaltyLockRequest
{
public:
	CLoyaltyLockRequest()
	{
		m_bWantLockServerInner = FALSE;
		m_bWantLockServerOuter = FALSE;
		m_bGotLockServerInner = FALSE;
		m_bGotLockServerOuter = FALSE;
		m_bOneShot = FALSE;
	}

public:
	bool m_bWantLockServerInner;
	bool m_bWantLockServerOuter;
	bool m_bGotLockServerInner;
	bool m_bGotLockServerOuter;
	bool m_bOneShot; 
};

//*********************************************************************

class CLoyaltyLockManager
{
public:
	CLoyaltyLockManager();
	~CLoyaltyLockManager();

public:
	int AcquireLocks(CLoyaltyLockRequest& LockRequest, CWnd* pParentWnd = NULL);
	void LogError(int nError);

private:
	int AcquireSingleLock(CString strLockName, CString strPromptText, int nErrorOffset, CWnd* pParentWnd);
	int AcquireLockSQL(CString strLockName);

private:
	int m_nConnection;
	CDatabase* m_pDb;
	CLoyaltyLockRequest m_LockRequest;

private:
	static int m_nLastError;
	static long long m_nLastErrorTime;
};

//*********************************************************************
