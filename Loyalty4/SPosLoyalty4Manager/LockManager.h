#pragma once
/**********************************************************************/
#define COMMS_ACTION_NONE 0
#define COMMS_ACTION_DISABLE 1
#define COMMS_ACTION_PAUSE 2
/**********************************************************************/
#define LOCK_RESULT_OK 0
#define LOCK_RESULT_FAIL_USERS 1
#define LOCK_RESULT_FAIL_LOYALTY 4
/**********************************************************************/
#define LOCK_RESULT_RELEASED 998
#define LOCK_RESULT_INACTIVE 999
/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyLockManager.h"
/**********************************************************************/
#include "WaitLockDlg.h"
/**********************************************************************/

struct CLockManagerInfo
{
public:
	CLockManagerInfo();

public:
	bool m_bLockAllUsers;
	CString m_strLockAllUsersPrompt;
	CString m_strStopCommsPrompt;
	bool m_bStopLoyalty;
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
	void LoyaltyFail( bool& bFlag );
	
private:
	CLockManagerInfo& m_infoLock;
	CWaitLoyaltyServicePauseDlg m_WaitLoyaltyServiceDlg;
	CLoyaltyLockManager m_LoyaltyLockManager;
	
private:
	int m_nLockResult;
};

/**********************************************************************/
