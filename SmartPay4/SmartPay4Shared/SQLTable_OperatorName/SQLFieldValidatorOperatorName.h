#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorOperatorName : public CSQLFieldValidatorBase 
{
public:
	static void SetOperatorNo(int n, int& nTarget);
	static void SetOperatorName(CString str, CString& strTarget);
};

//**********************************************************************

