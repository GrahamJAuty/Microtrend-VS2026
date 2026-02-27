#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorExternal : public CSQLFieldValidatorBase 
{
public:
	static void SetAESUsername(CString str, CString& strTarget);
	static void SetPasswordHash(CString str, CString& strTarget);
	static void SetAESUserID(CString str, CString& strTarget);
};

//**********************************************************************

