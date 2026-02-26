/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
/**********************************************************************/
#include "LoyReqTidyUpThreadManager.h"
/**********************************************************************/

CLoyReqTidyUpThreadManager::CLoyReqTidyUpThreadManager()
{
	m_ThreadIndex.m_nDbNo = 0;
	m_ThreadIndex.m_nSetNo = 0;
	
	for (int nDbNo = 0; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			m_pThread[nDbNo][nSetNo] = NULL;
		}
	}
}

/**********************************************************************/

CLoyReqTidyUpThreadManager::~CLoyReqTidyUpThreadManager()
{
}

/**********************************************************************/

void CLoyReqTidyUpThreadManager::CheckThread(CServerDataFolderSetIndex& ThreadIndex)
{
	m_ThreadIndex = ThreadIndex;

	if ((m_ThreadIndex.m_nDbNo < 1) || (m_ThreadIndex.m_nDbNo > PLU_DBNO_COUNT))
	{
		m_ThreadIndex.m_nDbNo = 1;
	}

	if ((m_ThreadIndex.m_nSetNo < 1) || (m_ThreadIndex.m_nSetNo > EPOSREQPATH_COUNT))
	{
		m_ThreadIndex.m_nSetNo = 1;
	}

	bool bAlreadyRunning = FALSE;
	CLoyReqTidyUpThreadInfo* pThreadInfo = &(m_arrayThreadInfo[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]);

	if (m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] != NULL)
	{
		if (WaitForSingleObject(m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_REQTIDYOUT, m_ThreadIndex.m_nDbNo, m_ThreadIndex.m_nSetNo);
			delete m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] = NULL;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		CString strReqFolder = Server.GetEposRequestPath(m_ThreadIndex);

		if (strReqFolder != "")
		{
			pThreadInfo->m_strREQFolder = Server.GetEposRequestPath(m_ThreadIndex);
			pThreadInfo->m_pTidyUpFileBuffer = &m_TidyUpFileBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] = AfxBeginThread(CLoyReqTidyUpThread::DoWork, pThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] != NULL)
			{
				MessageLogger.LogThreadDiagnostic(THREADLOG_REQTIDYIN, m_ThreadIndex.m_nDbNo, m_ThreadIndex.m_nSetNo);
				m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->m_bAutoDelete = FALSE;
				m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->ResumeThread();
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
						TRACE("Shutdown REQ TidyUp Thread\n");
					}
				}
			}
		}
	}
}

/**********************************************************************/

