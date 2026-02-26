#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorGroupTopupBonus : public CSQLFieldValidatorBase 
{
public:
	static void SetGroupNo(int n, int& nTarget);
	static void SetPurseNo(int n, int& nTarget);
	static void SetTopupNo(int n, int& nTarget);
	/*****/
	static void SetBonusType(int n, int& nTarget);
	static void SetBonusTrip(double d, double& dTarget);
	static void SetBonusPoints(int n, int& nTarget);
	static void SetBonusValue(double d, double& dTarget);
	static void SetBonusPercent(int n, int& nTarget);
};

//**********************************************************************

