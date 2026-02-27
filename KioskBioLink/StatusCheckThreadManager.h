#pragma once
/**********************************************************************/
#include "Defines.h"
#include "Status.h"
#include "StatusCheckThread.h"
/**********************************************************************/

class CStatusCheckThreadManager
{
public:
	CStatusCheckThreadManager(CStatus& status, int nAction);
	~CStatusCheckThreadManager();

public:
	void ShutdownThread();	
	void CheckThread();

public:
	bool IsThreadDone() { return m_bThreadDone; }
	bool GetBoolResult() { return m_bBoolResult; }
	int GetIntResult() { return m_nIntResult; }
	
private:
	CStatusCheckThreadInfo m_ThreadInfo;
	CWinThread* m_pThread = nullptr;

private:
	CStatus& m_Status;
	int m_nAction = nWAITSTATUS_ACTION_CHECKSERVERONLINE;

private:
	bool m_bThreadDone = FALSE;
	bool m_bBoolResult = FALSE;
	int m_nIntResult = 0;
};

/**********************************************************************/
