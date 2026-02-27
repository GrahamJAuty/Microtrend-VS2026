#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebbleExportTransaction : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(__int64 n, __int64& nTarget);
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetXFerUserID(__int64 n, __int64& nTarget);
	static void SetMemberID(CString str, CString& strTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	static void SetCCNo(int n, int& nTarget);
	static void SetSourceType(int n, int& nTarget);
	static void SetAppNo(int n, int& nTarget);
	static void SetAmount(double d, double& dTarget);
	static void SetPurse1(double d, double& dTarget);
	static void SetPurse2(double d, double& dTarget);
	static void SetCash(double d, double& dTarget);
	static void SetWebComment(CString str, CString& strTarget);
};

//**********************************************************************
