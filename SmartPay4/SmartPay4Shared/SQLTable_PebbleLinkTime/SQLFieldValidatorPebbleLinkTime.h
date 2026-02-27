#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebbleLinkTime : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(__int64 n, __int64& nTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetUsername(CString str, CString& strTarget);
	static void SetForename(CString str, CString& strTarget);
	static void SetSurname(CString str, CString& strTarget);
	static void SetGroupNo(int n, int& nTarget);
	static void SetPebbleFirstName(CString str, CString& strTarget);
	static void SetPebbleLastName(CString str, CString& strTarget);
	static void SetProcessType(int n, int& nTarget);
	static void SetMatchType(int n, int& nTarget);
	static void SetMIS(CString str, CString& strTarget);
	static void SetUPN(CString str, CString& strTarget);
};

//**********************************************************************
