/**********************************************************************/
#include "..\SmartPay4Shared\MessageLogger.h"
/**********************************************************************/
#include "LoyReqThreadManager.h"
/**********************************************************************/

CLoyReqThreadManager::CLoyReqThreadManager()
{
	m_nCurrentThread = 1;
	m_strREQFilepath = "";
	m_strREQFilename = "";
	m_strRSPFilepath = "";
	m_strRSPFilepathTemp = "";
	m_bIsReconRequest = FALSE;
	m_bIsJSONRequest = FALSE;
	m_nThreadTime = 0;

	for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		m_pThread[nSetNo] = NULL;
		m_arrayThreadInfo[nSetNo].m_nThread = nSetNo;
		m_arrayThreadInfo[nSetNo].m_bLogFileReads = FALSE;
	}
}

/**********************************************************************/

CLoyReqThreadManager::~CLoyReqThreadManager()
{
}

/**********************************************************************/

CString CLoyReqThreadManager::GetRequestLine(int n)
{
	CString strResult = "";
	if ((n >= 0) && (n < m_arrayRequest.GetSize()))
	{
		strResult = m_arrayRequest.GetAt(n);
	}
	return strResult;
}

/**********************************************************************/

CString CLoyReqThreadManager::GetResponseLine(int n)
{
	CString strResult = "";
	if ((n >= 0) && (n < m_arrayResponse.GetSize()))
	{
		strResult = m_arrayResponse.GetAt(n);
	}
	return strResult;
}

/**********************************************************************/

void CLoyReqThreadManager::SetResponseLine(int n, CString str)
{
	if ((n >= 0) && (n < m_arrayResponse.GetSize()))
	{
		m_arrayResponse.SetAt(n,str);
	}
}

/**********************************************************************/

void CLoyReqThreadManager::CheckThread(int nThread)
{
	m_arrayRequest.RemoveAll();
	m_nThreadTime = 0;

	m_nCurrentThread = nThread;

	if ((m_nCurrentThread < 1) || (m_nCurrentThread > EPOSREQPATH_COUNT))
	{
		m_nCurrentThread = 1;
	}

	bool bAlreadyRunning = FALSE;
	CLoyReqThreadInfo* pThreadInfo = &(m_arrayThreadInfo[m_nCurrentThread]);

	if (m_pThread[m_nCurrentThread] != NULL)
	{
		if (WaitForSingleObject(m_pThread[m_nCurrentThread]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_REQOUT, m_nCurrentThread);
			
			delete m_pThread[m_nCurrentThread];
			m_pThread[m_nCurrentThread] = NULL;

			for (int n = 0; n < m_RequestBuffer[m_nCurrentThread].GetSize(); n++)
			{
				m_arrayRequest.Add(m_RequestBuffer[m_nCurrentThread].GetAt(n));
			}

			m_strREQFilepath = pThreadInfo->m_strREQFilepath;
			m_strREQFilename = pThreadInfo->m_strREQFilename;
			m_strRSPFilepath = pThreadInfo->m_strRSPFilepath;
			m_strRSPFilepathTemp = pThreadInfo->m_strRSPFilepathTemp;
			m_bIsReconRequest = pThreadInfo->m_bIsReconRequest;
			m_bIsJSONRequest = pThreadInfo->m_bIsJSONRequest;

			m_arrayResponse.RemoveAll();
			m_arrayResponse.Add(m_strRSPFilepathTemp);
			m_arrayResponse.Add(m_strRSPFilepath);
			m_arrayResponse.Add(Server.GetExternalRequestPathFlag(m_nCurrentThread) ? "1" : "0");
			
			m_nThreadTime = pThreadInfo->m_EndTime - pThreadInfo->m_StartTime;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		CString strLogFolder = Server.GetEposLogPath(m_nCurrentThread);
		CString strReqFolder = Server.GetEposRequestPath(m_nCurrentThread);

		if ((strLogFolder != "") && (strReqFolder != ""))
		{
			m_RequestBuffer[m_nCurrentThread].RemoveAll();
			pThreadInfo->m_StartTime = GetTickCount64();
			pThreadInfo->m_EndTime = pThreadInfo->m_StartTime;
			pThreadInfo->m_strREQFolder = Server.GetEposRequestPath(m_nCurrentThread);
			pThreadInfo->m_pRequestBuffer = &m_RequestBuffer[m_nCurrentThread];
			pThreadInfo->m_bLogFileReads = ((Server.GetThreadDiagnosticsFlags() & 2) != 0 );
			pThreadInfo->m_strREQFilepath = "";
			pThreadInfo->m_strREQFilename = "";
			pThreadInfo->m_strRSPFilepath = "";
			pThreadInfo->m_strRSPFilepathTemp = "";
			pThreadInfo->m_bIsReconRequest = FALSE;
			pThreadInfo->m_bIsJSONRequest = FALSE;
			m_pThread[m_nCurrentThread] = AfxBeginThread(CLoyReqThread::DoWork, pThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pThread[m_nCurrentThread] != NULL)
			{
				MessageLogger.LogThreadDiagnostic(THREADLOG_REQIN, m_nCurrentThread);
				m_pThread[m_nCurrentThread]->m_bAutoDelete = FALSE;
				m_pThread[m_nCurrentThread]->ResumeThread();
			}
		}
	}
}

/**********************************************************************/

void CLoyReqThreadManager::ShutdownThreads()
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
					TRACE("Shutdown REQ Thread\n");
				}
			}
		}
	}
}

/**********************************************************************/

