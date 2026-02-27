/**********************************************************************/
#include "..\SPosLoyalty4Shared\MessageLogger.h"
/**********************************************************************/
#include "LoyReqThreadManager.h"
/**********************************************************************/

CLoyReqThreadManager::CLoyReqThreadManager()
{
	m_ThreadIndex.m_nDbNo = 0;
	m_ThreadIndex.m_nSetNo = 0;
	m_strREQFilepath = "";
	m_strREQFilename = "";
	m_strRSPFilepath = "";
	m_strRSPFilepathTemp = "";
	m_bIsReconRequest = FALSE;
	m_bIsJSONRequest = FALSE;
	m_nThreadTime = 0;

	for (int nDbNo = 0; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			m_pThread[nDbNo][nSetNo] = NULL;
			m_arrayThreadInfo[nDbNo][nSetNo].m_nDbNo = nDbNo;
			m_arrayThreadInfo[nDbNo][nSetNo].m_nSetNo = nSetNo;
			m_arrayThreadInfo[nDbNo][nSetNo].m_bLogFileReads = FALSE;
		}
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

void CLoyReqThreadManager::CheckThread(CServerDataFolderSetIndex& ThreadIndex)
{
	m_arrayRequest.RemoveAll();
	m_nThreadTime = 0;

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
	CLoyReqThreadInfo* pThreadInfo = &(m_arrayThreadInfo[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]);

	if (m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] != NULL)
	{
		if (WaitForSingleObject(m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_REQOUT, m_ThreadIndex.m_nDbNo, m_ThreadIndex.m_nSetNo);

			delete m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] = NULL;

			for (int n = 0; n < m_RequestBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetSize(); n++)
			{
				m_arrayRequest.Add(m_RequestBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetAt(n));
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
			m_arrayResponse.Add(Server.GetExternalRequestPathFlag(m_ThreadIndex) ? "1" : "0");

			m_nThreadTime = pThreadInfo->m_EndTime - pThreadInfo->m_StartTime;
		}
	}

	if (FALSE == bAlreadyRunning)
	{
		CString strLogFolder = Server.GetEposLogPath(m_ThreadIndex);
		CString strReqFolder = Server.GetEposRequestPath(m_ThreadIndex);

		if ((strLogFolder != "") && (strReqFolder != ""))
		{
			m_RequestBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].RemoveAll();
			pThreadInfo->m_StartTime = GetTickCount64();
			pThreadInfo->m_EndTime = pThreadInfo->m_StartTime;
			pThreadInfo->m_strREQFolder = Server.GetEposRequestPath(m_ThreadIndex);
			pThreadInfo->m_pRequestBuffer = &m_RequestBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			pThreadInfo->m_bLogFileReads = ((Server.GetThreadDiagnosticsFlags() & 2) != 0);
			pThreadInfo->m_strREQFilepath = "";
			pThreadInfo->m_strREQFilename = "";
			pThreadInfo->m_strRSPFilepath = "";
			pThreadInfo->m_strRSPFilepathTemp = "";
			pThreadInfo->m_bIsReconRequest = FALSE;
			pThreadInfo->m_bIsJSONRequest = FALSE;
			m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] = AfxBeginThread(CLoyReqThread::DoWork, pThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] != NULL)
			{
				MessageLogger.LogThreadDiagnostic(THREADLOG_REQIN, m_ThreadIndex.m_nDbNo, m_ThreadIndex.m_nSetNo);
				m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->m_bAutoDelete = FALSE;
				m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->ResumeThread();
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
						TRACE("Shutdown REQ Thread\n");
					}
				}
			}
		}
	}
}

/**********************************************************************/

