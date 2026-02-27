#pragma once
/**********************************************************************/
#include "CkZip.h"
/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
/**********************************************************************/
#include "BackupFilter.h"
#include "FileRemoverBackup.h"
#include "LockManager.h"
#include "BackupProgressDlg.h"
/**********************************************************************/
#define BACKUP_FILE_INCLUDE_YES 0
#define BACKUP_FILE_INCLUDE_NO 1
/**********************************************************************/

class CSystemBackup
{	
public:
	CSystemBackup ( bool bBackup, bool bRestoreToFolder, bool bFromScript, CLockManagerInfo& infoLock );
	CSystemBackup ( CString& strFilePath );

	bool IsComplete(){ return m_bComplete; }
	bool TerminatedSmartPay(){ return m_bTerminatedSmartPay; }
	const char* GetOnScreenError() { return m_strOnScreenError; }

private:
	void BackupFiles();
	void BackupFiles( const char* szFileName, const char* szPathName );

	void SetBackupFilename( bool bBackup );
	void CountFilesToBackup( const char* szLocalPath, __int64& nCountSoFar );

	bool CreateZipFile(CkZip& ZipArchive, const char* szLocalPath, __int64& nFilesSoFar, __int64& nFilesToBackup );

	void RestoreFiles( CLockManagerInfo& infoLock );
	void CleanUpRestore( CFileRemoverBackup& FileRemover );

	void LogBackupMessage ( const char* szMsg );
	void LogBackupMessage ( const char* szFilename, int nContext, int nError );
	void LogRestoreMessage ( const char* szFilename, int nContext, int nError );
	
	int GetFileIncludeType( const char* szLocalPath, const char* szFile );

private:
	const char* GetDefaultBackupFolder();
	void SetDefaultBackupFolder( const char* sz );
	
private:
	bool m_bComplete;
	bool m_bTerminatedSmartPay;
	CString m_strOnScreenError;
	CString m_strBackupFilter;
	CString m_strRestoreFilter;
	CString m_strBackupFilename;
	CString m_strDrive;

private:
	CString m_strDefaultBackupFolder;
	CBackupProgressDlg m_BackupProgressDlg;

private:
	CString m_strFolderPathSystemData;
	CString m_strFolderPathRestoreSave;
	CString m_strFolderPathZipTemp;

private:
	CBackupFilter m_BackupFilter;
	bool m_bAuto;

private:
	bool m_bRestoreToFolder;
	bool m_bFromScript;
};

/**********************************************************************/
