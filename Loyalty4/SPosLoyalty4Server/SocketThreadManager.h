#pragma once
/**********************************************************************/
#include "CkSocket.h"
/**********************************************************************/
#include "SocketListenerThread.h"
#include "SocketWorkerThread.h"
/**********************************************************************/

class CSocketThreadManager
{
public:
	CSocketThreadManager();
	~CSocketThreadManager();

public:
	void CreateNewWorkerThread(CkSocket* pClient);
	void CleanUpWorkerThreads();

public:
	bool GetLogListenerActionFlag() { return m_bLogListenerAction; }
	bool GetQuitListenerThreadFlag() { return m_bQuitListenerThread; }

public:
	void ShutdownThreads();

public:
	void StartListenerThread();
	void LogThread(CString strMsg);

private:
	bool CleanUpWorkerThread(int nIndex, bool bShutdown);

private:
	CArray<CSocketWorkerThreadInfo*, CSocketWorkerThreadInfo*> m_arrayWorkerThreadInfo;
	CWinThread* m_pListenerThread = nullptr;

private:
	bool m_bLogListenerAction = FALSE;
	bool m_bQuitListenerThread = FALSE;
};

/**********************************************************************/