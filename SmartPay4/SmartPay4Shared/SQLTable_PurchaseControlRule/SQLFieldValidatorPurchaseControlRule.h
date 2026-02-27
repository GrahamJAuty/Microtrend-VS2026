#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPurchaseControlRule : public CSQLFieldValidatorBase
{
public:
	static void SetRuleID(int n, int& nTarget);
	static void SetThreshold(int n, int& nTarget);
	static void SetExpiryDays(int n, int& nTarget);
	static void SetDescription(CString str, CString& strTarget);
};

//**********************************************************************

