/**********************************************************************/
#include "SocketThreadManager.h"
/**********************************************************************/

CSocketThreadManager::CSocketThreadManager()
{
}

/**********************************************************************/

CSocketThreadManager::~CSocketThreadManager()
{
	for (int n = 0; n < m_arrayWorkerThreadInfo.GetCount(); n++)
	{
		CSocketWorkerThreadInfo* pWorkerInfo = m_arrayWorkerThreadInfo[n];
		if (pWorkerInfo != nullptr)
		{
			if (pWorkerInfo->m_pClient != nullptr)
			{
				delete pWorkerInfo->m_pClient;
				pWorkerInfo->m_pClient = nullptr;
			}

			delete pWorkerInfo;
		}
	}
}

/**********************************************************************/

void CSocketThreadManager::StartListenerThread()
{
	bool bAlreadyRunning = FALSE;

	if (m_pListenerThread != NULL)
	{
		if (WaitForSingleObject(m_pListenerThread->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		m_pListenerThread = AfxBeginThread(CSocketListenerThread::DoWork, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (m_pListenerThread != nullptr)
		{
			m_pListenerThread->m_bAutoDelete = FALSE;
			m_pListenerThread->ResumeThread();
		}
	}
}

/**********************************************************************/

void CSocketThreadManager::CreateNewWorkerThread(CkSocket* pClient)
{
	CSocketWorkerThreadInfo* pWorkerInfo = new CSocketWorkerThreadInfo();
	pWorkerInfo -> m_bLogAction = TRUE;
	pWorkerInfo -> m_pClient = pClient;
	pWorkerInfo -> m_pThread = nullptr;

	m_arrayWorkerThreadInfo.Add(pWorkerInfo);

	CWinThread* pWorkerThread = AfxBeginThread(CSocketWorkerThread::DoWork, pWorkerInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	if (pWorkerThread != nullptr)
	{
		pWorkerInfo->m_pThread = pWorkerThread;
		pWorkerThread->m_bAutoDelete = FALSE;
		pWorkerThread->ResumeThread();
	}
}

/**********************************************************************/

void CSocketThreadManager::CleanUpWorkerThreads()
{
	for (int i = 0; i < m_arrayWorkerThreadInfo.GetCount(); i++)
	{
		CSocketWorkerThreadInfo* pWorkerInfo = m_arrayWorkerThreadInfo[i];

		if (pWorkerInfo != nullptr)
		{
			if (pWorkerInfo->m_bWantCleanup == TRUE)
			{
				if (pWorkerInfo->m_pThread != nullptr)
				{
					if (WaitForSingleObject(pWorkerInfo->m_pThread->m_hThread, 0) == WAIT_OBJECT_0)
					{
						if (CleanUpWorkerThread(i, FALSE) == TRUE)
						{
							m_arrayWorkerThreadInfo.RemoveAt(i--);
						}
					}
				}
			}
		}
		else
		{
			// This should never happen, but if it does, remove the null entry from the array.
			CString strMsg = "";
			strMsg.Format("Worker thread info at index %d is null", i);
			LogThread(strMsg);
			m_arrayWorkerThreadInfo.RemoveAt(i--);
		}
	}
}

/**********************************************************************/

void CSocketThreadManager::ShutdownThreads()
{
	m_bQuitListenerThread = TRUE;

	for (int i = 0; i < m_arrayWorkerThreadInfo.GetCount(); i++)
	{
		CSocketWorkerThreadInfo* pWorkerInfo = m_arrayWorkerThreadInfo[i];
		pWorkerInfo->m_bWantQuit = TRUE;

		if (pWorkerInfo->m_pClient != nullptr)
		{
			pWorkerInfo->m_pClient->Close(200);
		}
	}

	bool bThreadMaybeStillRunning = TRUE;

	ULONGLONG dwStartTime = GetTickCount64();
	bool bTimeoutOccurred = FALSE;

	while (TRUE == bThreadMaybeStillRunning)
	{
		if (GetTickCount64() - dwStartTime >= 5000)
		{
			bTimeoutOccurred = TRUE;
			LogThread("Timeout occurred while waiting for threads to shut down");
			break;
		}

		bThreadMaybeStillRunning = FALSE;
		
		if (m_pListenerThread != nullptr)
		{
			if (WaitForSingleObject(m_pListenerThread->m_hThread, 0) != WAIT_OBJECT_0)
			{
				bThreadMaybeStillRunning = TRUE;
			}
			else
			{
				delete m_pListenerThread;
				m_pListenerThread = nullptr;
				LogThread("Shutdown Socket Listener Thread\n");
			}
		}

		for (int i = 0; i < m_arrayWorkerThreadInfo.GetCount(); i++)
		{
			CSocketWorkerThreadInfo* pWorkerInfo = m_arrayWorkerThreadInfo[i];
			
			if (pWorkerInfo != nullptr)
			{		
				if (pWorkerInfo->m_pThread != nullptr)
				{
					if (WaitForSingleObject(pWorkerInfo->m_pThread->m_hThread, 0) != WAIT_OBJECT_0)
					{
						bThreadMaybeStillRunning = TRUE;
					}
					else if (CleanUpWorkerThread(i, TRUE) == TRUE)
					{
						m_arrayWorkerThreadInfo.RemoveAt(i--);
					}
					else
					{
						bThreadMaybeStillRunning = TRUE;
					}
				}
			}
		}
	}
}
		
/**********************************************************************/

bool CSocketThreadManager::CleanUpWorkerThread(int nIndex, bool bShutdown)
{
	bool bResult = FALSE;

	if ((nIndex >= 0) && (nIndex < m_arrayWorkerThreadInfo.GetCount()))
	{
		CString strReason = bShutdown ? "Shutdown" : "Clean up";

		CSocketWorkerThreadInfo* pWorkerInfo = m_arrayWorkerThreadInfo[nIndex];
		if (pWorkerInfo != nullptr)
		{
			CString strMsg = "";
			
			strMsg.Format("%s worker thread for client %d", (const char*)strReason, nIndex);
			LogThread(strMsg);

			if (pWorkerInfo->m_pClient != nullptr)
			{

				strMsg.Format("%s client socket for client %d", (const char*)strReason, nIndex);
				LogThread(strMsg);
				delete pWorkerInfo->m_pClient;
				pWorkerInfo->m_pClient = nullptr;
			}

			if (pWorkerInfo->m_pThread != nullptr)
			{
				strMsg.Format("%s worker thread for client %d", (const char*)strReason, nIndex);
				LogThread(strMsg);
				delete pWorkerInfo->m_pThread;
				pWorkerInfo->m_pThread = nullptr;
			}


			strMsg.Format("%s worker info for client %d", (const char*)strReason, nIndex);
			LogThread(strMsg);

			delete pWorkerInfo;
			
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

void CSocketThreadManager::LogThread(CString strMsg)
{
	CString strDate = "";
	CString strTime = "";
	SolutionGlobalFunctions::GetMessageLogDateTime(strDate, strTime);

	CSSFile fileLog;

	if (fileLog.Open(".\\SOCKETLOG.TXT", "ab") == TRUE)
	{
		CString strLog = "";
		strLog.Format("%s %s - %s",
			(const char*)strDate,
			(const char*)strTime,
			(const char*)strMsg);

		fileLog.WriteLine(strLog);
		fileLog.Close();
	}
}

/**********************************************************************/
