#pragma once
/**********************************************************************/
#include "Defines.h"
#include "Status.h"
#include "GetUserThread.h"
/**********************************************************************/

class CGetUserThreadManager
{
public:
	CGetUserThreadManager(CString strFilename, bool bServerValidate, CStringArray& arrayUserLines, CUserIDRecord& UserIDRecord);
	~CGetUserThreadManager();

public:
	void ShutdownThread();	
	void CheckThread();

public:
	bool IsThreadDone() { return m_bThreadDone; }
	bool GetBoolResult() { return m_bBoolResult; }	
	int GetMessageNo() { return m_nMessageNo; }
	
private:
	CGetUserThreadInfo m_ThreadInfo;
	CWinThread* m_pThread = nullptr;

private:
	CString m_strFilename = "";
	bool m_bServerValidate = FALSE;
	CUserIDRecord& m_UserIDRecord;
	CStringArray& m_arrayUserLines;

private:
	bool m_bBoolResult = FALSE;
	int m_nMessageNo = nMESSAGE_NONE;
	bool m_bThreadDone = FALSE;
};

/**********************************************************************/
