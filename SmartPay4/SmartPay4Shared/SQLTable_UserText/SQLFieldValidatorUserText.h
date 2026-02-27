#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorUserText : public CSQLFieldValidatorBase 
{
public:
	static void SetFieldLabel(CString str, CString& strTarget);
	static void SetUserText(CString str, CString& strTarget);
};

//**********************************************************************

