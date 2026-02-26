//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorScheme.h"
//**********************************************************************

void CSQLFieldValidatorScheme::SetSchemeNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Scheme::SchemeNo);
}

//**********************************************************************

void CSQLFieldValidatorScheme::SetSchemeName(CString str, CString& strTarget)
{
	SetString(str, strTarget, Scheme::SchemeName);
}

//**********************************************************************


