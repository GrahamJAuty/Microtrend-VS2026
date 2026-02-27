#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorDbVersion : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(int n, int& nTarget);
	static void SetVersion(int n, int& nTarget);
	static void SetExternalVersion1(int n, int& nTarget);
};

//**********************************************************************

