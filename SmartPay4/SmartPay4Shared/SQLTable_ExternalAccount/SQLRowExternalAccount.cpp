//**********************************************************************
#include "SQLRowExternalAccount.h"
//**********************************************************************

CSQLRowExternalAccount::CSQLRowExternalAccount()
{
	Reset();
}

//**********************************************************************

void CSQLRowExternalAccount::Reset()
{
	m_strAESUsername = "";
	m_strPasswordHash = "";
	m_strAESUserID = "";
}

//**********************************************************************

