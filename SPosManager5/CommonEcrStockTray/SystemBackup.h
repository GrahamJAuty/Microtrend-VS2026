#pragma once
/**********************************************************************/
#include "CkZip.h"
/**********************************************************************/
#include "..\CommonEcrStock\BackupProgressDlg.h"
/**********************************************************************/
#include "BackupFilter.h"
#include "FileRemover.h"
#include "LockManager.h"
/**********************************************************************/
#define BACKUP_FILE_INCLUDE_YES 0
#define BACKUP_FILE_INCLUDE_NO 1
/**********************************************************************/

class CSystemBackup
{	
public:
#ifndef POSTRAY_UTILITY
	CSystemBackup(bool bBackup, bool bRestoreToFolder, CLockManagerInfo& infoLock);
#endif
	
	CSystemBackup ( CString& strFilePath );

	bool IsComplete(){ return m_bComplete; }
	bool TerminatedLoyalty(){ return m_bTerminatedLoyalty; }
	const char* GetOnScreenError() { return m_strOnScreenError; }

private:
	void SetFolderPaths();
	void SetFolderPath(int nProgramPath, CString& strSystemData, CString& strRestoreSave);

private:
	void BackupFiles( const char* szFileName, const char* szPathName );

	void SetBackupFilename( bool bBackup );
	void CountFilesToBackup( const char* szLocalPath, __int64& nCountSoFar );

	bool CreateZipFile(CkZip& ZipArchive, const char* szLocalPath, __int64& nFilesSoFar, __int64& nFilesToBackup );

#ifndef POSTRAY_UTILITY
	void BackupFiles();
	void RestoreFiles( CLockManagerInfo& infoLock );
	void CleanUpRestore( CFileRemoverBackup& FileRemover );
	void CleanUpRestoreFolder(CFileRemoverBackup& FileRemover, CString& strDataFolder, CString& strSaveFolder);
	bool RemoveSaveFolder(CFileRemoverBackup& FileRemover, CString& strFolder, CString& strUserFilePath);
	bool RenameDataFolder(CString& strDataFolder, CString& strSaveFolder, CString& strUserFilePath, CString strFolderType);
#endif

	void LogBackupMessage ( const char* szMsg );
	void LogBackupMessage ( const char* szFilename, int nContext, int nError );
	void LogRestoreMessage ( const char* szFilename, int nContext, int nError );
	
	int GetFileIncludeType( const char* szLocalPath, const char* szFile );

	void AddGoodPath(int nProgramNo);

private:
	const char* GetDefaultBackupFolder();
	void SetDefaultBackupFolder( const char* sz );
	
private:
	bool m_bComplete;
	bool m_bTerminatedLoyalty;
	CString m_strOnScreenError;
	CString m_strBackupFilter;
	CString m_strRestoreFilter;
	CString m_strBackupFilename;
	CString m_strDrive;

private:
	CString m_strDefaultBackupFolder;

#ifndef POSTRAY_UTILITY
	CBackupProgressDlg m_BackupProgressDlg;
#endif

private:
	CString m_strFolderPathSystemData;
	CString m_strFolderPathRestoreSave;
	CString m_strFolderPathLoyaltyV2SystemData;
	CString m_strFolderPathLoyaltyV2RestoreSave;
	CString m_strFolderPathLoyaltyV3SystemData;
	CString m_strFolderPathLoyaltyV3RestoreSave;
	CString m_strFolderPathSmartPayV1SystemData;
	CString m_strFolderPathSmartPayV1RestoreSave;
	CString m_strFolderPathSmartPayV2SystemData;
	CString m_strFolderPathSmartPayV2RestoreSave;
	CString m_strFolderPathZipTemp;

private:
	CStringArray m_arrayGoodPaths;
	CBackupFilter m_BackupFilter;
	bool m_bAuto;

private:
	bool m_bRestoreToFolder;
};

/**********************************************************************/

