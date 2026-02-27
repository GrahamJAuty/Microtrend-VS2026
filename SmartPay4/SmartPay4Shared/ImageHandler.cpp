//*******************************************************************
#include "ImageHandler.h"
//*******************************************************************
extern CSysset Sysset;
//*******************************************************************

CImageHandler::CImageHandler ( bool bForRSPFile )
{
	m_bForRSPFile = bForRSPFile;
	m_bDoneServerRead = FALSE;
}

//*******************************************************************

CString CImageHandler::GetFolderPathInternal()
{
	CString strFolderPath = "";

	if (Server.GetPhotoIDFolderType() != 2)
	{
		CString strDataPath = Sysset.GetDataPath();

		if (strDataPath != "")
		{
			// full path - no extension
			strFolderPath.Format("%s%s\\PhotoID\\",
				(const char*)strDataPath,
				(const char*)Sysset.GetProgramFolder(SYSSET_SMARTPAY4));
		}
	}
	else
	{
		if ((FALSE == m_bForRSPFile) && (FALSE == m_bDoneServerRead))
		{
			Server.Read(Filenames.GetServerDataFilename());
			m_bDoneServerRead = TRUE;
		}

		strFolderPath = Server.GetSmartPayUNCImagePath();

		if (strFolderPath != "")
		{
			strFolderPath += "\\";
		}
	}

	return strFolderPath;
}

//*******************************************************************

CString CImageHandler::GetFolderPathRSP()
{
	CString strFolderPath = "";

	if ((FALSE == m_bForRSPFile) && (FALSE == m_bDoneServerRead))
	{
		Server.Read(Filenames.GetServerDataFilename());
		m_bDoneServerRead = TRUE;
	}

	if (Server.GetPhotoIDFolderType() != 2)
	{
		CString strDataPath = Server.GetSmartPayUNCSystemPath();

		if (strDataPath != "")
		{
			strDataPath += "\\";
		}
		
		if (strDataPath != "")
		{
			// full path - no extension
			strFolderPath.Format("%s%s\\PhotoID\\",
				(const char*)strDataPath,
				(const char*)Sysset.GetProgramFolder(SYSSET_SMARTPAY4));
		}
	}
	else
	{
		strFolderPath = Server.GetSmartPayUNCImagePath();

		if (strFolderPath != "")
		{
			strFolderPath += "\\";
		}
	}

	return strFolderPath;
}

//*******************************************************************

CString CImageHandler::GetImageFilepathInternalNoExt(CString strCardNo)
{
	CString strPath = "";
	
	strPath.Format("%s%s",
		(const char*)GetFolderPathInternal(),
		(const char*)System.FormatCardNo(strCardNo));

	return strPath;
}

//*******************************************************************

CString CImageHandler::GetImageFilename(CString strCardNo)
{
	CString strFilename = "";

	CString strMask = GetImageFilepathInternalNoExt(strCardNo);			// "<path>\\P44\\PhotoID\\000000xxxx"  - no file extension.bmp"

	if (strMask != "")
	{
		strMask += ".*";

		CFileFind FileFinder;
		BOOL bWorking = FileFinder.FindFile(strMask);

		while (bWorking)
		{
			bWorking = FileFinder.FindNextFile();
			if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
			{
				continue;
			}

			strFilename = FileFinder.GetFileName();
			strFilename.MakeUpper();
			if (strFilename == "THUMBS.DB")							// skip windows generated file 
			{
				continue;
			}

			break;
		}
	}

	return strFilename;
}

//*******************************************************************

CString CImageHandler::GetImageFilepath(CString strCardNo)
{
	CString strFilepath = "";
	CString strFilename = GetImageFilename(strCardNo);

	if (strFilename != "")
	{
		if (TRUE == m_bForRSPFile)
		{
			strFilepath += GetFolderPathRSP();
			strFilepath += strFilename;
		}
		else
		{
			strFilepath += GetFolderPathInternal();
			strFilepath += strFilename;
		}
	}

	return strFilepath;
}

//*******************************************************************

void CImageHandler::CopyFile ( CString strFromCardNo, CString strToCardNo )
{
	if (FALSE == m_bForRSPFile)
	{
		CString strFromFilename = GetImageFilepath(strFromCardNo);			// full name of first file found

		if (strFromFilename != "")
		{
			CString strToFilename = GetImageFilepathInternalNoExt(strToCardNo);	// full path - no extension

			strToFilename += '.';
			strToFilename += ::GetFileExtension(strFromFilename);

			::CopyFile(strFromFilename, strToFilename, FALSE);
		}
	}
}

//*******************************************************************

void CImageHandler::DeleteFile(CString strCardNo)
{
	if (FALSE == m_bForRSPFile)
	{
		CString strMask = GetImageFilepathInternalNoExt(strCardNo);		// no filename no extension"

		if (strMask != "")
		{
			strMask += ".*";
			CFileRecycler::SendToRecycleBin(strMask);
		}
	}
}

//*******************************************************************
