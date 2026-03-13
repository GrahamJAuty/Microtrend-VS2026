#pragma once
/**********************************************************************/
#include "CkSocket.h"
/**********************************************************************/

struct CSocketWorkerThreadInfo
{
public:
	bool m_bLogAction = FALSE;
	bool m_bWantQuit = FALSE;
	bool m_bRequestReady = FALSE;
	bool m_bResponseReady = FALSE;
	bool m_bWantCleanup = FALSE;
	CkSocket* m_pClient = nullptr;
	CWinThread* m_pThread = nullptr;
	CString m_strRequest = "";
	CString m_strResponse = "";
};

/**********************************************************************/

class CSocketWorkerThread
{
public:
	static UINT DoWork(LPVOID pParam);

private:
	static void DoWorkInternal(LPVOID pParam);


private:
	static void LogThread(LPVOID pParam, CString strMsg);
};

/**********************************************************************/
