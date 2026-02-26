#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "RealTimeCommsThread.h"
/**********************************************************************/

class CCashRSPClearBufferThreadManager
{
public:
	CCashRSPClearBufferThreadManager();
	void Init();
	
public:
	int GetTransactionCount(){ return m_RealTimeCommsThreadInfo.m_nTransactionCount; }
	void ClearTransactionCount(){ m_RealTimeCommsThreadInfo.m_nTransactionCount = 0; }
	void SetForceQuitFlag(){ m_RealTimeCommsThreadInfo.m_bForceQuit = TRUE; }

public:
	bool ThreadIsNull(){ return NULL == m_pRealTimeCommsThread; }
	void PrepareThread( C5000SiteInfo& siteInfo );
	void RunThread();
	DWORD GetThreadExitCode();
	void DeleteThread();
	void TerminateThread();

private:
	CWinThread* m_pRealTimeCommsThread;
	CRealTimeCommsThreadInfo m_RealTimeCommsThreadInfo;
};

/**********************************************************************/
extern CCashRSPClearBufferThreadManager CashRSPClearBufferThreadManager;
/**********************************************************************/
#endif
/**********************************************************************/
