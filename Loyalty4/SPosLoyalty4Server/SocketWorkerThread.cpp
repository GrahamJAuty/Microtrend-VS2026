/**********************************************************************/
#include "CkSocket.h"
#include <thread>
#include <iostream>
/**********************************************************************/
#include "SocketThreadManager.h"
/**********************************************************************/
#include "SocketListenerThread.h"
/**********************************************************************/

UINT CSocketWorkerThread::DoWork(LPVOID pParam)
{
	DoWorkInternal(pParam);
	
	CSocketWorkerThreadInfo* pInfo = (CSocketWorkerThreadInfo*)pParam;
	pInfo -> m_bWantCleanup = TRUE;

	return 0;
}

/**********************************************************************/

void CSocketWorkerThread::DoWorkInternal(LPVOID pParam)
{
	CSocketWorkerThreadInfo* pInfo = (CSocketWorkerThreadInfo*)pParam;

	pInfo->m_strRequest = "";
	pInfo->m_strResponse = "";
	pInfo->m_bRequestReady = FALSE;
	pInfo->m_bResponseReady = FALSE;

	bool bEndOfMessage = FALSE;
	
	while ((!bEndOfMessage) && (pInfo->m_pClient != nullptr) && (FALSE == pInfo ->m_bWantQuit))
	{
		CkString ckLine;
		if (pInfo->m_pClient->ReceiveString(ckLine))
		{
			CString strLine = ckLine.getUtf8();
			pInfo->m_strRequest += strLine;
			LogThread(pParam, CString("Received: ") + strLine);

			if ((strLine.Right(5) == "##END") || (strLine.IsEmpty()))
			{
				bEndOfMessage = true;
			}
		}
		else
		{
			LogThread(pParam, "Failed to receive string or connection closed");
			break;
		}
	}

	if (FALSE == bEndOfMessage)
	{
		LogThread(pParam, "Did not receive end of message");
		return;
	}


	pInfo->m_bRequestReady = TRUE;
	LogThread(pParam, "Request ready");

	/*
	while ((!pInfo->m_bResponseReady) && (FALSE == pInfo->m_bWantQuit))
	{
		Sleep(10); // Small delay to prevent busy waiting
	}

	if ((pInfo->m_bResponseReady) && (pInfo->m_pClient != NULL) && (FALSE == pInfo -> m_bWantQuit))
	{
		pInfo->m_pClient->SendString(pInfo->m_strResponse);
	}
	*/
}

/**********************************************************************/

void CSocketWorkerThread::LogThread(LPVOID pParam, CString strMsg)
{
	CSocketWorkerThreadInfo* pInfo = (CSocketWorkerThreadInfo*)pParam;

	if (FALSE == pInfo->m_bLogAction)
	{
		return;
	}

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