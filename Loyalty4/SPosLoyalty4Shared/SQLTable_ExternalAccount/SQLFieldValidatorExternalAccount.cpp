//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorExternalAccount.h"
//**********************************************************************

void CSQLFieldValidatorExternalAccount::SetAESUsername(CString str, CString& strTarget)
{
	SetString(str, strTarget, ExternalAccount::Username);
}

//**********************************************************************

void CSQLFieldValidatorExternalAccount::SetPasswordHash(CString str, CString& strTarget)
{
	SetString(str, strTarget, ExternalAccount::Password);
}

//**********************************************************************

void CSQLFieldValidatorExternalAccount::SetAESUserID(CString str, CString& strTarget)
{
	SetString(str, strTarget, ExternalAccount::UserID);
}

//**********************************************************************

