#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorScheme : public CSQLFieldValidatorBase
{
public:
	static void SetSchemeNo(int n, int& nTarget);
	static void SetSchemeName(CString str, CString& strTarget);
};

//**********************************************************************

