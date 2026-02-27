//*********************************************************************
#include "FileRecycler.h"
//*********************************************************************

void CFileRecycler::SendToRecycleBin ( const char* szFileMask )
{
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile ( szFileMask );

	while (bWorking)   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFilePath();
		::remove ( strFilename );	
	}
}

//*******************************************************************
