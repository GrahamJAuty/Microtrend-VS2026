#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorBromComBalanceChanges : public CSQLFieldValidatorBase 
{
public:
	static void SetPersonID(int n, int& nTarget);
	static void SetAccountID(__int64 n, __int64& nTarget);
};

//**********************************************************************

