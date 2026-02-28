/******************************************************************************/
#include <chrono>
#include <iomanip>
#include <sstream>
/******************************************************************************/
#include "SolutionGlobalFunctions.h"
/******************************************************************************/

bool SolutionGlobalFunctions::FileExists(const char* szFile)
{
	return (_access(szFile, 0) == 0) ? TRUE : FALSE;
}

/******************************************************************************/

void SolutionGlobalFunctions::TrimSpacesFromString(CString& strText, bool bRemoveLeadingZeros)
{
	strText.TrimLeft(' ');
	strText.TrimRight(' ');

	if (TRUE == bRemoveLeadingZeros)
	{
		strText.TrimLeft('0');
	}
}

/******************************************************************************/

void SolutionGlobalFunctions::GetMessageLogDateTime(CString& strDate, CString& strTime)
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

/******************************************************************************/
