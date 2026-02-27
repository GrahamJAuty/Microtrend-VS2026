/**********************************************************************/
#include "SystemData.h"
/**********************************************************************/
#include "PostTransactionThreadManager.h"
/**********************************************************************/

CPostTransactionThreadManager::CPostTransactionThreadManager(int nAction)
{
	m_nAction = nAction;
}

/**********************************************************************/

CPostTransactionThreadManager::~CPostTransactionThreadManager()
{
}

/**********************************************************************/

void CPostTransactionThreadManager::CheckThread()
{
	bool bAlreadyRunning = FALSE;

	if (m_pThread != nullptr)
	{
		if (WaitForSingleObject(m_pThread->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			delete m_pThread;
			m_pThread = nullptr;
			m_bThreadDone = TRUE;
			m_bBoolResult = m_ThreadInfo.m_bBoolResult;
			m_nIntResult = m_ThreadInfo.m_nIntResult;
		}
	}

	if ((FALSE == bAlreadyRunning) && (FALSE == m_bThreadDone))
	{
		m_ThreadInfo.m_nAction = m_nAction;
		m_ThreadInfo.m_strLoyLogTransFilename = System.m_strLoyLogTransFilename;
		m_ThreadInfo.m_strOfflineFilename = System.m_strOfflineLoyLogFilename;
		m_ThreadInfo.m_strPayLinkTTLFilename = System.m_strPayLinkTTLFilename;
		m_ThreadInfo.m_bBoolResult = FALSE;
		m_ThreadInfo.m_nIntResult = 0;

		m_pThread = AfxBeginThread(CPostTransactionThread::DoWork, &m_ThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if (m_pThread != nullptr)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

/**********************************************************************/

void CPostTransactionThreadManager::ShutdownThread()
{
	if (m_pThread == NULL)
	{
		TRACE("Post Transaction thread not running\n");
		return;
	}

	//m_ThreadInfo.m_bWantQuit = TRUE;
	if (WaitForSingleObject(m_pThread->m_hThread, 10000) == WAIT_OBJECT_0)
	{
		delete m_pThread;
		m_pThread = NULL;
		TRACE("Shutdown post transaction thread\n");
	}
	else
	{
		TRACE("Failed to shutdown post transaction thread\n");
	}
}

/**********************************************************************/

