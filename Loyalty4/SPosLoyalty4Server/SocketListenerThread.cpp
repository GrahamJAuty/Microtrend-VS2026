/**********************************************************************/
#include "CkSocket.h"
#include <thread>
#include <iostream>
/**********************************************************************/
#include "SocketThreadManager.h"
/**********************************************************************/
#include "SocketListenerThread.h"
/**********************************************************************/

UINT CSocketListenerThread::DoWork(LPVOID pParam)
{
	int nForceQuit = 0;
	CSocketThreadManager* pManager = (CSocketThreadManager*)pParam;

	CkSocket listener;

	// Bind to localhost:5555 and listen.
	bool success = listener.BindAndListen(5555, 5);
	if (!success)
	{
		LogThread(pParam, "Listener failed: " + CString(listener.lastErrorText()));
		return 0;
	}

	LogThread(pParam, "Listener bound to port 5555 and listening.");

	while (pManager->GetQuitListenerThreadFlag() == FALSE)
	{
		// Accept a single connection (blocking for now).
		CkSocket* client = listener.AcceptNextConnection(5000);
		if (client != nullptr)
		{
			pManager->CreateNewWorkerThread(client);
		}
	}

	return 0;
}

/**********************************************************************/

void CSocketListenerThread::LogThread(LPVOID pParam, CString strMsg)
{
	CSocketThreadManager* pManager = (CSocketThreadManager*)pParam;
	
	if (FALSE == pManager->GetLogListenerActionFlag())
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