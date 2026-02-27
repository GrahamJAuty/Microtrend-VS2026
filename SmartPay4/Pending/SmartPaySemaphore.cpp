//*********************************************************************
#include "..\SmartPay4Shared\MessageLogger.h"
//*********************************************************************
#include "FilenameHandler.h"
#include "FileRemover.h"
#include "Prompter.h"
#include "ServerData.h"
#include "WaitDlg.h"
//*********************************************************************
#include "SmartPaySemaphore.h"
//*********************************************************************
__int64 CSmartPaySemaphore::m_nMostRenameAttempts = 0;
int CSmartPaySemaphore::m_nRenameEventsSinceLastLog = 0;
__int64 CSmartPaySemaphore::m_nRenameAttemptsSinceLastLog = 0;
//*********************************************************************

CSmartPayLockRequest::CSmartPayLockRequest()
{
	m_bLockBackground = FALSE;
	m_bLockServerOuter = FALSE;
	m_bLockServerInner = FALSE;
};

//*********************************************************************
//*********************************************************************
//*********************************************************************

CSmartPaySemaphoreLockDetails::CSmartPaySemaphoreLockDetails()
{
	m_bIsLocked = FALSE;
	m_bZombieCheck = FALSE;
	m_ZombieCheckTime = 0;
	m_nLockNumber = 0;
	m_strLockFolder = "";
}

//*********************************************************************
//*********************************************************************
//*********************************************************************

CSmartPaySemaphore::CSmartPaySemaphore()
{
	m_pSmartPaySemaphoreThread = NULL;
	m_pSmartPaySemaphoreTidyUpThread = NULL;
	m_bDoneInit = FALSE;
	m_nLockNumberNext = 1;
}

//*********************************************************************

