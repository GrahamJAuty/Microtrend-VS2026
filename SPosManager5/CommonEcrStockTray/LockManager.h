#pragma once
/**********************************************************************/
#define COMMS_ACTION_NONE 0
#define COMMS_ACTION_DISABLE 1
#define COMMS_ACTION_PAUSE 2
/**********************************************************************/
#define LOCK_RESULT_OK 0
#define LOCK_RESULT_FAIL_USERS 1
#define LOCK_RESULT_FAIL_COMMS 2
#define LOCK_RESULT_FAIL_POSTRAY 3
#define LOCK_RESULT_FAIL_LOYALTY 4
/**********************************************************************/
#define LOCK_RESULT_RELEASED 998
#define LOCK_RESULT_INACTIVE 999
/**********************************************************************/
#include "WaitLockDlg.h"
/**********************************************************************/

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

struct CLockManagerInfo
{
public:
	CLockManagerInfo();

public:
	bool m_bLockAllUsers;
	CString m_strLockAllUsersPrompt;
	int m_nCommsAction;
	bool m_bStopComms;
	bool m_bStopCommsPMSExemption;
	CString m_strStopCommsPrompt;
	bool m_bStopPosTray;
	bool m_bStopLoyalty;
	CWnd* m_pParentWnd;

public:
	bool m_bLoyaltyWasRunning;
	bool m_bSmartPayWasRunning;
	bool m_bSmartBackWasRunning;
};

/**********************************************************************/

class CLockManager
{
public:
	CLockManager( CLockManagerInfo& infoLock );
	CLockManager( CLockManagerInfo& infoLock, CSmartPayLockRequest m_SmartPayLockRequest );
	~CLockManager();

public:
	bool GetLock();
	void ReleaseLock();

public:
	int GetLockResult(){ return m_nLockResult; }

private:
	void LoyaltyFail( bool& bFlag );
	void UnlockComms();

private:
	CLockManagerInfo& m_infoLock;
	CWaitLoyaltyPauseDlg m_WaitLoyaltyDlg;
	CWaitPosTrayDlg m_WaitPosTrayDlg;
	CWaitSmartPayServerPauseDlg m_WaitSmartPayServerDlg;
	CWaitSmartPayBgndPauseDlg m_WaitSmartPayBgndDlg;
	CWaitSmartPayServicePauseDlg m_WaitSmartPayServiceDlg;
	CWaitLoyaltyServicePauseDlg m_WaitLoyaltyServiceDlg;

private:
	int m_nLockResult;

private:
	CLocalSmartPaySemaphore m_SmartPaySemaphore;
	CLocalLoyaltySemaphore m_LoyaltySemaphore;
};

/**********************************************************************/
#else
/**********************************************************************/

struct CLockManagerInfo
{
public:
	CLockManagerInfo(){};
};

/**********************************************************************/

class CLockManager
{
public:
	CLockManager( CLockManagerInfo& info ){ m_nLockResult = LOCK_RESULT_INACTIVE; }
	~CLockManager(){};

public:
	bool GetLock();
	void ReleaseLock();

public:
	int GetLockResult(){ return m_nLockResult; }

private:
	int m_nLockResult;
};

/**********************************************************************/
#endif
/**********************************************************************/
