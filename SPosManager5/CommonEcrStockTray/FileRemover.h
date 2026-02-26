#pragma once
/**********************************************************************/
#include "..\CommonEcrStock\BackupProgressDlg.h"
/**********************************************************************/

class CFileRemover  
{
public:
	CFileRemover();
	CFileRemover( const char* szFile );

public:
	bool RemoveFile( const char* szFile );
	void RemoveFolder( const char* szFolder );
	void RemoveFolderContents( const char* szFolder, int& nFileNo ); 

private:
	bool IsProtected( const char* szFile );
	virtual void UpdateDisplay( int nFileNo ){}
};

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CFileRemoverBackup : public CFileRemover
{
public:
	CFileRemoverBackup( CBackupProgressDlg* pInfoDlg );

private:
	virtual void UpdateDisplay( int nFileNo );

private:
	CBackupProgressDlg* m_pInfoBackupDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/

