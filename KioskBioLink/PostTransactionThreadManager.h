#pragma once
/**********************************************************************/
#include "Defines.h"
#include "PostTransactionThread.h"
/**********************************************************************/

class CPostTransactionThreadManager
{
public:
	CPostTransactionThreadManager(int nAction);
	~CPostTransactionThreadManager();

public:
	void ShutdownThread();	
	void CheckThread();

public:
	bool IsThreadDone() { return m_bThreadDone; }
	bool GetBoolResult() { return m_bBoolResult; }
	int GetIntResult() { return m_nIntResult; }
	
private:
	CPostTransactionThreadInfo m_ThreadInfo;
	CWinThread* m_pThread = nullptr;

private:
	int m_nAction = nTRANPOST_ACTION_SENDNORMAL;

private:
	bool m_bThreadDone = FALSE;
	bool m_bBoolResult = FALSE;
	int m_nIntResult = 0;
};

/**********************************************************************/
