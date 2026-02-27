/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\MessageLogger.h"
/**********************************************************************/
#include "LoyReqTidyUpThreadManager.h"
/**********************************************************************/

CLoyReqTidyUpThreadManager::CLoyReqTidyUpThreadManager()
{
	m_nCurrentThread = 0;

	for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		m_pThread[nSetNo] = NULL;
	}
}

/**********************************************************************/

CLoyReqTidyUpThreadManager::~CLoyReqTidyUpThreadManager()
{
}

/**********************************************************************/

void CLoyReqTidyUpThreadManager::CheckThread(int nThread)
{
	m_nCurrentThread = nThread;

	if ((m_nCurrentThread < 1) || (m_nCurrentThread > EPOSREQPATH_COUNT))
	{
		m_nCurrentThread = 1;
	}

	bool bAlreadyRunning = FALSE;
	CLoyReqTidyUpThreadInfo* pThreadInfo = &(m_arrayThreadInfo[m_nCurrentThread]);

	if (m_pThread[m_nCurrentThread] != NULL)
	{
		if (WaitForSingleObject(m_pThread[m_nCurrentThread]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_REQTIDYOUT, m_nCurrentThread);
			delete m_pThread[m_nCurrentThread];
			m_pThread[m_nCurrentThread] = NULL;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		CString strReqFolder = Server.GetEposRequestPath(m_nCurrentThread);

		if (strReqFolder != "")
		{
			pThreadInfo->m_strREQFolder = Server.GetEposRequestPath(m_nCurrentThread);
			pThreadInfo->m_pTidyUpFileBuffer = &m_TidyUpFileBuffer[m_nCurrentThread];
			m_pThread[m_nCurrentThread] = AfxBeginThread(CLoyReqTidyUpThread::DoWork, pThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pThread[m_nCurrentThread] != NULL)
			{
				MessageLogger.LogThreadDiagnostic(THREADLOG_REQTIDYIN, m_nCurrentThread);
				m_pThread[m_nCurrentThread]->m_bAutoDelete = FALSE;
				m_pThread[m_nCurrentThread]->ResumeThread();
			}
		}
	}
}

/**********************************************************************/

void CLoyReqTidyUpThreadManager::ShutdownThreads()
{
	bool bThreadStillRunning = TRUE;

	while (TRUE == bThreadStillRunning)
	{
		bThreadStillRunning = FALSE;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			if (m_pThread[nSetNo] != NULL)
			{
				if (WaitForSingleObject(m_pThread[nSetNo]->m_hThread, 0) != WAIT_OBJECT_0)
				{
					bThreadStillRunning = TRUE;
				}
				else
				{
					delete m_pThread[nSetNo];
					m_pThread[nSetNo] = NULL;
					TRACE("Shutdown REQ TidyUp Thread\n");
				}
			}
		}
	}
}

/**********************************************************************/

