#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorBromComPendingItem : public CSQLFieldValidatorBase 
{
public:
	static void SetOrderID(int n, int& nTarget);
	static void SetPersonID(int n, int& nTarget);
	static void SetAmount(double d, double& dTarget);
};

//**********************************************************************

