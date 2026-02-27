//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************

#include "SystemData.h"
#include "GlobalFunctions.h"
#include "ImageHandler.h"

//*******************************************************************

CImageHandler::CImageHandler ( const char* szPath, const char* szFilename )
{
	m_strFilename = "";
	m_strFilenameMask.Format ( "%s\\%s.*", szPath, szFilename );
}

//*******************************************************************

bool CImageHandler::GetFirst()
{
	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( m_strFilenameMask );

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

//*******************************************************************

