#pragma once
//*********************************************************************
#ifndef POSTRAY_UTILITY
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

struct CLoyaltyLockRequest
{
public:
	CLoyaltyLockRequest();

public:
	bool m_bLockServerOuter;
	bool m_bLockServerInner;
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
	void Init( bool bLoyalty );
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
	CString GetProgramPath(){ return m_strSyssetPath + m_strSubPathProgram; }
	CString GetServicePath(){ return GetProgramPath() + m_strSubPathService; }
	CString GetServerOuterPath(){ return GetServicePath() + m_strSubPathServerOuter; }
	CString GetServerInnerPath(){ return GetServicePath() + m_strSubPathServerInner; }
	CString GetBackgroundPath(){ return GetServicePath() + m_strSubPathBackground; }

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
	CString m_strSyssetPath;
	CString m_strSubPathProgram;
	CString m_strSubPathService;
	CString m_strSubPathServerOuter;
	CString m_strSubPathServerInner;
	CString m_strSubPathBackground;
	
private:
	CWinThread* m_pSmartPaySemaphoreThread;
	CWinThread* m_pSmartPaySemaphoreTidyUpThread;
	CSmartPaySemaphoreThreadInfo m_SmartPaySemaphoreThreadInfo;
	CSmartPaySemaphoreTidyUpThreadInfo m_SmartPaySemaphoreTidyUpThreadInfo;

	friend class CLocalSmartPaySemaphore;
	friend class CLocalLoyaltySemaphore;
};

//*********************************************************************

class CLocalSmartPaySemaphore
{
public:
	CLocalSmartPaySemaphore();
	~CLocalSmartPaySemaphore();
	
public:
	void SetRequest( CSmartPayLockRequest Request );
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

class CLocalLoyaltySemaphore
{
public:
	CLocalLoyaltySemaphore();
	~CLocalLoyaltySemaphore();
	
public:
	void SetRequest( CLoyaltyLockRequest Request );
	void Lock();
	bool GotLock();
	void Unlock();
	
private:
	int m_nLockNumber;
	bool m_bSetRequest;
	bool m_bGotLock;
	bool m_bDoneUnlock;

private:
	CLoyaltyLockRequest m_Request;
};

//*********************************************************************
extern CSmartPaySemaphore SmartPaySemaphore;
extern CSmartPaySemaphore LoyaltySemaphore;
//*********************************************************************
#endif
//*********************************************************************
