#include "LoyaltySemaphoreThread.h"
/**********************************************************************/

CLoyaltySemaphoreThreadInfo::CLoyaltySemaphoreThreadInfo()
{
	m_bForceQuit = FALSE;
	m_bGotServerInnerLock = FALSE;
	m_bGotServerOuterLock = FALSE;
	m_bGotBackgroundLock = FALSE;
	m_bSeenServerInnerLockFile = FALSE;
	m_bSeenServerOuterLockFile = FALSE;
	m_bSeenBackgroundLockFile = FALSE;
	m_strServerInnerLockFolder = "";
	m_strServerOuterLockFolder = "";
	m_strBackgroundLockFolder = "";
}

/**********************************************************************/

UINT CLoyaltySemaphoreThread::DoWork( LPVOID pParam )
{
	CLoyaltySemaphoreThreadInfo* pInfo = (CLoyaltySemaphoreThreadInfo*) pParam;
	DoWorkInternal( pInfo );
	return 0;
}

/**********************************************************************/

void CLoyaltySemaphoreThread::DoWorkInternal( CLoyaltySemaphoreThreadInfo* pInfo )
{
	do
	{
		if ( TRUE == pInfo -> m_bGotServerInnerLock )
		{
			CreateLockFile(pInfo->m_strServerInnerLockFolder);
		}
		else if (FALSE == pInfo->m_bSeenServerInnerLockFile)
		{
			CString strLockFilename = pInfo->m_strServerInnerLockFolder + "\\lock.dat";
			pInfo->m_bSeenServerInnerLockFile = (::FileExists(strLockFilename));
		}

		if (TRUE == pInfo->m_bGotServerOuterLock)
		{
			CreateLockFile(pInfo->m_strServerOuterLockFolder);
		}
		else if (FALSE == pInfo->m_bSeenServerOuterLockFile)
		{
			CString strLockFilename = pInfo->m_strServerOuterLockFolder + "\\lock.dat";
			pInfo->m_bSeenServerOuterLockFile = (::FileExists(strLockFilename));
		}

		if (TRUE == pInfo->m_bGotBackgroundLock)
		{
			CreateLockFile(pInfo->m_strBackgroundLockFolder);
		}
		else if (FALSE == pInfo->m_bSeenBackgroundLockFile)
		{
			CString strLockFilename = pInfo->m_strBackgroundLockFolder + "\\lock.dat";
			pInfo->m_bSeenBackgroundLockFile = (::FileExists(strLockFilename));
		}

		Sleep(100);

	} 	
	while ( FALSE == pInfo -> m_bForceQuit );
}

/**********************************************************************/

void CLoyaltySemaphoreThread::CreateLockFile( CString strFolder )
{
	CString strLockFilename = strFolder + "\\lock.dat";

	if (::FileExists(strLockFilename) == FALSE)
	{
		CSSFile fileTest;
		if (fileTest.Open(strLockFilename, "wb") == TRUE)
		{
			fileTest.WriteLine("LOCK");
		}
	}
}

/**********************************************************************/
