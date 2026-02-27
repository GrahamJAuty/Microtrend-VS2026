#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebbleExportPayment : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	static void SetCreateDateTime(CString str, CString& strTarget);
	static void SetCCNo(int n, int& nTarget);
	static void SetPaymentType(int n, int& nTarget);
	static void SetPaymentAmount(double d, double& dTarget);
};

//**********************************************************************

