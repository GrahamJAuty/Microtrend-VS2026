#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorGroup : public CSQLFieldValidatorBase 
{
public:
	static void SetGroupNo(int n, int& nTarget);
	static void SetGroupName(CString str, CString& strTarget);
	static void SetPriceLevel(int n, int& nTarget);
};

//**********************************************************************

