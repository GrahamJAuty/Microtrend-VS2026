#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorWebPaymentOptions : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(int n, int& nTarget);
	static void SetDayFlags(int n, int& nTarget);
	static void SetDateString(CString str, CString& nTarget);
	static void SetTimeString(CString str, CString& nTarget);
};

//**********************************************************************

