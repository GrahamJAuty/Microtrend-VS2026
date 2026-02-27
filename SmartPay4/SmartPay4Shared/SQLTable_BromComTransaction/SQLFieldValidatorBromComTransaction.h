#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorBromComTransaction : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(__int64 n, __int64& nTarget);
	static void SetPersonID(int n, int& nTarget);
	static void SetDetails(CString str, CString& strTarget);
	static void SetAmount(double d, double& dTarget);
	static void SetBalance(double d, double& dTarget);
	static void SetDateTime(CString str, CString& strTarget);
	static void SetType(CString str, CString& strTarget);
	static void SetTransactionID(int n, int& nTarget);
	static void SetWebComment(CString str, CString& strTarget);
};

//**********************************************************************
