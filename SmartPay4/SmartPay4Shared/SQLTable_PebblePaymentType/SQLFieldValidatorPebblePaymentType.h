#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebblePaymentType : public CSQLFieldValidatorBase 
{
public:
	static void SetSPOSPaymentType(int n, int& nTarget);
	static void SetPebblePaymentName(CString str, CString& strTarget);
	static void SetPebbleTranType(int n, int& nTarget);
};

//**********************************************************************

