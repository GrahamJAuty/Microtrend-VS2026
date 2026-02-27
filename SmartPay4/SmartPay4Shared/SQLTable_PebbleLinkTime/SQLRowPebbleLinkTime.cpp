//**********************************************************************
#include "SQLRowPebbleLinkTime.h"
//**********************************************************************

CSQLRowPebbleLinkTime::CSQLRowPebbleLinkTime()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebbleLinkTime::Reset()
{
	m_nLineID = 1;
	m_strDate = "";
	m_strTime = "";
	m_nUserID = 0;
	m_strUsername = "";
	m_strForename = "";
	m_strSurname = "";
	m_nGroupNo = 0;
	m_strPebbleFirstName = "";
	m_strPebbleLastName = "";
	m_nProcessType = 0;
	m_nMatchType = 0;
	m_strMIS = "";
	m_strUPN = "";
}

//**********************************************************************

