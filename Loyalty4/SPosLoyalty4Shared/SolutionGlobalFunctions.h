#pragma once
//**********************************************************************

namespace SolutionGlobalFunctions
{
	bool FileExists(const char* szFile);
	void GetMessageLogDateTime(CString& strDate, CString& strTime);
	void TrimSpacesFromString(CString& strText, bool bRemoveLeadingZeros);
};

//**********************************************************************
