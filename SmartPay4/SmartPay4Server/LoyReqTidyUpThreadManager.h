#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\ServerData.h"
/**********************************************************************/
#include "LoyReqTidyUpThread.h"
/**********************************************************************/

class CLoyReqTidyUpThreadManager
{
public:
	CLoyReqTidyUpThreadManager();
	~CLoyReqTidyUpThreadManager();

public:
	void ShutdownThreads();

public:
	int GetCurrentThread() { return m_nCurrentThread; }
	
public:
	void CheckThread(int nThread);
	
private:
	CLoyReqTidyUpThreadInfo m_arrayThreadInfo[EPOSREQPATH_COUNT + 1];
	CReportConsolidationArray<CTidyUpFileInfo> m_TidyUpFileBuffer[EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[EPOSREQPATH_COUNT + 1];

private:
	int m_nCurrentThread;
};

/**********************************************************************/
