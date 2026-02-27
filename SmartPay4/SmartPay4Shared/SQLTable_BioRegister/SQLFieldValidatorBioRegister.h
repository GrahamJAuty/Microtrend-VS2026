#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorBioRegister : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetUsername(CString str, CString& strTarget);
	static void SetCounter(int n, int& nTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
};

//**********************************************************************

