//*******************************************************************
#include "FilenameHandler.h"
//*******************************************************************

CString CFilenameHandler::GetSQLConnectionOptionsFilename()
{
	CString strFilename = "";
	strFilename += Sysset.GetDataPath();
	strFilename += "\\SQL\\SQLCONNECT.INI";
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetSQLDatabaseFilename()
{
	CString strResult = GetFilename("SysFiles", "SmartPay.db");
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetFilename(const CString strFolder, const CString strFilename)
{
	CString strResult;

	if (strFilename.GetLength() == 0)
	{
		strResult.Format("%s\\%s", (const char*)Sysset.GetProgramPath(), (const char*)strFolder);
	}
	else
	{
		strResult.Format("%s\\%s\\%s", (const char*)Sysset.GetProgramPath(), (const char*)strFolder, (const char*)strFilename);
	}

	return strResult;
}

//*******************************************************************

