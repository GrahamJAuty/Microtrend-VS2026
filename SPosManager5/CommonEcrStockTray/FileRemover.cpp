/**********************************************************************/
#include <direct.h>
#include <ErrNo.h>
/**********************************************************************/
#include "FileRemover.h"
/**********************************************************************/

CFileRemover::CFileRemover()
{
}

/**********************************************************************/

CFileRemover::CFileRemover( const char* szFile )
{
	RemoveFile( szFile );
}

/**********************************************************************/

bool CFileRemover::RemoveFile( const char* szFile )
{
	if ( IsProtected( szFile ) == TRUE )
		return TRUE;

	CString strFile = szFile;

	if ( remove ( strFile ) == 0 )
		return TRUE;

	if ( errno != EACCES )
		return FALSE;

	DWORD dwAtt = GetFileAttributes( strFile );

	if ( dwAtt == 0xFFFFFFFF )
		return FALSE;

	if ( ( dwAtt & FILE_ATTRIBUTE_READONLY ) == 0 )
		return FALSE;

	dwAtt -= FILE_ATTRIBUTE_READONLY;

	if ( SetFileAttributes( strFile, dwAtt ) == FALSE )
		return FALSE;

	return ( remove( strFile ) == 0 );
}

/**********************************************************************/

void CFileRemover::RemoveFolder ( const char* szFolder )
{
	CString strFolder = szFolder;
	::TrimSpaces ( strFolder, FALSE );
	
	if ( strFolder == "" )
		return;

	int nFileNo = 1;
	RemoveFolderContents ( strFolder, nFileNo );
	_rmdir ( strFolder );
}

/**********************************************************************/

void CFileRemover::RemoveFolderContents ( const char* szFolder, int& nFileNo )
{
	CStringArray strFileNames;
	CByteArray bDirectoryFlags;

	CString strFolder = szFolder;
	::TrimSpaces ( strFolder, FALSE );
	
	if ( strFolder == "" )
		return;

	strFolder += "\\*.*";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strFolder ) != 0 );

	while (bWorking)   
	{
		bWorking = ( FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		CString strNewPath = FileFinder.GetFilePath();
		strFileNames.Add ( strNewPath );
				
		if ( FileFinder.IsDirectory() == TRUE )
		{
			RemoveFolderContents ( strNewPath, nFileNo );
			bDirectoryFlags.Add ( TRUE );
		}
		else
			bDirectoryFlags.Add ( FALSE );
	}

	for ( int nIndex = 0; nIndex < strFileNames.GetSize(); nIndex++ )
	{
		if ( bDirectoryFlags.GetAt ( nIndex ) == TRUE )
			_rmdir ( strFileNames.GetAt ( nIndex ) );
		else
			RemoveFile( strFileNames.GetAt( nIndex ) );

		UpdateDisplay( nFileNo++ );
	}
}

/**********************************************************************/

bool CFileRemover::IsProtected( const char* szFile )
{
	CString strFile1 = szFile;
	strFile1.MakeUpper();

	CStringArray arrayTest;
	//FIDDLE - ADD FILENAMES HERE TO PROTECT FROM DELETION
	
	for ( int n = 0; n < arrayTest.GetSize(); n++ )
	{
		CString strFile2 = arrayTest.GetAt(n);
		strFile2.MakeUpper();

		int nLen = strFile2.GetLength();
		if ( strFile1.GetLength() >= nLen )
		{
			if (strFile2 == strFile1.Right(nLen))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

CFileRemoverBackup::CFileRemoverBackup( CBackupProgressDlg* pInfoDlg )
{
	m_pInfoBackupDlg = pInfoDlg;
}

/**********************************************************************/

void CFileRemoverBackup::UpdateDisplay( int nFileNo )
{
	m_pInfoBackupDlg -> SetText( FALSE, BACKUP_PROGRESS_DELETEFILE, nFileNo );
}

/**********************************************************************/
#endif
/**********************************************************************/
