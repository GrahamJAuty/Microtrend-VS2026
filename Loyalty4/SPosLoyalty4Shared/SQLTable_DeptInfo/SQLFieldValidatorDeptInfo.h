#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorDeptInfo : public CSQLFieldValidatorBase 
{
public:
	static void SetDbNo(int n, int& nTarget);
	static void SetDeptNo(int n, int& nTarget);
	static void SetName(CString str, CString& strTarget);
	static void SetLoyaltyBonusTrip(double d, double& dTarget);
	static void SetLoyaltyBonusType(int n, int& nTarget);
	static void SetLoyaltyBonusPoints(int n, int& nTarget);
	static void SetLoyaltyBonusValue(double d, double& dTarget);
	static void SetLoyaltyBonusPercent(int n, int& nTarget);
};

//**********************************************************************

