/**********************************************************************/
#include "BusyFileUpdateThreadManager.h"
/**********************************************************************/

CBusyFileUpdateThreadManager::CBusyFileUpdateThreadManager()
{
	m_pThread = NULL;
}

/**********************************************************************/

CBusyFileUpdateThreadManager::~CBusyFileUpdateThreadManager()
{
}

/**********************************************************************/

void CBusyFileUpdateThreadManager::CheckThread()
{
	bool bAlreadyRunning = FALSE;

	if (m_pThread != NULL)
	{
		if (WaitForSingleObject(m_pThread->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			delete m_pThread;
			m_pThread = NULL;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		m_ThreadInfo.m_bWantQuit = FALSE;
		m_ThreadInfo.m_strBusyFilename = Filenames.GetBgndServiceBusyTimeFilename();

		m_pThread = AfxBeginThread(CBusyFileUpdateThread::DoWork, &m_ThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if (m_pThread != NULL)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

/**********************************************************************/

void CBusyFileUpdateThreadManager::ShutdownThread()
{
	if (m_pThread == NULL)
	{
		TRACE("Background Processor busy thread not running\n");
		return;
	}

	m_ThreadInfo.m_bWantQuit = TRUE;
	if (WaitForSingleObject(m_pThread->m_hThread, 10000) == WAIT_OBJECT_0)
	{
		delete m_pThread;
		m_pThread = NULL;
		TRACE("Shutdown background processor busy thread\n");
	}
	else
	{
		TRACE("Failed to shutdown background processor busy thread\n");
	}
}

/**********************************************************************/

