/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\MessageLogger.h"
/**********************************************************************/
#include "TransactionCache.h"
/**********************************************************************/
#include "LoyLogThreadManager.h"
/**********************************************************************/

CLoyLogThreadManager::CLoyLogThreadManager()
{
	m_nCurrentThread = 0;
	m_strLOGFilepath = "";
	m_strLOGFilename = "";
	m_strRSPFilepath = "";
	m_strRSPFilepathTemp = "";
	m_nThreadTime = 1;

	for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		m_pThread[nSetNo] = NULL;
	}
}

/**********************************************************************/

CLoyLogThreadManager::~CLoyLogThreadManager()
{
}

/**********************************************************************/

CString CLoyLogThreadManager::GetLOGLine(int n)
{
	CString strResult = "";
	if ((n >= 0) && (n < m_arrayLOG.GetSize()))
	{
		strResult = m_arrayLOG.GetAt(n);
	}
	return strResult;
}

/**********************************************************************/

void CLoyLogThreadManager::CheckThread(int nThread)
{
	m_arrayLOG.RemoveAll();
	m_nThreadTime = 0;
	
	m_nCurrentThread = nThread;
	
	if ((m_nCurrentThread < 1) || (m_nCurrentThread > EPOSREQPATH_COUNT))
	{
		m_nCurrentThread = 1;
	}

	bool bAlreadyRunning = FALSE;
	CLoyLogThreadInfo* pThreadInfo = &(m_arrayThreadInfo[m_nCurrentThread]);

	if (m_pThread[m_nCurrentThread] != NULL)
	{		
		if ( WaitForSingleObject(m_pThread[m_nCurrentThread]->m_hThread, 0 ) != WAIT_OBJECT_0)		
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_LOGOUT, m_nCurrentThread);

			delete m_pThread[m_nCurrentThread];
			m_pThread[m_nCurrentThread] = NULL;

			if (Server.GetLogAction2Flag() == TRUE)
			{
				//LOG ANY EMPTY FILES THAT WE HAVE NOT SEEN BEFORE
				for (int n = 0; n < m_EmptyFileBuffer[m_nCurrentThread].GetSize(); n++)
				{
					CString strEmptyFilePath = m_EmptyFileBuffer[m_nCurrentThread].GetAt(n);
					CSortedIntAndStringItem EmptyFileCacheItem;
					EmptyFileCacheItem.m_nItem = m_nCurrentThread;
					EmptyFileCacheItem.m_strItem = strEmptyFilePath;

					if (m_EmptyLogFileCache.CheckCache(EmptyFileCacheItem) == FALSE)
					{
						CString strMsg = "";
						strMsg.Format("==> SPOS LOG: %s (0)",
							(const char*)strEmptyFilePath);

						MessageLogger.LogSystemMessage(strMsg);
						m_EmptyLogFileCache.AddToCache(EmptyFileCacheItem);
					}
				}

				int nSize = m_LOGBuffer[m_nCurrentThread].GetSize();

				if (nSize > 0)
				{
					CString strLogFilePath = pThreadInfo->m_strLOGFilepath;

					CSortedIntAndStringItem EmptyFileCacheItem;
					EmptyFileCacheItem.m_nItem = m_nCurrentThread;
					EmptyFileCacheItem.m_strItem = strLogFilePath;

					CString strMsg = "";
					strMsg.Format("==> SPOS LOG: %s (%d)",
						(const char*)strLogFilePath,
						nSize);

					MessageLogger.LogSystemMessage(strMsg);
					m_EmptyLogFileCache.RemoveFromCache(EmptyFileCacheItem);
				}
			}

			if (m_LOGBuffer[m_nCurrentThread].GetSize() > 0)
			{
				CString strTransaction = m_LOGBuffer[m_nCurrentThread].GetAt(0);

				if (TransactionCache.CheckCache(strTransaction) == TRUE)
				{
					CString strMsg = "==> Log Cache Hit: ";
					strMsg += strTransaction;
					MessageLogger.LogSystemMessage(strMsg);
				}
				else
				{
					TransactionCache.AddToCache(strTransaction);

					for (int n = 0; n < m_LOGBuffer[m_nCurrentThread].GetSize(); n++)
					{
						m_arrayLOG.Add(m_LOGBuffer[m_nCurrentThread].GetAt(n));
					}

					m_strLOGFilepath = pThreadInfo->m_strLOGFilepath;
					m_strLOGFilename = pThreadInfo->m_strLOGFilename;
					m_strRSPFilepath = pThreadInfo->m_strRSPFilepath;
					m_strRSPFilepathTemp = pThreadInfo->m_strRSPFilepathTemp;
				}
			}

			m_nThreadTime = pThreadInfo->m_EndTime - pThreadInfo->m_StartTime;
		}
	}
	
	if (FALSE == bAlreadyRunning)
	{
		CString strLogFolder = Server.GetEposLogPath(m_nCurrentThread);
		CString strReqFolder = Server.GetEposRequestPath(m_nCurrentThread);

		if ((strLogFolder != "") && (strReqFolder != ""))
		{
			m_LOGBuffer[m_nCurrentThread].RemoveAll();
			m_EmptyFileBuffer[m_nCurrentThread].RemoveAll();
			pThreadInfo->m_StartTime = GetTickCount64();
			pThreadInfo->m_EndTime = pThreadInfo->m_StartTime;
			pThreadInfo->m_strLOGFolder = Server.GetEposLogPath(m_nCurrentThread);
			pThreadInfo->m_pLOGBuffer = &m_LOGBuffer[m_nCurrentThread];
			pThreadInfo->m_pEmptyFileBuffer = &m_EmptyFileBuffer[m_nCurrentThread];
			pThreadInfo->m_strLOGFilepath = "";
			pThreadInfo->m_strLOGFilename = "";
			pThreadInfo->m_strRSPFilepath = "";
			pThreadInfo->m_strRSPFilepathTemp = "";
			pThreadInfo->m_bCopyLogFile = Server.GetCopyLogFilesFlag();
			m_pThread[m_nCurrentThread] = AfxBeginThread(CLoyLogThread::DoWork, pThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pThread[m_nCurrentThread] != NULL)
			{
				MessageLogger.LogThreadDiagnostic(THREADLOG_LOGIN, m_nCurrentThread);
				m_pThread[m_nCurrentThread]->m_bAutoDelete = FALSE;
				m_pThread[m_nCurrentThread]->ResumeThread();
			}
		}
	}
}

/**********************************************************************/

void CLoyLogThreadManager::ShutdownThreads()
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
					CreateLocalLogFile();

					delete m_pThread[n];
					m_pThread[n] = NULL;
					TRACE("Shutdown LOG Thread\n");
				}
			}
		}
	}
}

/**********************************************************************/

void CLoyLogThreadManager::CreateLocalLogFile()
{
	if (GetLOGLineCount() != 0)
	{
		//HOLDING FOLDER FOR LOG FILES
		Filenames.CreateLogBufferFolder();

		CString strFilePath = "";
		strFilePath.Format("%s\\%3.3d_%s", 
			(const char*) Filenames.GetLogBufferFolder(), 
			m_nCurrentThread, 
			(const char*) GetLOGFilename());

		CSSFile fileLog;
		if (fileLog.Open(strFilePath + ".tmp", "wb") == TRUE)
		{
			for (int n = 0; n < GetLOGLineCount(); n++)
			{
				fileLog.WriteLine(GetLOGLine(n));
			}
		}
		fileLog.Close();

		CFileRemover FileRemover;
		FileRemover.RemoveFile(strFilePath);
		int x = rename(strFilePath + ".tmp", strFilePath);
	}
}

/**********************************************************************/
