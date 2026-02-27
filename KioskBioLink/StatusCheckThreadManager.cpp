/**********************************************************************/
#include "SystemData.h"
/**********************************************************************/
#include "StatusCheckThreadManager.h"
/**********************************************************************/

CStatusCheckThreadManager::CStatusCheckThreadManager(CStatus& Status, int nAction) : m_Status(Status)
{
	m_nAction = nAction;

	switch (m_nAction)
	{
	case nWAITSTATUS_ACTION_CHECKSERVERONLINE:
	case nWAITSTATUS_ACTION_SETSTATUSON_CHECKSERVERONLINE:
	case nWAITSTATUS_ACTION_SETSTATUSINSERVICE_CHECKSERVERONLINE:
		m_nIntResult = nSTATUS_OFFLINE;
		m_bBoolResult = FALSE;
		break;

	case nWAITSTATUS_ACTION_SETSTATUSON:
	case nWAITSTATUS_ACTION_SETSTATUSOFF:
	case nWAITSTATUS_ACTION_SETSTATUSINSERVICE:
	case nWAITSTATUS_ACTION_SETSTATUSOUTSERVICE:
	case nWAITSTATUS_ACTION_EMPTYCASH:
		m_nIntResult = 0;
		m_bBoolResult = FALSE;
		break;
	}
}

/**********************************************************************/

CStatusCheckThreadManager::~CStatusCheckThreadManager()
{
}

/**********************************************************************/

void CStatusCheckThreadManager::CheckThread()
{
	bool bAlreadyRunning = FALSE;

	if (m_pThread != nullptr)
	{
		if (WaitForSingleObject(m_pThread->m_hThread, 0) != WAIT_OBJECT_0)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			delete m_pThread;
			m_pThread = nullptr;
			m_bThreadDone = TRUE;
			m_nIntResult = m_ThreadInfo.m_nIntResult;
			m_bBoolResult = m_ThreadInfo.m_bBoolResult;
			m_Status.SetStatusErrorNo(m_ThreadInfo.m_nStatusErrorNo);
			m_Status.SetStatusReply(m_ThreadInfo.m_strStatusReply);
		}
	}

	if ((FALSE == bAlreadyRunning) && (FALSE == m_bThreadDone))
	{
		m_ThreadInfo.m_nTerminalNo = m_Status.GetTerminalNo();
		m_ThreadInfo.m_nStatusErrorNo = m_Status.GetStatusErrorNo();
		m_ThreadInfo.m_strImportFile = m_Status.GetImportFile();
		m_ThreadInfo.m_strStatusReply = m_Status.GetStatusReply();
		m_ThreadInfo.m_strLoyReqFilename = System.m_strLoyReqFilename;
		m_ThreadInfo.m_strLoyReqTempFilename = System.m_strLoyReqTempFilename;
		m_ThreadInfo.m_strLoyReqReplyFilename = System.m_strLoyReqReplyFilename;
		m_ThreadInfo.m_nAction = m_nAction;
		m_ThreadInfo.m_nIntResult = 0;
		m_ThreadInfo.m_bBoolResult = FALSE;

		m_pThread = AfxBeginThread(CStatusCheckThread::DoWork, &m_ThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if (m_pThread != nullptr)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

/**********************************************************************/

void CStatusCheckThreadManager::ShutdownThread()
{
	if (m_pThread == NULL)
	{
		TRACE("Status Check thread not running\n");
		return;
	}

	//m_ThreadInfo.m_bWantQuit = TRUE;
	if (WaitForSingleObject(m_pThread->m_hThread, 10000) == WAIT_OBJECT_0)
	{
		delete m_pThread;
		m_pThread = NULL;
		TRACE("Shutdown status check thread\n");
	}
	else
	{
		TRACE("Failed to shutdown status check thread\n");
	}
}

/**********************************************************************/

