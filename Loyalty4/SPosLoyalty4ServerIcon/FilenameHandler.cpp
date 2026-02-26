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

CString CFilenameHandler::GetServerLogFolder()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog",
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

CString CFilenameHandler::GetTrayLogFilename()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog\\TrayLog.rep",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetHostPCNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s\\FILE%4.4d%s.dat",
		(const char*)Sysset.GetPCPathHost(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetHostPCNumericFilenameOld(int nFileNum, CString strSuffix)
{
	CString strResult;

	strResult.Format("%s%4.4d%s.dat",
		(const char*)Sysset.GetPCPathHost(),
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

/*
CString CFilenameHandler::GetRecordInuseFilename(const char* szCardNo)
{
	CString strFilename;	strFilename.Format("%s\\SysFiles\\ed1-%s.run", Sysset.GetProgramPath(), System.FormatCardNo(szCardNo));
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetFamilyPath(const char* szAccount)
{
	CString strAccount = szAccount;
	if (strAccount != "")		strAccount.Format("%s.dat", System.FormatCardNo(szAccount));

	return GetFilename("Family", strAccount);
}
*/

//*******************************************************************