void CSmartPaySemaphore::Init()
{
	if (FALSE == m_bDoneInit)
	{
		m_lockServerOuter.m_bIsLocked = FALSE;
		m_lockServerOuter.m_bZombieCheck = FALSE;
		m_lockServerOuter.m_ZombieCheckTime = 0;
		m_lockServerOuter.m_nLockNumber = 0;
		m_lockServerOuter.m_strLockFolder = Filenames.GetServicesServerOuterLockFolder();
		m_lockServerOuter.m_strLockType = "Server outer ";

		m_lockServerInner.m_bIsLocked = FALSE;
		m_lockServerInner.m_bZombieCheck = FALSE;
		m_lockServerInner.m_ZombieCheckTime = 0;
		m_lockServerInner.m_nLockNumber = 0;
		m_lockServerInner.m_strLockFolder = Filenames.GetServicesServerInnerLockFolder();
		m_lockServerInner.m_strLockType = "Server inner ";

		m_lockBackground.m_bIsLocked = FALSE;
		m_lockBackground.m_bZombieCheck = FALSE;
		m_lockBackground.m_ZombieCheckTime = 0;
		m_lockBackground.m_nLockNumber = 0;
		m_lockBackground.m_strLockFolder = Filenames.GetServicesBackgroundLockFolder();
		m_lockBackground.m_strLockType = "Background ";

		m_SmartPaySemaphoreThreadInfo.m_bForceQuit = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_bGotServerInnerLock = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_bGotServerOuterLock = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_bGotBackgroundLock = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_bSeenServerInnerLockFile = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_bSeenServerOuterLockFile = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_bSeenBackgroundLockFile = FALSE;
		m_SmartPaySemaphoreThreadInfo.m_strServerInnerLockFolder = m_lockServerInner.m_strLockFolder;
		m_SmartPaySemaphoreThreadInfo.m_strServerOuterLockFolder = m_lockServerOuter.m_strLockFolder;
		m_SmartPaySemaphoreThreadInfo.m_strBackgroundLockFolder = m_lockBackground.m_strLockFolder;
		m_pSmartPaySemaphoreThread = AfxBeginThread(CSmartPaySemaphoreThread::DoWork, &m_SmartPaySemaphoreThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if (m_pSmartPaySemaphoreThread != NULL)
		{
			m_pSmartPaySemaphoreThread->m_bAutoDelete = FALSE;
			m_pSmartPaySemaphoreThread->ResumeThread();
			m_bDoneInit = TRUE;

			m_SmartPaySemaphoreTidyUpThreadInfo.m_bForceQuit = FALSE;
			m_SmartPaySemaphoreTidyUpThreadInfo.m_strServerInnerLockFolder = m_lockServerInner.m_strLockFolder;
			m_SmartPaySemaphoreTidyUpThreadInfo.m_strServerOuterLockFolder = m_lockServerOuter.m_strLockFolder;
			m_SmartPaySemaphoreTidyUpThreadInfo.m_strBackgroundLockFolder = m_lockBackground.m_strLockFolder;
			m_SmartPaySemaphoreTidyUpThreadInfo.m_nState = 0;
			m_SmartPaySemaphoreTidyUpThreadInfo.m_strCheckFolder = "";
			m_SmartPaySemaphoreTidyUpThreadInfo.m_DirectorySeenTime = 0;

			m_pSmartPaySemaphoreTidyUpThread = AfxBeginThread(CSmartPaySemaphoreTidyUpThread::DoWork, &m_SmartPaySemaphoreTidyUpThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pSmartPaySemaphoreTidyUpThread != NULL)
			{
				m_pSmartPaySemaphoreTidyUpThread->m_bAutoDelete = FALSE;
				m_pSmartPaySemaphoreTidyUpThread->ResumeThread();
			}
		}
	}
}

//*********************************************************************

void CSmartPaySemaphore::ResetZombieCheck()
{
	m_lockServerInner.m_bZombieCheck = FALSE;
	m_lockServerInner.m_ZombieCheckTime = 0;
	m_lockServerOuter.m_bZombieCheck = FALSE;
	m_lockServerOuter.m_ZombieCheckTime = 0;
	m_lockBackground.m_bZombieCheck = FALSE;
	m_lockBackground.m_ZombieCheckTime = 0;
}

//*********************************************************************

CSmartPaySemaphore::~CSmartPaySemaphore()
{
	if (TRUE == m_bDoneInit)
	{
		if (TRUE == m_lockServerOuter.m_bIsLocked)
		{
			DeleteLockFolder(m_lockServerOuter);
		}

		if (TRUE == m_lockServerInner.m_bIsLocked)
		{
			DeleteLockFolder(m_lockServerInner);
		}

		if (TRUE == m_lockBackground.m_bIsLocked)
		{
			DeleteLockFolder(m_lockBackground);
		}

		if (m_pSmartPaySemaphoreThread != NULL)
		{
			DWORD dwExitCode = 0;
			m_SmartPaySemaphoreThreadInfo.m_bForceQuit = TRUE;

			do
			{
				GetExitCodeThread(m_pSmartPaySemaphoreThread->m_hThread, &dwExitCode);
			} 		
			while (STILL_ACTIVE == dwExitCode);

			delete m_pSmartPaySemaphoreThread;
		}

		if (m_pSmartPaySemaphoreTidyUpThread != NULL)
		{
			DWORD dwExitCode = 0;
			m_SmartPaySemaphoreTidyUpThreadInfo.m_bForceQuit = TRUE;

			do
			{
				GetExitCodeThread(m_pSmartPaySemaphoreTidyUpThread->m_hThread, &dwExitCode);
			} 
			while (STILL_ACTIVE == dwExitCode);

			delete m_pSmartPaySemaphoreTidyUpThread;
		}
	}
}

//*********************************************************************

bool CSmartPaySemaphore::CreateLockFolder(CSmartPaySemaphoreLockDetails& LockDetails)
{
	bool bResult = FALSE;
	bool bCreatedFolder = FALSE;

	CString strTempFolder;
	strTempFolder.Format("%s_%I64d", 
		(const char*) LockDetails.m_strLockFolder, 
		GetTickCount64());

	if (CreateDirectory(strTempFolder, NULL) == TRUE)
	{
		bCreatedFolder = TRUE;

		CSSFile fileActive;
		if (fileActive.Open(strTempFolder + "\\Active.dat", "wb") == TRUE)
		{
			fileActive.WriteLine("Active");
			fileActive.Close();
			bResult = (Rename_Trap(strTempFolder, LockDetails.m_strLockFolder) == 0);
		}
	}

	if ((FALSE == bResult) && (TRUE == bCreatedFolder))
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFolder(strTempFolder);
	}

	return bResult;
}

//*********************************************************************

