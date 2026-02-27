#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPINNumber : public CSQLFieldValidatorBase
{
public:
	static void SetPINNumber(CString str, CString& strTarget);
	static void SetUserID(CString str, CString& strTarget);
	static void SetUsername(CString str, CString& strTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	static void SetUseCounter(int n, int& nTarget);
};

//**********************************************************************

