//$$******************************************************************
#include "LoyaltyCommandLineInfo.h"
//$$******************************************************************

CLoyaltyCommandLineInfo::CLoyaltyCommandLineInfo() : CCommandLineInfo()
{
}

//$$******************************************************************

void CLoyaltyCommandLineInfo::ParseParam(const char* szParam, BOOL bFlag, BOOL bLast)
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
