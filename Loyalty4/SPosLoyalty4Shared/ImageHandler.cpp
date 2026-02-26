/******************************************************************************/
#include "FileRemover.h"
/******************************************************************************/
#include "ImageHandler.h"
/******************************************************************************/

extern CSysset Sysset;

//*******************************************************************

/*
CImageHandler::CImageHandler( const char* szCardNo, const char* szUNCPath )
{
	CString strCardNo		= szCardNo;
	CString strPath			= szUNCPath;
	CString strPnnFolder	= Sysset.GetProgramFolder ( SYSSET_LOYALTYX500V2 );

	AddLeading ( strCardNo, Account::CardNo.Max, '0' );

	if ( strPath == "" )	strPath = Sysset.GetDataPath(); // "<data path\>"		   - used by LoyaltyManager
	else					strPath += '\\';				// "\\ComputerName\Folder" - used by LoyaltyServer

	m_strFullFileTitle.Format ( "%s%s\\PhotoID\\%s", strPath, strPnnFolder, strCardNo );	// full path - no extension
}

//*******************************************************************

bool CImageHandler::GetFirst()
{
	CString strFilenameMask = m_strFullFileTitle + ".*";

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strFilenameMask );

	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		m_strFilename = FileFinder.GetFilePath();
		return TRUE;
	}

	return FALSE;
}
*/

//*******************************************************************

CImageHandler::CImageHandler ( const char* szUNCPath )
	: m_strDataPath(szUNCPath)
{
	if ( m_strDataPath == "" )	m_strDataPath = Sysset.GetDataPath();	// "<data path\>"		   - used by LoyaltyManager
	else						m_strDataPath += '\\';					// "\\ComputerName\Folder" - used by LoyaltyServer
}

//***************************************************************
// return   "<path>\\P33\\PhotoID\\000000xxxx"  - no file extension.bmp"

CString CImageHandler::GetImageFileTitle ( const char* szCardNo )
{
	CString strFileTitle;

	Server.Read( Filenames.GetServerDataFilename() );

	if ( Server.GetPhotoIDFolderType() != 2 )
		strFileTitle.Format ( "%s%s\\PhotoID\\%s", m_strDataPath, Sysset.GetProgramFolder(SYSSET_LOYALTY4), System.FormatCardNo(szCardNo) );	// full path - no extension
	else
		strFileTitle.Format ( "%s\\%s", Server.GetLoyaltyUNCImagePath(), System.FormatCardNo(szCardNo) );
	
	return strFileTitle;
}

//*******************************************************************

CString CImageHandler::GetFirst ( const char* szCardNo )
{
	CString strFilename = "";

	CString strMask = GetImageFileTitle ( szCardNo );			// "<path>\\P33\\PhotoID\\000000xxxx"  - no file extension.bmp"
	strMask += ".*";

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strMask );

	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		strFilename = FileFinder.GetFilePath();				// full file name +.ext
		break;
	}

	return strFilename;
}

//*******************************************************************

void CImageHandler::DeleteAllExcept ( const char* szCardNo, CString& strExtToSave )
{
	strExtToSave.MakeUpper();

	CStringArray arrayFilesToDelete;

	CString strMask = GetImageFileTitle ( szCardNo );			// "<path>\\P33\\PhotoID\\000000xxxx"  - no file extension.bmp"
	strMask += ".*";

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strMask );

	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFilePath();				
		
		bool bKeepFile = FALSE;
		CString strExt = strFilename.Right(4);
		if ( strExt.Left(1) == "." )
		{
			strExt.MakeUpper();
			if ( strExtToSave == strExt.Right(3) )
				bKeepFile = TRUE;
		}

		if ( FALSE == bKeepFile )
			arrayFilesToDelete.Add(strFilename);
	}

	for ( int n = 0; n < arrayFilesToDelete.GetSize(); n++ )
		CFileRemover FileRemover( arrayFilesToDelete.GetAt(n) );
}

//*******************************************************************

void CImageHandler::CopyFile ( const char* szFromCardNo, const char* szToCardNo )
{
	bool bReply = FALSE;

	CString strFromFilename = GetFirst ( szFromCardNo );			// full name of first file found

	if ( strFromFilename != "" )
	{
		CString strToFilename = GetImageFileTitle ( szToCardNo );	// full path - no extension

		strToFilename += '.';
		strToFilename += ::GetFileExtension ( strFromFilename );

		::CopyFile ( strFromFilename, strToFilename, FALSE );
	}
}

//*******************************************************************

void CImageHandler::DeleteFile ( const char* szCardNo )
{
	CString strMask = GetImageFileTitle ( szCardNo );		// no filename no extension"
	strMask += ".*";

	::SendToRecycleBin ( strMask );
}

//*******************************************************************
