#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorAccount : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetGroupNo(int n, int& nTarget);
	static void SetInfo3(CString str, CString& strTarget);
	static void SetInfo4(CString str, CString& strTarget);
};

//**********************************************************************

