//*******************************************************************
#include "FilenameHandler.h"
//*******************************************************************

CString CFilenameHandler::GetFilename(const char* szFolder, const char* szFilename)
{
	CString strFilename;

	if (strlen(szFilename) == 0)
	{
		strFilename.Format("%s\\%s", 
			(const char*) Sysset.GetProgramPath(), 
			szFolder);
	}
	else
	{
		strFilename.Format("%s\\%s\\%s",
			(const char*) Sysset.GetProgramPath(),
			szFolder, 
			szFilename);
	}

	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogParamsFilename()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog\\ServerLog.prm",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogFilename()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog\\ServerLog.rep",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundLogFilename()
{
	CString strResult = "";

	strResult.Format("%s\\BgndLog\\BgndLog.rep",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundLogParamsFilename()
{
	CString strResult = "";

	strResult.Format("%s\\BgndLog\\BgndLog.prm",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetClientPCNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult;

	strResult.Format("%s\\FILE%4.4d%s.dat",
		(const char*)Sysset.GetPCPathClient(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetSysFilesNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult;

	strResult.Format("%s%4.4d%s.dat",
		(const char*)GetFilename("SysFiles", "File"),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogFolder()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundLogFolder()
{
	CString strResult = "";

	strResult.Format("%s\\BgndLog",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetTrayLogFilename()
{
	CString strResult = "";

#ifdef BGNDICON
	strResult += GetBackgroundLogFolder();
#else
	strResult += GetServerLogFolder();
#endif

	strResult += "\\TrayLog.rep";

	return strResult;
}

//*******************************************************************
