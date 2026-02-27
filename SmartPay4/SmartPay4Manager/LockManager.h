#pragma once
/**********************************************************************/
#define COMMS_ACTION_NONE 0
#define COMMS_ACTION_DISABLE 1
#define COMMS_ACTION_PAUSE 2
/**********************************************************************/
#define LOCK_RESULT_OK 0
#define LOCK_RESULT_FAIL_USERS 1
#define LOCK_RESULT_FAIL_SMARTPAY 4
/**********************************************************************/
#define LOCK_RESULT_RELEASED 998
#define LOCK_RESULT_INACTIVE 999
/**********************************************************************/
#include "WaitLockDlg.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayLockManager.h"
/**********************************************************************/

struct CLockManagerInfo
{
public:
	CLockManagerInfo();

public:
	bool m_bLockAllUsers;
	CString m_strLockAllUsersPrompt;
	CString m_strStopCommsPrompt;
	bool m_bStopSmartPay;
	CWnd* m_pParentWnd;
};

/**********************************************************************/

class CLockManager
{
public:
	CLockManager( CLockManagerInfo& infoLock );
	~CLockManager();

public:
	bool GetLock();
	void ReleaseLock();

public:
	int GetLockResult(){ return m_nLockResult; }

private:
	void SmartPayFail( bool& bFlag );
	
private:
	CLockManagerInfo& m_infoLock;
	CWaitSmartPayServicePauseDlg m_WaitSmartPayServiceDlg;
	CSmartPayLockManager m_SmartPayLockManager;
	
private:
	int m_nLockResult;
};

/**********************************************************************/
