//*********************************************************************
#include "FileRemover.h"
#include "Prompter.h"
//*********************************************************************
#include "SmartPaySemaphore.h"
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

CLoyaltyLockRequest::CLoyaltyLockRequest()
{
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

void CSmartPaySemaphore::Init(bool bLoyalty)
{
	if (FALSE == m_bDoneInit)
	{
		m_strSyssetPath = Sysset.GetSyssetProgramPath();
		m_strSyssetPath = m_strSyssetPath.Left(m_strSyssetPath.GetLength() - 4);
		m_strSubPathProgram = bLoyalty ? "\\P45" : "\\P44";
		m_strSubPathService = "\\WinServices";
		m_strSubPathServerOuter = "\\Server1";
		m_strSubPathServerInner = "\\Server2";
		m_strSubPathBackground = "\\Bgnd";

		m_lockServerOuter.m_bIsLocked = FALSE;
		m_lockServerOuter.m_bZombieCheck = FALSE;
		m_lockServerOuter.m_ZombieCheckTime = 0;
		m_lockServerOuter.m_nLockNumber = 0;
		m_lockServerOuter.m_strLockFolder = GetServerOuterPath();
		m_lockServerOuter.m_strLockType = "Server outer ";

		m_lockServerInner.m_bIsLocked = FALSE;
		m_lockServerInner.m_bZombieCheck = FALSE;
		m_lockServerInner.m_ZombieCheckTime = 0;
		m_lockServerInner.m_nLockNumber = 0;
		m_lockServerInner.m_strLockFolder = GetServerInnerPath();
		m_lockServerInner.m_strLockType = "Server inner ";

		m_lockBackground.m_bIsLocked = FALSE;
		m_lockBackground.m_bZombieCheck = FALSE;
		m_lockBackground.m_ZombieCheckTime = 0;
		m_lockBackground.m_nLockNumber = 0;
		m_lockBackground.m_strLockFolder = GetBackgroundPath();
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
	if ( TRUE == m_bDoneInit )
	{
		CFileRemover FileRemover;

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
	strTempFolder.Format("%s_%I64d", LockDetails.m_strLockFolder, GetTickCount64());

	if (CreateDirectory(strTempFolder, NULL) == TRUE)
	{
		bCreatedFolder = TRUE;

		CSSFile fileActive;
		if (fileActive.Open(strTempFolder + "\\Active.dat", "wb") == TRUE)
		{
			fileActive.WriteLine("Active");
			fileActive.Close();
			bResult = (rename(strTempFolder, LockDetails.m_strLockFolder) == 0);
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
	rename(LockDetails.m_strLockFolder, LockDetails.GetDeletionFolder());
	FileRemover.RemoveFolder(LockDetails.GetDeletionFolder());
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

	::MakeSubdirectory(GetProgramPath());
	::MakeSubdirectory(GetServicePath());

	if (FALSE == LockDetails.m_bIsLocked)
	{
		if (CreateLockFolder(LockDetails) == FALSE)
		{
			if (::FileExists(LockDetails.GetActiveFilename()) == TRUE)
			{
				ULONGLONG TickCountNow = GetTickCount64();

				if ( FALSE == LockDetails.m_bZombieCheck )
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
					if (TickCountNow > LockDetails.m_ZombieCheckTime + 15000)
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
			DeleteLockFolder(LockDetails);
			LockDetails.m_bIsLocked = FALSE;
			bThreadGotLock = FALSE;
			LockDetails.m_nLockNumber = 0;
			Diagnostic(LockDetails, "release lock");
		}
	}
}

//*********************************************************************

void CSmartPaySemaphore::Diagnostic(CSmartPaySemaphoreLockDetails& LockDetails, CString strMsg)
{
#ifdef _DEBUG
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
#endif
}

//*********************************************************************
//*********************************************************************
//*********************************************************************

CLocalSmartPaySemaphore::CLocalSmartPaySemaphore()
{
	m_nLockNumber = SmartPaySemaphore.GetNextLockNumber();
	m_bSetRequest = FALSE;
	m_bGotLock = FALSE;
	m_bDoneUnlock = FALSE;
}

//*********************************************************************

void CLocalSmartPaySemaphore::SetRequest( CSmartPayLockRequest Request )
{
	if ( FALSE == m_bSetRequest )
	{
		m_Request = Request;
		m_bSetRequest = TRUE;
	}
}

//*********************************************************************

void CLocalSmartPaySemaphore::Lock()
{
	if ( ( TRUE == m_bGotLock ) || ( TRUE == m_bDoneUnlock ) || ( FALSE == m_bSetRequest ) )
	{
		return;
	}

	m_bGotLock = TRUE;

	if ( TRUE == m_Request.m_bLockBackground )
	{
		m_bGotLock = SmartPaySemaphore.LockBackground( m_nLockNumber );
	}

	if ( ( TRUE == m_Request.m_bLockServerOuter ) && ( TRUE == m_bGotLock ) )
	{
		m_bGotLock = SmartPaySemaphore.LockServerOuter( m_nLockNumber );
	}

	if ( ( TRUE == m_Request.m_bLockServerInner ) && ( TRUE == m_bGotLock ) )
	{
		m_bGotLock = SmartPaySemaphore.LockServerInner( m_nLockNumber );
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

CLocalLoyaltySemaphore::CLocalLoyaltySemaphore()
{
	m_nLockNumber = LoyaltySemaphore.GetNextLockNumber();
	m_bSetRequest = FALSE;
	m_bGotLock = FALSE;
	m_bDoneUnlock = FALSE;
}

//*********************************************************************

void CLocalLoyaltySemaphore::SetRequest( CLoyaltyLockRequest Request )
{
	if ( FALSE == m_bSetRequest )
	{
		m_Request = Request;
		m_bSetRequest = TRUE;
	}
}

//*********************************************************************

void CLocalLoyaltySemaphore::Lock()
{
	if ( ( TRUE == m_bGotLock ) || ( TRUE == m_bDoneUnlock ) || ( FALSE == m_bSetRequest ) )
	{
		return;
	}

	m_bGotLock = TRUE;

	if ( ( TRUE == m_Request.m_bLockServerOuter ) && ( TRUE == m_bGotLock ) )
	{
		m_bGotLock = LoyaltySemaphore.LockServerOuter( m_nLockNumber );
	}

	if ( ( TRUE == m_Request.m_bLockServerInner ) && ( TRUE == m_bGotLock ) )
	{
		m_bGotLock = LoyaltySemaphore.LockServerInner( m_nLockNumber );
	}
}

//*********************************************************************

CLocalLoyaltySemaphore::~CLocalLoyaltySemaphore()
{
	Unlock();
}

//*********************************************************************

bool CLocalLoyaltySemaphore::GotLock()
{
	return ( ( TRUE == m_bGotLock ) && ( FALSE == m_bDoneUnlock ) );
}

//*********************************************************************

void CLocalLoyaltySemaphore::Unlock()
{
	if ( FALSE == m_bDoneUnlock )
	{
		LoyaltySemaphore.UnlockServerOuter( m_nLockNumber );
		LoyaltySemaphore.UnlockServerInner( m_nLockNumber );
		m_bDoneUnlock = TRUE;
	}
}

//*********************************************************************
