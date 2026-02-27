#pragma once
//**********************************************************************
#include "..\\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorCCNo : public CSQLFieldValidatorBase 
{
public:
	static void SetType(CString str, CString& strTarget);
	static void SetTypeInt(int n, int& nTarget);
	static void SetNumber(int n, int& nTarget);
};

//**********************************************************************

