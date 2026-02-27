//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorExternal.h"
//**********************************************************************

void CSQLFieldValidatorExternal::SetAESUsername(CString str, CString& strTarget)
{
	SetString(str, strTarget, ExternalAccount::Username);
}

//**********************************************************************

void CSQLFieldValidatorExternal::SetPasswordHash(CString str, CString& strTarget)
{
	SetString(str, strTarget, ExternalAccount::Password);
}

//**********************************************************************

void CSQLFieldValidatorExternal::SetAESUserID(CString str, CString& strTarget)
{
	SetString(str, strTarget, ExternalAccount::UserID);
}

//**********************************************************************

