#pragma once
/**********************************************************************/

struct CSmartPaySemaphoreThreadInfo
{
public:
	CSmartPaySemaphoreThreadInfo();

public:
	bool m_bForceQuit;
	CString m_strServerOuterLockFolder;
	CString m_strServerInnerLockFolder;
	CString m_strBackgroundLockFolder;
	bool m_bGotServerInnerLock;
	bool m_bGotServerOuterLock;
	bool m_bGotBackgroundLock;
	bool m_bSeenServerInnerLockFile;
	bool m_bSeenServerOuterLockFile;
	bool m_bSeenBackgroundLockFile;
};

/**********************************************************************/

class CSmartPaySemaphoreThread
{
public:
	static UINT DoWork(LPVOID pParam);

private:
	static void DoWorkInternal(CSmartPaySemaphoreThreadInfo* pInfo);
	static void CreateLockFile(CString strFolder);
};

/**********************************************************************/
