#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorBromComItem : public CSQLFieldValidatorBase 
{
public:
	static void SetOrderID(int n, int& nTarget);
	static void SetPersonID(int n, int& nTarget);
	static void SetAmount(double d, double& dTarget);
	static void SetStatus(CString str, CString& strTarget);
};

//**********************************************************************

