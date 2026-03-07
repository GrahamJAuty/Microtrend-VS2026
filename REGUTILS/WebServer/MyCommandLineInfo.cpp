//$$******************************************************************
#include "MyCommandLineInfo.h"
//$$******************************************************************

CMyCommandLineInfo::CMyCommandLineInfo() : CCommandLineInfo()
{
}

//$$******************************************************************

void CMyCommandLineInfo::ParseParam(const char* szParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag == TRUE)					// see if a flag ( strips of  '/' character )
	{
		CString strParam = szParam;
	}
	else
	{
		m_arrayParam.Add(szParam);
	}

	CCommandLineInfo::ParseParam(szParam, bFlag, bLast);
}

//$$******************************************************************
