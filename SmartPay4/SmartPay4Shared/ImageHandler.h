#pragma once
//*******************************************************************
#include "ServerData.h"
//*******************************************************************

class CImageHandler
{
public:
	CImageHandler ( bool bForRSPFile );
	CString GetImageFilepath(CString strCardNo);
	void CopyFile ( CString strFromCardNo, CString strToCardNo );
	void DeleteFile ( CString strCardNo );

private:
	CString GetFolderPathInternal();
	CString GetFolderPathRSP();
	CString GetImageFilepathInternalNoExt(CString strCardNo);
	CString GetImageFilename(CString strCardNo);
	
private:
	bool m_bForRSPFile;
	bool m_bDoneServerRead;
};

//*******************************************************************
