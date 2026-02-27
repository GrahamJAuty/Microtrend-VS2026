/**********************************************************************/
#include "SystemData.h"
/**********************************************************************/
#include "GetUserThreadManager.h"
/**********************************************************************/

CGetUserThreadManager::CGetUserThreadManager(CString strFilename, bool bServerValidate, CStringArray& arrayUserLines, CUserIDRecord& UserIDRecord) :
	m_UserIDRecord(UserIDRecord),
	m_arrayUserLines(arrayUserLines)
{
	m_strFilename = strFilename;
	m_bServerValidate = bServerValidate;
	m_arrayUserLines.RemoveAll();
}

/**********************************************************************/

CGetUserThreadManager::~CGetUserThreadManager()
{
}

/**********************************************************************/

void CGetUserThreadManager::CheckThread()
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
			m_bBoolResult = m_ThreadInfo.m_bBoolResult;
			m_nMessageNo = m_ThreadInfo.m_UserIDRecord.m_nMessageNo;
			m_UserIDRecord.CopyFrom(m_ThreadInfo.m_UserIDRecord);

			for (int n = 0; n < m_ThreadInfo.m_arrayUserLines.GetCount(); n++)
			{
				m_arrayUserLines.Add(m_ThreadInfo.m_arrayUserLines.GetAt(n));
			}
		}
	}

	if ((FALSE == bAlreadyRunning) && (FALSE == m_bThreadDone))
	{
		m_ThreadInfo.m_strFilename = m_strFilename;
		m_ThreadInfo.m_UserIDRecord.CopyFrom(m_UserIDRecord);
		m_ThreadInfo.m_strLoyReqFilename = System.m_strLoyReqFilename;
		m_ThreadInfo.m_strLoyReqTempFilename = System.m_strLoyReqTempFilename;
		m_ThreadInfo.m_strLoyReqReplyFilename = System.m_strLoyReqReplyFilename;
		m_ThreadInfo.m_nMessageNo = nMESSAGE_NONE;
		m_ThreadInfo.m_bServerValidate = m_bServerValidate;
		m_ThreadInfo.m_bBoolResult = FALSE;

		m_pThread = AfxBeginThread(CGetUserThread::DoWork, &m_ThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if (m_pThread != nullptr)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

/**********************************************************************/

void CGetUserThreadManager::ShutdownThread()
{
	if (m_pThread == NULL)
	{
		TRACE("Get User thread not running\n");
		return;
	}

	//m_ThreadInfo.m_bWantQuit = TRUE;
	if (WaitForSingleObject(m_pThread->m_hThread, 10000) == WAIT_OBJECT_0)
	{
		delete m_pThread;
		m_pThread = NULL;
		TRACE("Shutdown get user thread\n");
	}
	else
	{
		TRACE("Failed to shutdown get user thread\n");
	}
}

/**********************************************************************/

