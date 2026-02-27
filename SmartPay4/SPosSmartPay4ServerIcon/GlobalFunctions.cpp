//*******************************************************************
#include <chrono>
#include <iomanip>
#include <sstream>
//*******************************************************************
#include "Globalfunctions.h"
//*******************************************************************

bool FileExists(const char* szFile) { return (_access(szFile, 0) == 0) ? TRUE : FALSE; }

//*******************************************************************

void TrimSpacesFromString(CString& strText, bool bRemoveLeadingZeros)
{
	strText.TrimLeft(' ');
	strText.TrimRight(' ');

	if (TRUE == bRemoveLeadingZeros)
	{
		strText.TrimLeft('0');
	}
}

//*******************************************************************

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

CString GetTidyAppName( bool bDummy )
{
	return "Loyalty Server Icon"; 
}

/**********************************************************************/

CString GetRunOnceFilename()
{
	CString strFilename = "";

#ifdef BGNDICON
	strFilename.Format("TRAY2_%10.10d", Sysset.GetPCIdHost());
#else
	strFilename.Format("TRAY1_%10.10d", Sysset.GetPCIdHost());
#endif

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
	strTimeStamp.Format("%s_%15.15I64d",
		(const char*) GetSimpleDateTimeNowString(),
		GetTickCount64());

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

CString GetSimpleDateTimeNowString()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strDateTime = "";
	strDateTime.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	return strDateTime;
}

/**********************************************************************/

void GetMessageLogDateTime(CString& strDate, CString& strTime)
{
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &t);

	// Format date as DD/MM/YYYY
	std::ostringstream ossDate;
	ossDate << std::setw(2) << std::setfill('0') << tm.tm_mday << "/"
		<< std::setw(2) << std::setfill('0') << (tm.tm_mon + 1) << "/"
		<< std::setw(4) << std::setfill('0') << (tm.tm_year + 1900);
	strDate = CString(ossDate.str().c_str());

	// Format time as HH:MM:SS.mmm
	std::ostringstream ossTime;
	ossTime << std::setw(2) << std::setfill('0') << tm.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << tm.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << tm.tm_sec << "."
		<< std::setw(3) << std::setfill('0') << ms.count();
	strTime = CString(ossTime.str().c_str());
}

/**********************************************************************/