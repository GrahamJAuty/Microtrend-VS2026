#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorGroup : public CSQLFieldValidatorBase 
{
public:
	static void SetGroupNo(int n, int& nTarget);
	static void SetGroupName(CString str, CString& strTarget);
	static void SetSchemeNo(int n, int& nTarget);
	/*****/
	static void SetRefreshType(int n, int& nTarget);
	static void SetRefreshFlags(int n, int& nTarget);
	static void SetRefreshValue(double d, double& dTarget);
	static void SetRefreshAppend(int n, int& nTarget);
	/*****/
	static void SetPurseAllowed(int n, int& nTarget);
	/*****/
	static void SetMaxSpendType(int n, int& nTarget);
	/*****/
	static void SetMaxSpend(double d, double& dTarget);
	/*****/
	static void SetMaxOverdraft(double d, double& dTarget);
	/*****/
	static void SetRevalueType(int n, int& nTarget);
	static void SetRevalueTrip(double d, double& dTarget);
	static void SetRevaluePoints(int n, int& nTarget);
	static void SetRevalueBonus(double d, double& dTarget);
	static void SetRevaluePercent(int n, int& nTarget);
	/*****/
	static void SetRewardType(int n, int& nTarget);
	static void SetRewardTrip(double d, double& dTarget);
	static void SetRewardPoints(int n, int& nTarget);
	static void SetRewardBonus(double d, double& dTarget);
	static void SetRewardLimit(double d, double& dTarget);
	static void SetRewardTrigger(double d, double& dTarget);
	/*****/
	static void SetRedeemType(int n, int& nTarget);
	static void SetRedeemTrip(int n, int& nTarget);
	static void SetRedeemValue(double d, double& dTarget);
};

//**********************************************************************

