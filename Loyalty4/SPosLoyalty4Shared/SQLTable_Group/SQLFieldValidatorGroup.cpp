//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************

void CSQLFieldValidatorGroup::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetGroupName(CString str, CString& strTarget)
{
	SetString(str, strTarget, Group::GroupName);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetSchemeNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::SchemeNo);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRefreshType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RefreshType);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRefreshFlags(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RefreshFlags);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRefreshValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RefreshValue);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRefreshAppend(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RefreshAppend);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetPurseAllowed(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::Purse1Allowed);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetMaxSpendType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::MaxSpendType);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetMaxSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::MaxSpendPeriod1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetMaxOverdraft(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::MaxOverdraftPeriod1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevalueType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::LegacyRevalueType1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevalueTrip(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::LegacyRevalueTrip1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevaluePoints(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::LegacyRevaluePoints1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevalueBonus(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::LegacyRevalueBonus1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevaluePercent(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::LegacyRevaluePercent1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRewardType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RewardType);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRewardTrip(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RewardTrip);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRewardPoints(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RewardPoints);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRewardBonus(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RewardBonus);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRewardLimit(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RewardLimit);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRewardTrigger(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RewardTrigger);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRedeemType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RedeemType);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRedeemTrip(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RedeemTripAsInt);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRedeemValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RedeemValue);
}

//**********************************************************************

