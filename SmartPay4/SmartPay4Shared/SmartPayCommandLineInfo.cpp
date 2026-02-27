//$$******************************************************************
#include "SmartPayCommandLineInfo.h"
//$$******************************************************************

CSmartPayCommandLineInfo::CSmartPayCommandLineInfo(bool bAllowEmailMode) : CCommandLineInfo()
{
	m_bAllowEmailMode = bAllowEmailMode;
	m_bEmailMode = FALSE;
}

//$$******************************************************************

void CSmartPayCommandLineInfo::ParseParam(const char* szParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag == TRUE)					// see if a flag ( strips of  '/' character )
	{
		CString strParam = szParam;

		if ( (strParam == "EMAIL") && ( TRUE == m_bAllowEmailMode))
		{
			m_bEmailMode = TRUE;
		}
	}
	else
	{
		m_arrayParam.Add(szParam);
	}

	CCommandLineInfo::ParseParam(szParam, bFlag, bLast);
}

//$$******************************************************************
