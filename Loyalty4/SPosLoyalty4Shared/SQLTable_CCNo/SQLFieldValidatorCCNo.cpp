//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorCCNo.h"
//**********************************************************************

void CSQLFieldValidatorCCNo::SetType(CString str, CString& strTarget)
{
	SetString(str, strTarget, CCNo::Type);
}

//**********************************************************************

void CSQLFieldValidatorCCNo::SetTypeInt(int n, int& nTarget)
{
	SetInt(n, nTarget, CCNo::TypeInt);
}

//**********************************************************************

void CSQLFieldValidatorCCNo::SetNumber(int n, int& nTarget)
{
	SetInt(n, nTarget, CCNo::Number);
}

//**********************************************************************