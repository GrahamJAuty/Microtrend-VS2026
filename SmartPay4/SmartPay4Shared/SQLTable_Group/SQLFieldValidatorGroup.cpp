//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
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

void CSQLFieldValidatorGroup::SetUserType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::UserType);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetSetNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::SetNo);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetShiftGroups(CString str, CString& strTarget)
{
	SetString(str, strTarget, Group::ShiftGroups);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetPriceLevel(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::PriceLevel);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetVATShift(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::VATShift);
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

void CSQLFieldValidatorGroup::SetRefreshTypePeriod(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RefreshTypePeriod1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRefreshValuePeriod(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RefreshValuePeriod1);
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
	SetDouble(d, dTarget, Group::MaxSpendPurse1Period1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetMaxOverdraft(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::MaxOverdraftPurse1Period1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetDeptInhibit(CString str, CString& strTarget)
{
	SetString(str, strTarget, Group::DeptInhibitPurse1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetLowBalance(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::LowBalancePurse1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetPointsAchievement(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::PointsAchievement);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevalueType(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RevalueType);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevalueTrip(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RevalueTrip1);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevaluePoints(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RevaluePoints);
}

//**********************************************************************

void CSQLFieldValidatorGroup::SetRevalueBonus(double d, double& dTarget)
{
	SetDouble(d, dTarget, Group::RevalueBonus);
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

void CSQLFieldValidatorGroup::SetRedeemPurse(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::RedeemPurse);
}

//**********************************************************************
