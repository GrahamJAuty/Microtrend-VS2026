/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "LoyaltySemaphoreTidyUpThread.h"
/**********************************************************************/

CLoyaltySemaphoreTidyUpThreadInfo::CLoyaltySemaphoreTidyUpThreadInfo()
{
	m_bForceQuit = FALSE;
	m_strServerInnerLockFolder = "";
	m_strServerOuterLockFolder = "";
	m_strBackgroundLockFolder = "";
	ResetState(0);
}

/**********************************************************************/

void CLoyaltySemaphoreTidyUpThreadInfo::ResetState(int nState)
{
	if ((nState >= 0) && (nState <= 2))
	{
		m_nState = nState;
	}
	else
	{
		m_nState = 0;
	}

	m_strCheckFolder = "";
	m_DirectorySeenTime = 0;
}

/**********************************************************************/

UINT CLoyaltySemaphoreTidyUpThread::DoWork( LPVOID pParam )
{
	CLoyaltySemaphoreTidyUpThreadInfo* pInfo = (CLoyaltySemaphoreTidyUpThreadInfo*) pParam;
	DoWorkInternal( pInfo );
	return 0;
}

/**********************************************************************/

void CLoyaltySemaphoreTidyUpThread::DoWorkInternal( CLoyaltySemaphoreTidyUpThreadInfo* pInfo )
{
	do
	{
		CString strFolder = "";
		
		switch (pInfo -> m_nState)
		{
		case 0: 
			strFolder = pInfo->m_strServerInnerLockFolder + "_";
			break;

		case 1:
			strFolder = pInfo->m_strServerOuterLockFolder + "_";
			break;

		case 2:
			strFolder = pInfo->m_strBackgroundLockFolder + "_";
			break;
		}

		switch (pInfo->m_nState)
		{
		case 0:
		case 1:
		case 2:
		{
			CFileFind fileFinder;

			BOOL bWorking = fileFinder.FindFile( strFolder + "*.*");

			bool bGotOldFiles = FALSE;

			while ( (bWorking == TRUE) && ( pInfo -> m_nState != 3 ) )
			{
				bWorking = fileFinder.FindNextFile();

				if (fileFinder.IsDots() == TRUE)
					continue;

				if (fileFinder.IsDirectory() == FALSE)
					continue;

				pInfo->m_strCheckFolder = fileFinder.GetFilePath();
				pInfo->m_DirectorySeenTime = GetTickCount64();
				pInfo->m_nState = 3;
				break;
			}
		}
		break;
		}

		if (pInfo->m_nState != 3)
		{
			pInfo -> ResetState(pInfo->m_nState + 1 );
		}
		else
		{
			if (::ExistSubdirectory(pInfo->m_strCheckFolder) == FALSE)
			{
				pInfo->ResetState(0);
			}
			else if ( GetTickCount64() > pInfo ->m_DirectorySeenTime + 20000 )
			{
				CFileRemover FileRemover;
				FileRemover.RemoveFolder(pInfo->m_strCheckFolder);
				pInfo->ResetState(0);
			}
		}

		Sleep(100);
	} 	
	while ( FALSE == pInfo -> m_bForceQuit );
}

/**********************************************************************/
