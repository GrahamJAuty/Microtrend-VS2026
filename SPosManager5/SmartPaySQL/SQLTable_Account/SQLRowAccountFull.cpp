//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************
#include "SQLRowAccountFull.h"
//**********************************************************************

CSQLRowAccountFull::CSQLRowAccountFull()
{
	Reset();
}

//**********************************************************************

CSQLRowAccountFull::CSQLRowAccountFull(const CSQLRowAccountFull& source)
{
	*this = source;
}

//**********************************************************************

CSQLRowAccountFull& CSQLRowAccountFull::operator= (const CSQLRowAccountFull& source)
{
	m_nUserID = source.m_nUserID;
	m_nGroupNo = source.m_nGroupNo;
	m_strInfo3 = source.m_strInfo3;
	m_strInfo4 = source.m_strInfo4;
	return *this;
}

//**********************************************************************

void CSQLRowAccountFull::Reset()
{
	m_nUserID = 0;
	m_nGroupNo = 1;
	m_strInfo3 = "";
	m_strInfo4 = "";
}

//**********************************************************************

CString CSQLRowAccountFull::GetUserIDString()
{
	CString str = "";
	str.Format("%I64d", m_nUserID);	
	return str;
}

//**********************************************************************