void CSmartPaySemaphore::DeleteLockFolder(CSmartPaySemaphoreLockDetails& LockDetails)
{
	CFileRemover FileRemover;
	FileRemover.RemoveFolder(LockDetails.GetDeletionFolder());

	CString strDeletionFolder = "";
	strDeletionFolder.Format("%s_%I64d", 
		(const char*) LockDetails.GetDeletionFolder(), 
		GetTickCount64());

	__int64 nAttempts = 0;
	bool bSuccess = FALSE;
	ULONGLONG dwTickCount = GetTickCount64();

	while (GetTickCount64() < dwTickCount + 1000)
	{
		Rename_Trap(LockDetails.m_strLockFolder, strDeletionFolder);
		nAttempts++;

		if (::ExistSubdirectory(LockDetails.m_strLockFolder) == FALSE)
		{
			bSuccess = TRUE;
			break;
		}
	}

	if (FALSE == bSuccess)
	{
		CString strMsg;
		strMsg.Format("Rename lock folder failed after %I64d attempts", nAttempts);
		Diagnostic(LockDetails, strMsg);

		MessageLogger.LogSystemMessage(strMsg);
	}
	else if (nAttempts > 1)
	{
		m_nRenameEventsSinceLastLog++;
		m_nRenameAttemptsSinceLastLog += nAttempts;

		if ((m_nRenameEventsSinceLastLog >= 100) || (nAttempts > m_nMostRenameAttempts))
		{
			CString strMsg;
			strMsg.Format("Rename lock folder succeeded after %I64d attempts (%d:%I64d)",
				nAttempts,
				m_nRenameEventsSinceLastLog,
				m_nRenameAttemptsSinceLastLog);

			Diagnostic(LockDetails, strMsg);

			MessageLogger.LogSystemMessage(strMsg);

			m_nRenameAttemptsSinceLastLog = 0;
			m_nRenameEventsSinceLastLog = 0;

			if (nAttempts > m_nMostRenameAttempts)
			{
				m_nMostRenameAttempts = nAttempts;
			}
		}
	}

	FileRemover.RemoveFolder(strDeletionFolder);
}

//*********************************************************************

bool CSmartPaySemaphore::LockServerOuter(int nLockNumber)
{
	return AcquireLock(m_lockServerOuter, nLockNumber, m_SmartPaySemaphoreThreadInfo.m_bSeenServerOuterLockFile, m_SmartPaySemaphoreThreadInfo.m_bGotServerOuterLock);
}

//*********************************************************************

bool CSmartPaySemaphore::LockServerInner(int nLockNumber)
{
	return AcquireLock(m_lockServerInner, nLockNumber, m_SmartPaySemaphoreThreadInfo.m_bSeenServerInnerLockFile, m_SmartPaySemaphoreThreadInfo.m_bGotServerInnerLock);
}

//*********************************************************************

bool CSmartPaySemaphore::LockBackground(int nLockNumber)
{
	return AcquireLock(m_lockBackground, nLockNumber, m_SmartPaySemaphoreThreadInfo.m_bSeenBackgroundLockFile, m_SmartPaySemaphoreThreadInfo.m_bGotBackgroundLock);
}

//*********************************************************************

bool CSmartPaySemaphore::AcquireLock(CSmartPaySemaphoreLockDetails& LockDetails, int nLockNumber, bool& bThreadSeenLock, bool& bThreadGotLock)
{
	if (FALSE == m_bDoneInit)
	{
		return FALSE;
	}

	::MakeSubdirectory(Filenames.GetServicesFolder());

	if (FALSE == LockDetails.m_bIsLocked)
	{
		if (CreateLockFolder(LockDetails) == FALSE)
		{
			if (::FileExists(LockDetails.GetActiveFilename()) == TRUE)
			{
				ULONGLONG TickCountNow = GetTickCount64();

				if (FALSE == LockDetails.m_bZombieCheck)
				{
					if (TickCountNow > LockDetails.m_ZombieCheckTime + 5000)
					{
						LockDetails.m_bZombieCheck = TRUE;
						LockDetails.m_ZombieCheckTime = TickCountNow;
						CFileRemover FileRemover(LockDetails.GetLockFilename());
						bThreadSeenLock = FALSE;
					}
				}
				else if (FALSE == bThreadSeenLock)
				{
					ULONGLONG nTimeout = Server.GetLockFolderTimeout();
					if (nTimeout > 15)
					{
						nTimeout = 15;
					}

					if (TickCountNow > LockDetails.m_ZombieCheckTime + ( 1000 * nTimeout ) )
					{
						LockDetails.m_bZombieCheck = FALSE;
						DeleteLockFolder(LockDetails);
						Diagnostic(LockDetails, "force deleted");
					}
				}
				else
				{
					LockDetails.m_bZombieCheck = FALSE;
					Diagnostic(LockDetails, "defended");
				}
			}

			return FALSE;
		}
		else
		{
			LockDetails.m_bZombieCheck = FALSE;
			Diagnostic(LockDetails, "got lock");
		}

		LockDetails.m_bIsLocked = TRUE;
		bThreadGotLock = TRUE;
		LockDetails.m_nLockNumber = nLockNumber;
	}

	return TRUE;
}

//*********************************************************************

