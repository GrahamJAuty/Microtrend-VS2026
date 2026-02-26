/**********************************************************************/
#include "PosTrayLock.h"
/**********************************************************************/

CPosTrayLock::CPosTrayLock()
{
	m_strFilename = "";
	m_bLockedByEcrman = FALSE;
	m_nLockedInstanceNo = 0;
	m_nLockType = 0;
	m_nLockTag = 0;
}

/**********************************************************************/

void CPosTrayLock::SetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PosTrayLockFile );
	m_strFilename = FnUp.GetFilenameToUse();

	CPosTrayLockFile LockFile;
	LockFile.m_bIsLocked = FALSE;

	for ( int n = 1; n <= Sysset.GetMaxUsers(); n++ )
	{
		LockFile.m_strFilename = FnUp.GetInstanceFilenameToUse( n );
		m_arrayFiles.Add( LockFile );
	}
}

/**********************************************************************/

CPosTrayLock::~CPosTrayLock()
{
	ClearLocks();
}

/**********************************************************************/

void CPosTrayLock::ClearLocks()
{
	for ( int n = 0; n < m_arrayFiles.GetSize(); n++ )
	{
		if ( TRUE == m_arrayFiles[n].m_bIsLocked )
		{
			m_arrayFiles[n].m_file.Close();
			m_arrayFiles[n].m_bIsLocked = FALSE;
		}
	}
}

/**********************************************************************/

bool CPosTrayLock::LockFile(int nLockType, __int64 nLockTag)
{
	if (m_strFilename == "")
	{
		return FALSE;
	}

	bool bAlreadyLocked = TRUE;
	for ( int n = 0; n < m_arrayFiles.GetSize(); n++ )
	{
		if ( FALSE == m_arrayFiles[n].m_bIsLocked )
		{
			bAlreadyLocked = FALSE;
			break;
		}
	}

	if ( TRUE == bAlreadyLocked )
	{
		LogError(0,m_nLockType, m_nLockTag, nLockType, nLockTag);
		m_bLockedByEcrman = FALSE;
		m_nLockType = nLockType;
		m_nLockTag = nLockTag;
		return TRUE;
	}

	ClearLocks();

	m_bLockedByEcrman = FALSE;
	for ( int n = 0; n < m_arrayFiles.GetSize(); n++ )
	{
		if ( m_arrayFiles[n].m_file.Open( m_arrayFiles[n].m_strFilename, "wb", _SH_DENYRW ) == TRUE )
		{
			m_arrayFiles[n].m_bIsLocked = TRUE;
		}
		else
		{
			m_bLockedByEcrman = TRUE;
			m_nLockedInstanceNo = n + 1;
			m_arrayFiles[n].m_bIsLocked = FALSE;
			ClearLocks();
			break;
		}
	}

	bool bGotLock = (FALSE == m_bLockedByEcrman);

	if (TRUE == bGotLock)
	{
		m_nLockType = nLockType;
		m_nLockTag = nLockTag;
	}

	return bGotLock;
}

/**********************************************************************/

void CPosTrayLock::UnlockFile(int nLockType, __int64 nLockTag)
{
	if ((nLockType != m_nLockType) || (nLockTag != m_nLockTag))
	{
		LogError(1, m_nLockType, m_nLockTag, nLockType, nLockTag);
	}
	else
	{
		ClearLocks();
		m_bLockedByEcrman = FALSE;
	}
}

/**********************************************************************/

void CPosTrayLock::LogError( int nErrorType, int nOldLockType, __int64 nOldLockTag, int nNewLockType, __int64 nNewLockTag )
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CSSFile fileLog;
	fileLog.Open(".\\PTLock.dat", "ab");

	CString strDate = "";;
	strDate.Format("%4.4d%2.2d%2.2d ",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay());

	CString strTime = "";;
	strTime.Format("%2.2d%2.2d%2.2d ",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	CString strError = "";
	switch (nErrorType)
	{
	case 0:	
		strError = "Already Locked ";
		break;

	default:
		strError = "Unknown Error ";
		break;
	}

	CString strInfo = "";
	switch (nErrorType)
	{
	case 0:
	case 1:
		strInfo.Format("%d %I64d %d %I64d",
			nOldLockType,
			nOldLockTag,
			nNewLockType,
			nNewLockTag);
		break;
	}

	fileLog.WriteLine(strDate + strTime + strError + strInfo);
}

/**********************************************************************/
