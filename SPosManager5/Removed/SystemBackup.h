#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "BackupFilter.h"
#include "FileRemover.h"
#include "LockManager.h"
#include "BackupProgressDlg.h"
/**********************************************************************/
#define BACKUP_FILE_INCLUDE_YES 0
#define BACKUP_FILE_INCLUDE_NO 1
#define BACKUP_FILE_INCLUDE_ABORT 2
/**********************************************************************/

class CSystemBackup
{	
public:
	CSystemBackup ( bool bBackup, bool bRestoreToFolder, CLockManagerInfo& infoLock );
	CSystemBackup ( CString& strFilePath );

	bool IsComplete(){ return m_bComplete; }
	bool TerminatedLoyalty(){ return m_bTerminatedLoyalty; }
	const char* GetOnScreenError() { return m_strOnScreenError; }

private:
	void BackupFiles();
	void BackupFiles( const char* szFileName, const char* szPathName );

	void SetBackupFilename( bool bBackup );
	void CountFilesToBackup( const char* szLocalPath, __int64& nCountSoFar );

	const char* GetMiniZipFilename( int n );
	const char* GetMiniZipExtendedFilename( int n );

	bool CreateMiniZipFiles( const char* szLocalPath, __int64& nFilesSoFar, __int64& nFilesToBackup );
	bool CreateZipFile( CString& strZipFilename, bool bSpanning, __int64& nFilesSoFar, __int64& nFilesToBackup, bool bShowProgress );
	int CreateZipFile( CString& strZipFilename, int nFileNumber, int& nFileListPos );

	void RestoreFiles( CLockManagerInfo& infoLock );
	bool RestoreFromZipFile( CString& strZipFilename, __int64& nFilesSoFar, __int64& nTarget, bool bShowProgress );
	void CleanUpRestore( CFileRemoverBackup& FileRemover );

	void LogBackupMessage ( const char* szMsg );
	void LogBackupMessage ( const char* szFilename, int nContext, int nFamily, int nError );
	void LogRestoreMessage ( const char* szFilename, int nContext, int nFamily, int nError );

	void OpenArchive ( int& nErrorFamily, int& nErrorNumber, const char* szPath, int nMode, int nVolumeSize );
	void CloseArchive ( int& nErrorFamily, int& nErrorNumber );
	void AddNewFile ( int& nErrorFamily, int& nErrorNumber, const char* szPath );
	void ExtractFile ( int& nErrorFamily, int& nErrorNumber, int nIndex );
	
	bool IsValidTargetFolder( CString& strFilename, CStringArray& arrayTargetFolders );

	static bool ChangeDisk( DWORD dwNumber, int iCode, void* pData);
	static bool BackupCallback ( DWORD dwFileSize, int nBytesRead, void *pData);

	int GetFileIncludeType( const char* szLocalPath, const char* szFile );

private:
	const char* GetDefaultBackupFolder();
	void SetDefaultBackupFolder( const char* sz );
	
private:
	__int64 m_nTotalFileSize;
	__int64 m_nProcessedSoFar;

private:
	bool m_bComplete;
	bool m_bTerminatedLoyalty;
	CString m_strOnScreenError;
	CZipArchive m_ZipArchive;
	CStringArray m_arrayFilenames;
	CString m_strBackupFilter;
	CString m_strRestoreFilter;
	CString m_strBackupFilename;
	CString m_strDrive;

private:
	CString m_strDefaultBackupFolder;
	CString m_strMiniZipFilename;
	CString m_strMiniZipExtendedFilename;
	int m_nNextZipFileNo;
	int m_nFilesPerMiniZip;
	CBackupProgressDlg m_BackupInfoDlg;

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
	CBackupFilter m_BackupFilter;
	bool m_bAuto;

private:
	bool m_bRestoreToFolder;
};

/**********************************************************************/
#endif
/**********************************************************************/
