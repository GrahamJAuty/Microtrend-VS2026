//*********************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
//*********************************************************************
#include "Prompter.h"
//*********************************************************************
#include "LoyaltySemaphore.h"
//*********************************************************************
__int64 CLoyaltySemaphore::m_nMostRenameAttempts = 0;
int CLoyaltySemaphore::m_nRenameEventsSinceLastLog = 0;
__int64 CLoyaltySemaphore::m_nRenameAttemptsSinceLastLog = 0;
//*********************************************************************

CLoyaltyLockRequest::CLoyaltyLockRequest()
{
	m_bLockServerOuter = FALSE;
	m_bLockServerInner = FALSE;
};

//*********************************************************************
//*********************************************************************
//*********************************************************************

CLoyaltySemaphoreLockDetails::CLoyaltySemaphoreLockDetails()
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

CLoyaltySemaphore::CLoyaltySemaphore()
{
	m_pLoyaltySemaphoreThread = NULL;
	m_pLoyaltySemaphoreTidyUpThread = NULL;
	m_bDoneInit = FALSE;
	m_nLockNumberNext = 1;
}

//*********************************************************************

void CLoyaltySemaphore::Init()
{
	if (FALSE == m_bDoneInit)
	{
		m_strSyssetPath = Sysset.GetSyssetProgramPath();
		m_strSyssetPath = m_strSyssetPath.Left(m_strSyssetPath.GetLength() - 4);
		m_strSubPathProgram = "\\P47";
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

		m_LoyaltySemaphoreThreadInfo.m_bForceQuit = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_bGotServerInnerLock = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_bGotServerOuterLock = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_bGotBackgroundLock = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_bSeenServerInnerLockFile = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_bSeenServerOuterLockFile = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_bSeenBackgroundLockFile = FALSE;
		m_LoyaltySemaphoreThreadInfo.m_strServerInnerLockFolder = m_lockServerInner.m_strLockFolder;
		m_LoyaltySemaphoreThreadInfo.m_strServerOuterLockFolder = m_lockServerOuter.m_strLockFolder;
		m_LoyaltySemaphoreThreadInfo.m_strBackgroundLockFolder = m_lockBackground.m_strLockFolder;
		m_pLoyaltySemaphoreThread = AfxBeginThread(CLoyaltySemaphoreThread::DoWork, &m_LoyaltySemaphoreThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if (m_pLoyaltySemaphoreThread != NULL)
		{
			m_pLoyaltySemaphoreThread->m_bAutoDelete = FALSE;
			m_pLoyaltySemaphoreThread->ResumeThread();
			m_bDoneInit = TRUE;

			m_LoyaltySemaphoreTidyUpThreadInfo.m_bForceQuit = FALSE;
			m_LoyaltySemaphoreTidyUpThreadInfo.m_strServerInnerLockFolder = m_lockServerInner.m_strLockFolder;
			m_LoyaltySemaphoreTidyUpThreadInfo.m_strServerOuterLockFolder = m_lockServerOuter.m_strLockFolder;
			m_LoyaltySemaphoreTidyUpThreadInfo.m_strBackgroundLockFolder = m_lockBackground.m_strLockFolder;
			m_LoyaltySemaphoreTidyUpThreadInfo.m_nState = 0;
			m_LoyaltySemaphoreTidyUpThreadInfo.m_strCheckFolder = "";
			m_LoyaltySemaphoreTidyUpThreadInfo.m_DirectorySeenTime = 0;

			m_pLoyaltySemaphoreTidyUpThread = AfxBeginThread(CLoyaltySemaphoreTidyUpThread::DoWork, &m_LoyaltySemaphoreTidyUpThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

			if (m_pLoyaltySemaphoreTidyUpThread != NULL)
			{
				m_pLoyaltySemaphoreTidyUpThread->m_bAutoDelete = FALSE;
				m_pLoyaltySemaphoreTidyUpThread->ResumeThread();
			}
		}
	}
}

//*********************************************************************

void CLoyaltySemaphore::ResetZombieCheck()
{
	m_lockServerInner.m_bZombieCheck = FALSE;
	m_lockServerInner.m_ZombieCheckTime = 0;
	m_lockServerOuter.m_bZombieCheck = FALSE;
	m_lockServerOuter.m_ZombieCheckTime = 0;
	m_lockBackground.m_bZombieCheck = FALSE;
	m_lockBackground.m_ZombieCheckTime = 0;
}

//*********************************************************************

CLoyaltySemaphore::~CLoyaltySemaphore()
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

		if (m_pLoyaltySemaphoreThread != NULL)
		{
			DWORD dwExitCode = 0;
			m_LoyaltySemaphoreThreadInfo.m_bForceQuit = TRUE;

			do
			{
				GetExitCodeThread(m_pLoyaltySemaphoreThread->m_hThread, &dwExitCode);
			} 		
			while (STILL_ACTIVE == dwExitCode);

			delete m_pLoyaltySemaphoreThread;
		}

		if (m_pLoyaltySemaphoreTidyUpThread != NULL)
		{
			DWORD dwExitCode = 0;
			m_LoyaltySemaphoreTidyUpThreadInfo.m_bForceQuit = TRUE;

			do
			{
				GetExitCodeThread(m_pLoyaltySemaphoreTidyUpThread->m_hThread, &dwExitCode);
			} 
			while (STILL_ACTIVE == dwExitCode);

			delete m_pLoyaltySemaphoreTidyUpThread;
		}
	}
}

