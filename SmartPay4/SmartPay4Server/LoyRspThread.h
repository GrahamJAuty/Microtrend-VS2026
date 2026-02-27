#pragma once
/**********************************************************************/

struct CLoyRspThreadInfo
{
public:
	CLoyRspThreadInfo();

public:
	CStringArray* m_pResponseBuffer = nullptr;
	int m_nThread = 0;
	bool m_bLogFileWrites = FALSE;
};

/**********************************************************************/

class CLoyRspThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal( CLoyRspThreadInfo* pInfo );
	static void LogThread(CLoyRspThreadInfo* pInfo, CString strMsg);
};

/**********************************************************************/
