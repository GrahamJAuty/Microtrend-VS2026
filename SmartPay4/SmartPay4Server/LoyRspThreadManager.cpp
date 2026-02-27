/**********************************************************************/
#include "..\SmartPay4Shared\MessageLogger.h"
/**********************************************************************/
#include "LoyRspThreadManager.h"
/**********************************************************************/

CLoyRspThreadManager::CLoyRspThreadManager()
{
	for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		m_pThread[nSetNo] = NULL;
		m_arrayThreadInfo[nSetNo].m_nThread = nSetNo;
		m_arrayThreadInfo[nSetNo].m_bLogFileWrites = FALSE;
	}
}

/**********************************************************************/

CLoyRspThreadManager::~CLoyRspThreadManager()
{
}

/**********************************************************************/

CString CLoyRspThreadManager::GetResponseHeader()
{
	return "***MTREND_RESPONSE***";
}

/**********************************************************************/

void CLoyRspThreadManager::QueueResponseHeader(int nThread)
{
	if ((nThread >= 1) && (nThread <= EPOSREQPATH_COUNT))
	{
		m_ResponseQueue[nThread].Add(GetResponseHeader());
	}
}

/**********************************************************************/

void CLoyRspThreadManager::QueueResponseLine(int nThread, CString strLine)
{
	if ((nThread >= 1) && (nThread <= EPOSREQPATH_COUNT))
	{
		m_ResponseQueue[nThread].Add(strLine);
	}
}

/**********************************************************************/

void CLoyRspThreadManager::CheckThread(int nThread)
{
	if ((nThread < 1) || (nThread > EPOSREQPATH_COUNT))
	{
		nThread = 1;
	}

	bool bAlreadyRunning = FALSE;

	if (m_pThread[nThread] != NULL)
	{
		if (WaitForSingleObject(m_pThread[nThread]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_RSPOUT, nThread);

			delete m_pThread[nThread];
			m_pThread[nThread] = NULL;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		m_ResponseCurrent[nThread].RemoveAll();

		if (m_ResponseQueue[nThread].GetSize() != 0)
		{
			CString str = m_ResponseQueue[nThread].GetAt(0);
			if (str != GetResponseHeader())
			{
				m_ResponseQueue[nThread].RemoveAll();
				return;
			}

			m_ResponseQueue[nThread].RemoveAt(0);

			while (m_ResponseQueue[nThread].GetSize() != 0)
			{
				CString str = m_ResponseQueue[nThread].GetAt(0);
				if (str == GetResponseHeader())
				{
					break;
				}

				m_ResponseCurrent[nThread].Add(str);
				m_ResponseQueue[nThread].RemoveAt(0);
			}

			if (m_ResponseCurrent[nThread].GetSize() > 2)
			{
				CString strLogFolder = Server.GetEposLogPath(nThread);
				CString strReqFolder = Server.GetEposRequestPath(nThread);

				if ((strLogFolder != "") && (strReqFolder != ""))
				{
					m_arrayThreadInfo[nThread].m_pResponseBuffer = &m_ResponseCurrent[nThread];
					m_arrayThreadInfo[nThread].m_bLogFileWrites = ((Server.GetThreadDiagnosticsFlags() & 2) != 0);
					m_pThread[nThread] = AfxBeginThread(CLoyRspThread::DoWork, &m_arrayThreadInfo[nThread], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

					if (m_pThread[nThread] != NULL)
					{
						MessageLogger.LogThreadDiagnostic(THREADLOG_RSPIN, nThread);
						m_pThread[nThread]->m_bAutoDelete = FALSE;
						m_pThread[nThread]->ResumeThread();
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CLoyRspThreadManager::ShutdownThreads()
{
	bool bThreadStillRunning = TRUE;

	while (TRUE == bThreadStillRunning)
	{
		bThreadStillRunning = FALSE;

		for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
		{
			if (m_pThread[n] != NULL)
			{
				if (WaitForSingleObject(m_pThread[n]->m_hThread, 0) != WAIT_OBJECT_0)
				{
					bThreadStillRunning = TRUE;
				}
				else
				{
					delete m_pThread[n];
					m_pThread[n] = NULL;
					TRACE("Shutdown RSP Thread\n");
				}
			}
		}
	}
}

/**********************************************************************/
