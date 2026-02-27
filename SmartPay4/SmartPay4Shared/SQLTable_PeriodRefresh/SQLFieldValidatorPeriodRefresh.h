#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPeriodRefresh : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetPeriodDate(CString str, CString& strTarget);
	static void SetPeriodBalance(double d, double& dTarget);
};

//**********************************************************************

