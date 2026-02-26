/**********************************************************************/
#include "..\SPosLoyalty4Shared\MessageLogger.h"
/**********************************************************************/
#include "LoyRspThreadManager.h"
/**********************************************************************/

CLoyRspThreadManager::CLoyRspThreadManager()
{
	for (int nDbNo = 0; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			m_pThread[nDbNo][nSetNo] = NULL;
		}
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

void CLoyRspThreadManager::QueueResponseHeader(CServerDataFolderSetIndex& ThreadIndex)
{
	if ((ThreadIndex.m_nDbNo >= 1) && (ThreadIndex.m_nDbNo <= PLU_DBNO_COUNT) && (ThreadIndex.m_nSetNo >= 1) && (ThreadIndex.m_nSetNo <= EPOSREQPATH_COUNT))
	{
		m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].Add(GetResponseHeader());
	}
}

/**********************************************************************/

void CLoyRspThreadManager::QueueResponseLine(CServerDataFolderSetIndex& ThreadIndex, CString strLine)
{
	if ((ThreadIndex.m_nDbNo >= 1) && (ThreadIndex.m_nDbNo <= PLU_DBNO_COUNT) && (ThreadIndex.m_nSetNo >= 1) && (ThreadIndex.m_nSetNo <= EPOSREQPATH_COUNT))
	{
		m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].Add(strLine);
	}
}

/**********************************************************************/

void CLoyRspThreadManager::CheckThread(CServerDataFolderSetIndex& ThreadIndex)
{
	if ((ThreadIndex.m_nDbNo < 1) || (ThreadIndex.m_nDbNo > PLU_DBNO_COUNT))
	{
		ThreadIndex.m_nDbNo = 1;
	}

	if ((ThreadIndex.m_nSetNo < 1) || (ThreadIndex.m_nSetNo > EPOSREQPATH_COUNT))
	{
		ThreadIndex.m_nSetNo = 1;
	}

	bool bAlreadyRunning = FALSE;

	if (m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo] != NULL)
	{
		if (WaitForSingleObject(m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_RSPOUT, ThreadIndex.m_nDbNo, ThreadIndex.m_nSetNo);

			delete m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo];
			m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo] = NULL;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		m_ResponseCurrent[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].RemoveAll();

		if (m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].GetSize() != 0)
		{
			{
				CString str = m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].GetAt(0);

				if (str != GetResponseHeader())
				{
					m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].RemoveAll();
					return;
				}
			}

			m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].RemoveAt(0);

			while (m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].GetSize() != 0)
			{
				CString str = m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].GetAt(0);

				if (str == GetResponseHeader())
				{
					break;
				}

				m_ResponseCurrent[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].Add(str);
				m_ResponseQueue[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].RemoveAt(0);
			}

			if (m_ResponseCurrent[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].GetSize() > 2)
			{
				CString strLogFolder = Server.GetEposLogPath(ThreadIndex);
				CString strReqFolder = Server.GetEposRequestPath(ThreadIndex);

				if ((strLogFolder != "") && (strReqFolder != ""))
				{
					m_arrayThreadInfo[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo].m_pResponseBuffer = &m_ResponseCurrent[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo];
					m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo] = AfxBeginThread(CLoyRspThread::DoWork, &m_arrayThreadInfo[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

					if (m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo] != NULL)
					{
						MessageLogger.LogThreadDiagnostic(THREADLOG_RSPIN, ThreadIndex.m_nDbNo, ThreadIndex.m_nSetNo);
						m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo]->m_bAutoDelete = FALSE;
						m_pThread[ThreadIndex.m_nDbNo][ThreadIndex.m_nSetNo]->ResumeThread();
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

		for (int nDbNo = 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
		{
			for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
			{
				if (m_pThread[nDbNo][nSetNo] != NULL)
				{
					if (WaitForSingleObject(m_pThread[nDbNo][nSetNo]->m_hThread, 0) != WAIT_OBJECT_0)
					{
						bThreadStillRunning = TRUE;
					}
					else
					{
						delete m_pThread[nDbNo][nSetNo];
						m_pThread[nDbNo][nSetNo] = NULL;
						TRACE("Shutdown RSP Thread\n");
					}
				}
			}
		}
	}
}

/**********************************************************************/
