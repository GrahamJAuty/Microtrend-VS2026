//*******************************************************************
#include <chrono>
#include <iomanip>
#include <sstream>
/******************************************************************************/
#include "Globalfunctions.h"
//*******************************************************************

void GetPCPathHost(CString& strPath)
{
	CString strTemp = Sysset.GetPCPathHost();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetPCPathClient(CString& strPath)
{
	CString strTemp = Sysset.GetPCPathClient();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetDataProgramPath(CString& strPath)
{
	CString strTemp = Sysset.GetProgramPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

CString GetTidyAppName(bool bDummy)
{
	return "Loyalty Tray Icon";
}

/**********************************************************************/

CString GetRunOnceFilename()
{
	CString strFilename = "";
	strFilename.Format("TRAY1_%10.10d", Sysset.GetPCIdHost());

	CString strPath = Sysset.GetSyssetProgramPath();
	strPath += "\\";
	strPath += strFilename;

	return strPath;
}

/**********************************************************************/

CString CreateRunOnceTimeStamp()
{	
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	
	CString strTimeStamp = "";
	strTimeStamp.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d_%15.15I64d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond(),
		GetTickCount64() );

	CSSFile fileTimeStamp;
	if (fileTimeStamp.Open(GetRunOnceFilename(), "wb") == TRUE)
	{
		fileTimeStamp.WriteLine(strTimeStamp);
	}
	else
	{
		strTimeStamp = "";
	}

	return strTimeStamp;
}

/**********************************************************************/

void CreateSubdirectory(const char* szDir)
{
	CString strDir = szDir;
	if (::ExistSubdirectory(strDir) == FALSE)
	{
		::MakeSubdirectory(strDir);
	}
}

/**********************************************************************/
