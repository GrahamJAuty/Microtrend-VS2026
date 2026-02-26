#pragma once
/**********************************************************************/
#include "AutoBackupOptions.h"
/**********************************************************************/
#include "LockManager.h"
/**********************************************************************/
#define BACKUP_REASON_NONE 0
#define BACKUP_REASON_SCHEDULED_HOURLY 1
#define BACKUP_REASON_SCHEDULED_DAILY 2
#define BACKUP_REASON_SCHEDULED_WEEKLY 3
#define BACKUP_REASON_SCHEDULED_CUSTOM 4
#define BACKUP_REASON_TIMESINCELAST 5
#define BACKUP_REASON_OVERDUE 6
#define BACKUP_REASON_FROMMENU 99
/**********************************************************************/

struct CAutoBackupResult
{
public:
	CAutoBackupResult();

public:
	COleDateTime m_oleDateTime;
	int m_nReason;
	int m_nResult;
	CString m_strError;
};

/**********************************************************************/

class CAutoBackupProcessor 
{
public:
	CAutoBackupProcessor();
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }

public:
	void ResetNextAttemptTime();
	bool CheckNextAttemptTime();
	bool CheckForcePromptTime();
	COleDateTime GetNextBackupTime(){ return m_OleNextBackupTime; }
	COleDateTime GetNextAttemptTime(){ return m_OleNextAttemptTime; }
	COleDateTime GetForcePromptTime(){ return m_OleForcePromptTime; }
	int GetNextAttemptReason();
	void TestPendingChanges( COleDateTime& oleTimeNextBackup, COleDateTime& oleTimeNextAttempt, COleDateTime& oleTimeForcePrompt, int& nReason );

public:
	void ScheduleNextBackup( bool bCompletedLastBackup );
	void ScheduleNextPrompt( int nWait, bool bForcePrompt );
	COleDateTime CalculateNextBackupTime();
	void NudgeNextAttemptTime( int nMinutes );
	const char* GetBackupReasonText( int nReason );

private:	
	int GetBackupReasonFromAutoBackupType();

public:
	void BuildAutoBackupList( CStringArray& arrayAutoBackup );
	void ExpireOldBackups();

public:
	void CheckAutoBackup( CWnd* pParent );
	void ManualBackup( CWnd* pParent );
	bool GetWantQuitFlag(){ return m_bWantQuit; }
	bool GetLoggedResultFlag(){ return m_bLoggedResult; }

public:
	int GetLastResultLineCount();
	const char* GetLastResultLine( int nIndex );
	const char* GetNextBackupLine();

private:
	void PromptAutoBackup( CWnd* pParent, bool bForcePrompt );
	int ProcessAutoBackup( CWnd* pParent );
	int ProcessAutoBackupInternal( CString& strOnScreenError );
	int ManualBackupInternal( CWnd* pParent );
	int ManualBackupInternal( CString& strOnScreenError, CLockManagerInfo& infoLock );
	void ProcessBackupResult( int nResult, bool bFromMenu );
	bool CheckZipFolder( const char* szPath, int& nVersion, CString& strExtra, int& nFileSize );

private:
	void LogResult( int Reason, int nResult, const char* szError );

private:
	void TraceTime( const char* szText, COleDateTime& dateTime );
	void TraceStatus( COleDateTime timeNow, const char* szHeader );

private:
	COleDateTime m_OleNextBackupTime;
	COleDateTime m_OleNextAttemptTime;
	COleDateTime m_OleForcePromptTime;
	int m_nNextPromptReason;

private:
	bool m_bReadyToGo;
	bool m_bWantQuit;
	bool m_bLoggedResult;

private:
	CArray<CAutoBackupResult,CAutoBackupResult> m_arrayResults;
	CString m_strLastResultLine;
	CString m_strNextBackupLine;
	CString m_strReason;
};

/**********************************************************************/
extern CAutoBackupProcessor AutoBackupProcessor; 
/**********************************************************************/

