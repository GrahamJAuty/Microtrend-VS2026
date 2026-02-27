//******************************************************************************
#include "..\SmartPay4Shared\ImageHandler.h"
//******************************************************************************
#include "PhotoIDCopy.h"
//******************************************************************************

CPhotoIDCopy::CPhotoIDCopy()
{
}

//*******************************************************************

bool CPhotoIDCopy::CopyPhotoFile ( const char* szSourcePath, const char* szCardNo )
{
	CString strFilename;
	strFilename.Format ( "%s.%s", 
		(const char*) System.FormatCardNo(szCardNo), 
		(const char*) ::GetFileExtension(szSourcePath) );

	CString strDestPathname = Filenames.GetPhotoIDPath ( strFilename );
	return Copy ( szSourcePath, strDestPathname );
}

//*******************************************************************
// Import photoID files from '..\P44\Imports\PhotoID' folder

int CPhotoIDCopy::Import()
{
	int nCount = 0;

	CString strMask = Filenames.GetImportsPath ( "PhotoID\\*.*" );		// ..\P44\Imports\PhotoID\*.*
	
	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strMask );

	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		strFilename.MakeUpper();
		if (strFilename == "THUMBS.DB")								// skip windows generated file 
		{
			continue;
		}

		CString strSource		= FileFinder.GetFilePath();
		CString strDestination	= Filenames.GetPhotoIDPath ( FileFinder.GetFileName() );	// ..\\PhotoID\nnnnnn.xxx

		if ( Copy ( strSource, strDestination ) == TRUE ) 
		{
			++nCount;
			CFileRecycler::SendToRecycleBin ( strSource );
		}
	}

	return nCount;
}

//*******************************************************************
// copy photo file to PhotodID folder and to Exports\PhotoID folder

bool CPhotoIDCopy::Copy ( const char* szSourcePath, const char* szDestPathname )
{
	bool bReply = FALSE;

	if ( ::CopyFile ( szSourcePath, szDestPathname, FALSE ) !=  0 )		// return none zero = ok 
	{
		bReply = TRUE;
		if ( System.GetExportEposPhotoFlag() == TRUE)
		{
			CString strFilename = ::StripPathname ( szDestPathname );	// get just filename ( nnnnnn.xxx )

			CString strDestPathname;
			strDestPathname.Format ( "%s\\%s", 
				(const char*) System.GetEposPhotoIDFolder(), 
				(const char*) strFilename);

			::CopyFile ( szSourcePath, strDestPathname, FALSE );		// copy to Epos commsin folder
		}
	}	
	return bReply;
}

//*******************************************************************
//*******************************************************************
// copy any photo files from PhotoID folder to Exports\PhotoID folder
//*******************************************************************

int CPhotoIDCopy::EposExport()
{
	int nCount = 0;

	Server.Read( Filenames.GetServerDataFilename() );

	CString strMask = Filenames.GetPhotoIDPath ( "*.*" );
	
	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strMask );

	CString strDestination;
	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		strFilename.MakeUpper();
		if (strFilename == "THUMBS.DB")									// skip windows generated file 
		{
			continue;
		}

		CString strSource = FileFinder.GetFilePath();													// full name of Photo file
		strDestination.Format ( "%s\\%s", 
			(const char*) System.GetEposPhotoIDFolder(), 
			(const char*) FileFinder.GetFileName());	// nnnnnn.xxx

		if (::CopyFile(strSource, strDestination, FALSE) == TRUE)
		{
			++nCount;
		}
	}

	return nCount;
}

//********************************************************************