#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
 
struct CPosTrayLockFile
{
public:
	CPosTrayLockFile()
	{
		m_bIsLocked = FALSE;
		m_strFilename = "";
	}

public:
	CSSFile m_file;
	bool m_bIsLocked;
	CString m_strFilename;
};

/**********************************************************************/

class CPosTrayLock 
{
public:
	CPosTrayLock();
	~CPosTrayLock();

public:
	void SetFilename();

public:
	bool LockFile( int nLockType, __int64 nLockTag );
	void UnlockFile(int nLockType, __int64 nLockTag );

public:
	bool LockedByEcrman(){ return m_bLockedByEcrman; }
	int GetLockedInstanceNo(){ return m_nLockedInstanceNo; }

private:
	void ClearLocks();

private:
	void LogError(int nErrorType, int nOldLockType, __int64 nOldLockTag, int nNewLockType, __int64 nNewLockTag);
	
private:
	CString m_strFilename;
	bool m_bLockedByEcrman;
	int m_nLockedInstanceNo;

private:
	CArray<CPosTrayLockFile,CPosTrayLockFile> m_arrayFiles;

private:
	int m_nLockType;
	__int64 m_nLockTag;
};

/**********************************************************************/
extern CPosTrayLock PosTrayLock;
/**********************************************************************/
#endif
/**********************************************************************/
