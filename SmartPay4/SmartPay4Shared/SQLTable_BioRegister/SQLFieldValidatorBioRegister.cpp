//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorBioRegister.h"
//**********************************************************************

void CSQLFieldValidatorBioRegister::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, BioRegister::UserID);
}

//**********************************************************************

void CSQLFieldValidatorBioRegister::SetUsername(CString str, CString& strTarget)
{
	SetString(str, strTarget, BioRegister::Username);
}

//**********************************************************************

void CSQLFieldValidatorBioRegister::SetCounter(int n, int& nTarget)
{
	SetInt(n, nTarget, BioRegister::Counter);
}

//**********************************************************************

void CSQLFieldValidatorBioRegister::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, BioRegister::FirstDate);
}

//**********************************************************************

void CSQLFieldValidatorBioRegister::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, BioRegister::FirstTime);
}

//**********************************************************************

