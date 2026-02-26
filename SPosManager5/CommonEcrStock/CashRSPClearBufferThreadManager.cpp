/**********************************************************************/
#include "NetworkCSVArray.h"
#include "CashRSPClearBufferThreadManager.h"
/**********************************************************************/

CCashRSPClearBufferThreadManager::CCashRSPClearBufferThreadManager(void)
{
	m_pRealTimeCommsThread = NULL;
}

/**********************************************************************/

void CCashRSPClearBufferThreadManager::Init()
{
	m_RealTimeCommsThreadInfo.m_strSchedLogAllFilename = Super.SchedLogAll();
	m_RealTimeCommsThreadInfo.m_strSchedLogOldFilename = Super.SchedLogOld();
	m_RealTimeCommsThreadInfo.m_lMaxSchedLogFileSize = Super.MaxLogSysFilesize();

	m_RealTimeCommsThreadInfo.m_strNetworkLogAllFilename = dbNetwork.GetFilePathSyssetSchedLogAll(0);
	m_RealTimeCommsThreadInfo.m_strNetworkLogOldFilename = dbNetwork.GetFilePathSyssetSchedLogOld(0);
	m_RealTimeCommsThreadInfo.m_lMaxNetworkLogFileSize = Super.MaxLogLocFilesize();

	CFilenameUpdater FnUp ( SysFiles::PosTrayStopFile );
	m_RealTimeCommsThreadInfo.m_strPosUtilityStopFile = FnUp.GetFilenameToUse();
	m_RealTimeCommsThreadInfo.m_bPosUtilityStopFlag = FALSE;
	m_RealTimeCommsThreadInfo.m_nMaxUsers = Sysset.GetMaxUsers();
}

/**********************************************************************/

void CCashRSPClearBufferThreadManager::PrepareThread( C5000SiteInfo& infoSite )
{
	m_RealTimeCommsThreadInfo.m_bCASHRSP = TRUE;
	m_RealTimeCommsThreadInfo.m_bLogAllCASHRSPFiles = DealerFlags.GetLogAllCashRSPFilesFlag();
	m_RealTimeCommsThreadInfo.m_bPMS = FALSE;
	m_RealTimeCommsThreadInfo.m_bProducts = FALSE;
	m_RealTimeCommsThreadInfo.m_bCustomer = FALSE;			
	m_RealTimeCommsThreadInfo.m_siteInfoSales = infoSite;
	m_RealTimeCommsThreadInfo.m_bEnableLocal = TRUE;
	m_RealTimeCommsThreadInfo.m_nTransactionCount = 0;
	m_RealTimeCommsThreadInfo.m_nZeroFileCount = infoSite.m_nZeroFileCount;
	m_RealTimeCommsThreadInfo.m_nSpeed = 10;
	m_RealTimeCommsThreadInfo.m_bForceQuit = FALSE;

	_mkdir( infoSite.m_strLocalDestPath );
}

/**********************************************************************/

void CCashRSPClearBufferThreadManager::RunThread()
{
	m_pRealTimeCommsThread = AfxBeginThread( CRealTimeCommsThread::DoComms, &m_RealTimeCommsThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	
	if ( m_pRealTimeCommsThread != NULL )
	{
		m_pRealTimeCommsThread -> m_bAutoDelete = FALSE;
		m_pRealTimeCommsThread -> ResumeThread();
	}	
}

/**********************************************************************/

DWORD CCashRSPClearBufferThreadManager::GetThreadExitCode()
{
	DWORD dwExitCode = 0;
	GetExitCodeThread( m_pRealTimeCommsThread -> m_hThread, &dwExitCode );
	return dwExitCode;
}

/**********************************************************************/

void CCashRSPClearBufferThreadManager::DeleteThread()
{
	if ( NULL != m_pRealTimeCommsThread )
	{
		delete m_pRealTimeCommsThread;
		m_pRealTimeCommsThread = NULL;
	}
}

/**********************************************************************/

void CCashRSPClearBufferThreadManager::TerminateThread()
{
	if ( NULL != m_pRealTimeCommsThread)
	{
		DWORD dwExitCode = 0;
		m_RealTimeCommsThreadInfo.m_bForceQuit = TRUE;

		do
		{
			GetExitCodeThread( m_pRealTimeCommsThread -> m_hThread, &dwExitCode );
		}
		while ( STILL_ACTIVE == dwExitCode );
		
		delete m_pRealTimeCommsThread;
	}
}

/**********************************************************************/
