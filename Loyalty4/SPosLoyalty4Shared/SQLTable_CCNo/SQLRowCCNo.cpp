//**********************************************************************
#include "SQLFieldValidatorCCNo.h"
//**********************************************************************
#include "SQLRowCCNo.h"
//**********************************************************************

CSQLRowCCNo::CSQLRowCCNo()
{
	Reset();
}

//**********************************************************************

void CSQLRowCCNo::Reset()
{
	m_nType = CCNO_TYPE_UNKNOWN;
	m_nNumber = 0;
}

//**********************************************************************

CString CSQLRowCCNo::GetTypeString()
{
	CString strType = "";

	switch (m_nType)
	{
	case CCNO_TYPE_SERVER:
		strType = CCNoTypeNames::Server;
		break;

	case CCNO_TYPE_PC:
		strType = CCNoTypeNames::PC;
		break;

	case CCNO_TYPE_POS:
		strType = CCNoTypeNames::POS;
		break;

	case CCNO_TYPE_EXTERNAL:
		strType = CCNoTypeNames::External;
		break;

	case CCNO_TYPE_UNKNOWN:
	default:
		strType = CCNoTypeNames::Other;
		break;
	}

	return strType;
}

//**********************************************************************

void CSQLRowCCNo::SetTypeString(CString strType)
{
	if (strType.CompareNoCase(CCNoTypeNames::Server) == 0)
	{
		m_nType = CCNO_TYPE_SERVER;
	}
	else if (strType.CompareNoCase(CCNoTypeNames::PC) == 0)
	{
		m_nType = CCNO_TYPE_PC;
	}
	else if (strType.CompareNoCase(CCNoTypeNames::POS) == 0)
	{
		m_nType = CCNO_TYPE_POS;
	}
	else if (strType.CompareNoCase(CCNoTypeNames::External) == 0)
	{
		m_nType = CCNO_TYPE_EXTERNAL;
	}
	else
	{
		m_nType = CCNO_TYPE_UNKNOWN;
	}
}

//**********************************************************************

void CSQLRowCCNo::IncrementCCNo()
{
	m_nNumber++;

	int nCCNoMax = (CCNO_TYPE_EXTERNAL == m_nType) ? 99999999 : 9999;

	if (m_nNumber > nCCNoMax)
	{
		m_nNumber = 1;
	}
}

//**********************************************************************


