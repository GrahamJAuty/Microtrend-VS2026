#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorStaffGiftException : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(__int64 n, __int64& nTarget);
	static void SetCustomerID(__int64 n, __int64& nTarget);
	static void SetFolderIndexDbNo(int n, int& nTarget);
	static void SetFolderIndexSetNo(int n, int& nTarget);
	static void SetServerNo(int n, int& nTarget);
	static void SetServerID(__int64 n, __int64& nTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	static void SetValue(double d, double& dTarget);
	static void SetErrorCode(int n, int& nTarget);
};

//**********************************************************************

