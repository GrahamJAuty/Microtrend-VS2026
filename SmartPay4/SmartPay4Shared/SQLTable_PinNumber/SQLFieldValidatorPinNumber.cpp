//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPINNumber.h"
//**********************************************************************

void CSQLFieldValidatorPINNumber::SetPINNumber(CString str, CString& strTarget)
{
	SetString(str, strTarget, PINNumber::PINNumber);
}

//**********************************************************************

void CSQLFieldValidatorPINNumber::SetUserID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PINNumber::UserID);
}

//**********************************************************************

void CSQLFieldValidatorPINNumber::SetUsername(CString str, CString& strTarget)
{
	SetString(str, strTarget, PINNumber::Username);
}

//**********************************************************************

void CSQLFieldValidatorPINNumber::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PINNumber::FirstDate);
}

//**********************************************************************

void CSQLFieldValidatorPINNumber::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PINNumber::FirstTime);
}

//**********************************************************************

void CSQLFieldValidatorPINNumber::SetUseCounter(int n, int& nTarget)
{
	SetInt(n, nTarget, PINNumber::UseCounter);
}

//**********************************************************************
