#pragma once
//*********************************************************************
#include "SmartPaySemaphoreThread.h"
#include "SmartPaySemaphoreTidyUpThread.h"
//*********************************************************************

struct CSmartPayLockRequest
{
public:
	CSmartPayLockRequest();

public:
	bool m_bLockServerOuter;
	bool m_bLockServerInner;
	bool m_bLockBackground;
};

//*********************************************************************

struct CSmartPaySemaphoreLockDetails
{
public:
	CSmartPaySemaphoreLockDetails();

public:
	CString GetActiveFilename() { return m_strLockFolder + "\\Active.dat"; }
	CString GetLockFilename() { return m_strLockFolder + "\\Lock.dat"; }
	CString GetDeletionFolder() { return m_strLockFolder + "_xxx"; }

public:
	bool m_bIsLocked;
	bool m_bZombieCheck;
	ULONGLONG m_ZombieCheckTime;
	int m_nLockNumber;
	CString m_strLockFolder;
	CString m_strLockType;
};

//*********************************************************************

class CSmartPaySemaphore
{
public:
	CSmartPaySemaphore();
	~CSmartPaySemaphore();

public:
	void Init();
	void ResetZombieCheck();

private:
	int GetNextLockNumber(){ return m_nLockNumberNext++; }
	
private:
	bool LockServerOuter( int nLockNumber );
	bool LockServerInner( int nLockNumber );
	bool LockBackground( int nLockNumber );
	bool AcquireLock(CSmartPaySemaphoreLockDetails& LockDetails, int nLockNumber, bool& bThreadSeenLock, bool& bThreadGotLock);
	bool CreateLockFolder(CSmartPaySemaphoreLockDetails& LockDetails);

private:
	void UnlockServerOuter( int nLockNumber );
	void UnlockServerInner( int nLockNumber );
	void UnlockBackground( int nLockNumber );
	void ReleaseLock(int nLockNumber, CSmartPaySemaphoreLockDetails& LockDetails, bool& bThreadGotLock);
	void DeleteLockFolder(CSmartPaySemaphoreLockDetails& LockDetails);

private:
	void Diagnostic(CSmartPaySemaphoreLockDetails& LockDetails, CString strMsg);

private:
	CSmartPaySemaphoreLockDetails m_lockServerOuter;
	CSmartPaySemaphoreLockDetails m_lockServerInner;
	CSmartPaySemaphoreLockDetails m_lockBackground;

private:
	bool m_bDoneInit;	
	int m_nLockNumberNext;
	
private:
	CWinThread* m_pSmartPaySemaphoreThread;
	CWinThread* m_pSmartPaySemaphoreTidyUpThread;
	CSmartPaySemaphoreThreadInfo m_SmartPaySemaphoreThreadInfo;
	CSmartPaySemaphoreTidyUpThreadInfo m_SmartPaySemaphoreTidyUpThreadInfo;

	friend class CLocalSmartPaySemaphore;
	friend class CLocalSmartPaySemaphoreBackup;

private:
	static __int64 m_nMostRenameAttempts;
	static int m_nRenameEventsSinceLastLog;
	static __int64 m_nRenameAttemptsSinceLastLog;
};

//*********************************************************************

class CLocalSmartPaySemaphore
{
public:
	CLocalSmartPaySemaphore();
	CLocalSmartPaySemaphore( CSmartPayLockRequest Request, CWnd* pParent = NULL );
	~CLocalSmartPaySemaphore();

public:
	void Init(CSmartPayLockRequest Request, CWnd* pParent = NULL);	

public:
	bool GotLock();
	void Unlock();
	
private:
	int m_nLockNumber;
	bool m_bGotLock;
	bool m_bDoneUnlock;
	bool m_bDoneInit;
};

//*********************************************************************

class CLocalSmartPaySemaphoreBackup
{
public:
	CLocalSmartPaySemaphoreBackup();
	~CLocalSmartPaySemaphoreBackup();

public:
	void SetRequest(CSmartPayLockRequest Request);
	void Lock();
	bool GotLock();
	void Unlock();

private:
	int m_nLockNumber;
	bool m_bSetRequest;
	bool m_bGotLock;
	bool m_bDoneUnlock;

private:
	CSmartPayLockRequest m_Request;
};

//*********************************************************************
extern CSmartPaySemaphore SmartPaySemaphore;
//*********************************************************************
