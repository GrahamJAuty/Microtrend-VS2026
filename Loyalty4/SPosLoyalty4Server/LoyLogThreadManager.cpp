/**********************************************************************/
#include "..\SPOSLoyalty4Shared\FileRemover.h"
#include "..\SPOSLoyalty4Shared\MessageLogger.h"
/**********************************************************************/
#include "TransactionCache.h"
/**********************************************************************/
#include "LoyLogThreadManager.h"
/**********************************************************************/

CLoyLogThreadManager::CLoyLogThreadManager()
{
	m_ThreadIndex.m_nDbNo = 0;
	m_ThreadIndex.m_nSetNo = 0;
	m_strLOGFilepath = "";
	m_strLOGFilename = "";
	m_strRSPFilepath = "";
	m_strRSPFilepathTemp = "";
	m_nThreadTime = 1;
	
	for (int nDbNo = 0; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		for (int nSetNo = 0; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			m_pThread[nDbNo][nSetNo] = NULL;
		}
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

void CLoyLogThreadManager::CheckThread(CServerDataFolderSetIndex& ThreadIndex)
{
	m_arrayLOG.RemoveAll();
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
	CLoyLogThreadInfo* pThreadInfo = &(m_arrayThreadInfo[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]);

	if (m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] != NULL)
	{
		if (WaitForSingleObject(m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			MessageLogger.LogThreadDiagnostic(THREADLOG_LOGOUT, m_ThreadIndex.m_nDbNo, m_ThreadIndex.m_nSetNo);

			delete m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] = NULL;

			if (Server.GetLogActionFlag() == TRUE)
			{
				//LOG ANY EMPTY FILES THAT WE HAVE NOT SEEN BEFORE
				for (int n = 0; n < m_EmptyFileBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetSize(); n++)
				{
					CString strEmptyFilePath = m_EmptyFileBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetAt(n);
					CSortedIntAndStringItem EmptyFileCacheItem;
					EmptyFileCacheItem.m_nItem = (m_ThreadIndex.m_nDbNo * 1000) + m_ThreadIndex.m_nSetNo;
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

				int nSize = m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetSize();

				if (nSize > 0)
				{
					CString strLogFilePath = pThreadInfo->m_strLOGFilepath;

					CSortedIntAndStringItem EmptyFileCacheItem;
					EmptyFileCacheItem.m_nItem = (m_ThreadIndex.m_nDbNo * 1000) + m_ThreadIndex.m_nSetNo;
					EmptyFileCacheItem.m_strItem = strLogFilePath;

					CString strMsg = "";
					strMsg.Format("==> SPOS LOG: %s (%d)",
						(const char*)strLogFilePath,
						nSize);

					MessageLogger.LogSystemMessage(strMsg);
					m_EmptyLogFileCache.RemoveFromCache(EmptyFileCacheItem);
				}
			}

			if (m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetSize() > 0)
			{
				CString strTransaction = m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetAt(0);

				if (TransactionCache.CheckCache(strTransaction) == TRUE)
				{
					CString strMsg = "==> Log Cache Hit: ";
					strMsg += strTransaction;
					MessageLogger.LogSystemMessage(strMsg);
				}
				else
				{
					TransactionCache.AddToCache(strTransaction);

					for (int n = 0; n < m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetSize(); n++)
					{
						m_arrayLOG.Add(m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].GetAt(n));
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
		CString strLogFolder = Server.GetEposLogPath(m_ThreadIndex);
		CString strReqFolder = Server.GetEposRequestPath(m_ThreadIndex);

		if ((strLogFolder != "") && (strReqFolder != ""))
		{
			m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].RemoveAll();
			m_EmptyFileBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo].RemoveAll();
			pThreadInfo->m_StartTime = GetTickCount64();
			pThreadInfo->m_EndTime = pThreadInfo->m_StartTime;
			pThreadInfo->m_strLOGFolder = Server.GetEposLogPath(m_ThreadIndex);
			pThreadInfo->m_pLOGBuffer = &m_LOGBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			pThreadInfo->m_pEmptyFileBuffer = &m_EmptyFileBuffer[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo];
			pThreadInfo->m_strLOGFilepath = "";
			pThreadInfo->m_strLOGFilename = "";
			pThreadInfo->m_strRSPFilepath = "";
			pThreadInfo->m_strRSPFilepathTemp = "";
			pThreadInfo->m_bCopyLogFile = Server.GetCopyLogFilesFlag();
			m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] = AfxBeginThread(CLoyLogThread::DoWork, pThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo] != NULL)
			{
				MessageLogger.LogThreadDiagnostic(THREADLOG_LOGIN, m_ThreadIndex.m_nDbNo, m_ThreadIndex.m_nSetNo);
				m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->m_bAutoDelete = FALSE;
				m_pThread[m_ThreadIndex.m_nDbNo][m_ThreadIndex.m_nSetNo]->ResumeThread();
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
						CreateLocalLogFile();

						delete m_pThread[nDbNo][nSetNo];
						m_pThread[nDbNo][nSetNo] = NULL;
						TRACE("Shutdown LOG Thread\n");
					}
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
		strFilePath.Format("%s\\%3.3d_%3.3d_%s", 
			(const char*) Filenames.GetLogBufferFolder(), 
			m_ThreadIndex.m_nDbNo,
			m_ThreadIndex.m_nSetNo,
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
		rename(strFilePath + ".tmp", strFilePath);
	}
}

/**********************************************************************/
