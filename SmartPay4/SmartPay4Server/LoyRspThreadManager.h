#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\ServerData.h"
/**********************************************************************/
#include "LoyRspThread.h"
/**********************************************************************/

class CLoyRspThreadManager
{
public:
	CLoyRspThreadManager();
	~CLoyRspThreadManager();

public:
	void ShutdownThreads();

public:
	void QueueResponseHeader(int nThread);
	void QueueResponseLine(int nThread, CString strLine);
	void CheckThread(int nThread);

private:
	CString GetResponseHeader();

private:
	CLoyRspThreadInfo m_arrayThreadInfo[EPOSREQPATH_COUNT + 1];
	CStringArray m_ResponseQueue[EPOSREQPATH_COUNT + 1];
	CStringArray m_ResponseCurrent[EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[EPOSREQPATH_COUNT + 1];
};

/**********************************************************************/
