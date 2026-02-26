#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorChartwellExportPayment : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetTransactionID(__int64 n, __int64& nTarget);
	static void SetTerminalNo(int n, int& nTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetCreateDateTime(CString str, CString& strTarget);
	static void SetGroupNo(int n, int& nTarget);
	static void SetPaymentPurse(double d, double& dTarget);
};

//**********************************************************************