void CSmartPaySemaphore::UnlockServerOuter(int nLockNumber)
{
	ReleaseLock(nLockNumber, m_lockServerOuter, m_SmartPaySemaphoreThreadInfo.m_bGotServerOuterLock);
}

//*********************************************************************

void CSmartPaySemaphore::UnlockServerInner(int nLockNumber)
{
	ReleaseLock(nLockNumber, m_lockServerInner, m_SmartPaySemaphoreThreadInfo.m_bGotServerInnerLock);
}

//*********************************************************************

void CSmartPaySemaphore::UnlockBackground(int nLockNumber)
{
	ReleaseLock(nLockNumber, m_lockBackground, m_SmartPaySemaphoreThreadInfo.m_bGotBackgroundLock);
}

//*********************************************************************

void CSmartPaySemaphore::ReleaseLock(int nLockNumber, CSmartPaySemaphoreLockDetails& LockDetails, bool& bThreadGotLock)
{
	if (TRUE == m_bDoneInit)
	{
		if ((TRUE == LockDetails.m_bIsLocked) && (LockDetails.m_nLockNumber == nLockNumber))
		{
			bThreadGotLock = FALSE;
			DeleteLockFolder(LockDetails);
			LockDetails.m_bIsLocked = FALSE;
			LockDetails.m_nLockNumber = 0;
			Diagnostic(LockDetails, "release lock");
		}
	}
}

//*********************************************************************

void CSmartPaySemaphore::Diagnostic(CSmartPaySemaphoreLockDetails& LockDetails, CString strMsg)
{
	if (Server.GetCreateLockDiagnosticsFlag() == TRUE)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strLine;
		strLine.Format("%2.2d%2.2d%2.2d %s %s",
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			(const char*)LockDetails.m_strLockType,
			(const char*)strMsg);

		CSSFile fileLockInfo;
		if (fileLockInfo.Open(".\\LockInfo.dat", "ab"))
		{
			fileLockInfo.WriteLine(strLine);
		}
	}
}

//*********************************************************************
//*********************************************************************
//*********************************************************************

CLocalSmartPaySemaphore::CLocalSmartPaySemaphore()
{
	m_bDoneInit = FALSE;
	m_bGotLock = FALSE;
	m_bDoneUnlock = FALSE;
	m_nLockNumber = 0;
}

//*********************************************************************

CLocalSmartPaySemaphore::CLocalSmartPaySemaphore(CSmartPayLockRequest Request, CWnd* pParent)
{
	m_bDoneInit = FALSE;
	m_bGotLock = FALSE;
	m_bDoneUnlock = FALSE;
	m_nLockNumber = 0;
	Init(Request, pParent);
}

//*********************************************************************

void CLocalSmartPaySemaphore::Init(CSmartPayLockRequest Request, CWnd* pParent)
{
	if ( TRUE == m_bDoneInit)
	{
		return;
	}

	m_bDoneInit = TRUE;
	m_bDoneUnlock = FALSE;
	m_nLockNumber = SmartPaySemaphore.GetNextLockNumber();

	m_bGotLock = TRUE;

	if ( TRUE == Request.m_bLockBackground )
	{
		m_bGotLock = SmartPaySemaphore.LockBackground( m_nLockNumber );

		if ( FALSE == m_bGotLock )
		{
			CWaitDlg dlgWait( pParent );
			dlgWait.SetMessageText( "Pausing Background Service" );

			bool bKeepTrying = TRUE;

			while ( ( FALSE == m_bGotLock ) && ( TRUE == bKeepTrying ) )
			{
				int nRetries = 0;
				while( ( FALSE == m_bGotLock ) && ( nRetries < 5 ) )
				{
					Sleep(100);
					m_bGotLock = SmartPaySemaphore.LockBackground( m_nLockNumber );
					nRetries++;
				}

				if ( FALSE == m_bGotLock )
				{
					bKeepTrying = ( Prompter.YesNo( "Unable to pause background service : keep trying", "", TRUE, FALSE ) == IDYES );
				}
			}
		}
	}

	if ( ( TRUE == Request.m_bLockServerOuter ) && ( TRUE == m_bGotLock ) )
	{
		m_bGotLock = SmartPaySemaphore.LockServerOuter( m_nLockNumber );

		if ( FALSE == m_bGotLock )
		{
			CWaitDlg dlgWait( pParent );
			dlgWait.SetMessageText( "Pausing SmartPay Server" );

			bool bKeepTrying = TRUE;

			while ( ( FALSE == m_bGotLock ) && ( TRUE == bKeepTrying ) )
			{
				int nRetries = 0;
				while( ( FALSE == m_bGotLock ) && ( nRetries < 10 ) )
				{
					Sleep(100);
					m_bGotLock = SmartPaySemaphore.LockServerOuter( m_nLockNumber );
					nRetries++;
				}

				if ( FALSE == m_bGotLock )
				{
					bKeepTrying = ( Prompter.YesNo( "Unable to pause SmartPay Server : keep trying", "", TRUE, FALSE ) == IDYES );
				}
			}
		}
	}

	if ( ( TRUE == Request.m_bLockServerInner ) && ( TRUE == m_bGotLock ) )
	{
		m_bGotLock = SmartPaySemaphore.LockServerInner( m_nLockNumber );

		if ( FALSE == m_bGotLock )
		{
			CWaitDlg dlgWait( pParent );
			dlgWait.SetMessageText( "Pausing SmartPay Server" );

			bool bKeepTrying = TRUE;

			while ( ( FALSE == m_bGotLock ) && ( TRUE == bKeepTrying ) )
			{
				int nRetries = 0;
				while( ( FALSE == m_bGotLock ) && ( nRetries < 10 ) )
				{
					Sleep(100);
					m_bGotLock = SmartPaySemaphore.LockServerInner( m_nLockNumber );
					nRetries++;
				}

				if ( FALSE == m_bGotLock )
				{
					bKeepTrying = ( Prompter.YesNo( "Unable to pause SmartPay Server : keep trying", "", TRUE, FALSE ) == IDYES );
				}
			}
		}
	}
}

