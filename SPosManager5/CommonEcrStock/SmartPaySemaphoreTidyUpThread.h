#pragma once
/**********************************************************************/

struct CSmartPaySemaphoreTidyUpThreadInfo
{
public:
	CSmartPaySemaphoreTidyUpThreadInfo();
	void ResetState( int nState );

public:
	bool m_bForceQuit;
	CString m_strServerOuterLockFolder;
	CString m_strServerInnerLockFolder;
	CString m_strBackgroundLockFolder;
	int m_nState;
	CString m_strCheckFolder;
	ULONGLONG m_DirectorySeenTime;
};

/**********************************************************************/

class CSmartPaySemaphoreTidyUpThread
{
public:
	static UINT DoWork(LPVOID pParam);

private:
	static void DoWorkInternal(CSmartPaySemaphoreTidyUpThreadInfo* pInfo);
	static void CreateLockFile(CString strFolder);
};

/**********************************************************************/