//*********************************************************************

bool CLoyaltySemaphore::CreateLockFolder(CLoyaltySemaphoreLockDetails& LockDetails)
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

void CLoyaltySemaphore::DeleteLockFolder(CLoyaltySemaphoreLockDetails& LockDetails)
{
	CFileRemover FileRemover;
	FileRemover.RemoveFolder(LockDetails.GetDeletionFolder());

	CString strDeletionFolder = "";
	strDeletionFolder.Format("%s_%I64d", LockDetails.GetDeletionFolder(), GetTickCount64());

	__int64 nAttempts = 0;
	bool bSuccess = FALSE;
	ULONGLONG dwTickCount = GetTickCount64();

	while (GetTickCount64() < dwTickCount + 1000)
	{
		rename(LockDetails.m_strLockFolder, strDeletionFolder);
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

			if (nAttempts < m_nMostRenameAttempts)
			{
				m_nMostRenameAttempts = nAttempts;
			}
		}
	}

	FileRemover.RemoveFolder(strDeletionFolder);
}

//*********************************************************************

bool CLoyaltySemaphore::LockServerOuter(int nLockNumber)
{
	return AcquireLock(m_lockServerOuter, nLockNumber, m_LoyaltySemaphoreThreadInfo.m_bSeenServerOuterLockFile, m_LoyaltySemaphoreThreadInfo.m_bGotServerOuterLock);
}

//*********************************************************************

bool CLoyaltySemaphore::LockServerInner(int nLockNumber)
{
	return AcquireLock(m_lockServerInner, nLockNumber, m_LoyaltySemaphoreThreadInfo.m_bSeenServerInnerLockFile, m_LoyaltySemaphoreThreadInfo.m_bGotServerInnerLock);
}

//*********************************************************************

bool CLoyaltySemaphore::LockBackground(int nLockNumber)
{
	return AcquireLock(m_lockBackground, nLockNumber, m_LoyaltySemaphoreThreadInfo.m_bSeenBackgroundLockFile, m_LoyaltySemaphoreThreadInfo.m_bGotBackgroundLock);
}

//*********************************************************************

bool CLoyaltySemaphore::AcquireLock(CLoyaltySemaphoreLockDetails& LockDetails, int nLockNumber, bool& bThreadSeenLock, bool& bThreadGotLock)
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

void CLoyaltySemaphore::UnlockServerOuter(int nLockNumber)
{
	ReleaseLock(nLockNumber, m_lockServerOuter, m_LoyaltySemaphoreThreadInfo.m_bGotServerOuterLock);
}

//*********************************************************************

void CLoyaltySemaphore::UnlockServerInner(int nLockNumber)
{
	ReleaseLock(nLockNumber, m_lockServerInner, m_LoyaltySemaphoreThreadInfo.m_bGotServerInnerLock);
}

//*********************************************************************

void CLoyaltySemaphore::UnlockBackground(int nLockNumber)
{
	ReleaseLock(nLockNumber, m_lockBackground, m_LoyaltySemaphoreThreadInfo.m_bGotBackgroundLock);
}

//*********************************************************************

void CLoyaltySemaphore::ReleaseLock(int nLockNumber, CLoyaltySemaphoreLockDetails& LockDetails, bool& bThreadGotLock)
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

void CLoyaltySemaphore::Diagnostic(CLoyaltySemaphoreLockDetails& LockDetails, CString strMsg)
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