//*********************************************************************

CLocalSmartPaySemaphore::~CLocalSmartPaySemaphore()
{
	Unlock();
}

//*********************************************************************

bool CLocalSmartPaySemaphore::GotLock()
{
	return ( ( TRUE == m_bGotLock ) && ( FALSE == m_bDoneUnlock ) );
}

//*********************************************************************

void CLocalSmartPaySemaphore::Unlock()
{
	if ( FALSE == m_bDoneUnlock )
	{
		SmartPaySemaphore.UnlockBackground( m_nLockNumber );
		SmartPaySemaphore.UnlockServerOuter( m_nLockNumber );
		SmartPaySemaphore.UnlockServerInner( m_nLockNumber );
		m_bDoneUnlock = TRUE;
	}
}

//*********************************************************************
//*********************************************************************
//*********************************************************************

CLocalSmartPaySemaphoreBackup::CLocalSmartPaySemaphoreBackup()
{
	m_nLockNumber = SmartPaySemaphore.GetNextLockNumber();
	m_bSetRequest = FALSE;
	m_bGotLock = FALSE;
	m_bDoneUnlock = FALSE;
}

//*********************************************************************

void CLocalSmartPaySemaphoreBackup::SetRequest(CSmartPayLockRequest Request)
{
	if (FALSE == m_bSetRequest)
	{
		m_Request = Request;
		m_bSetRequest = TRUE;
	}
}

//*********************************************************************

void CLocalSmartPaySemaphoreBackup::Lock()
{
	if ((TRUE == m_bGotLock) || (TRUE == m_bDoneUnlock) || (FALSE == m_bSetRequest))
	{
		return;
	}

	m_bGotLock = TRUE;

	if (TRUE == m_Request.m_bLockBackground)
	{
		m_bGotLock = SmartPaySemaphore.LockBackground(m_nLockNumber);
	}

	if ((TRUE == m_Request.m_bLockServerOuter) && (TRUE == m_bGotLock))
	{
		m_bGotLock = SmartPaySemaphore.LockServerOuter(m_nLockNumber);
	}

	if ((TRUE == m_Request.m_bLockServerInner) && (TRUE == m_bGotLock))
	{
		m_bGotLock = SmartPaySemaphore.LockServerInner(m_nLockNumber);
	}
}

//*********************************************************************

CLocalSmartPaySemaphoreBackup::~CLocalSmartPaySemaphoreBackup()
{
	Unlock();
}

//*********************************************************************

bool CLocalSmartPaySemaphoreBackup::GotLock()
{
	return ((TRUE == m_bGotLock) && (FALSE == m_bDoneUnlock));
}

//*********************************************************************

void CLocalSmartPaySemaphoreBackup::Unlock()
{
	if (FALSE == m_bDoneUnlock)
	{
		SmartPaySemaphore.UnlockBackground(m_nLockNumber);
		SmartPaySemaphore.UnlockServerOuter(m_nLockNumber);
		SmartPaySemaphore.UnlockServerInner(m_nLockNumber);
		m_bDoneUnlock = TRUE;
	}
}

//*********************************************************************
