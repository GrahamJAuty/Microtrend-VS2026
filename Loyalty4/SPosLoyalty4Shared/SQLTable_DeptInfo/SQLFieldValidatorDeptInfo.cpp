//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorDeptInfo.h"
//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetDbNo(int n, int& nTarget)
{
	SetInt(n, nTarget, DeptInfo::DbNo);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetDeptNo(int n, int& nTarget)
{
	SetInt(n, nTarget, DeptInfo::DeptNo);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetName(CString str, CString& strTarget)
{
	SetString(str, strTarget, DeptInfo::Name);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetLoyaltyBonusTrip(double d, double& dTarget)
{
	SetDouble(d, dTarget, DeptInfo::LoyaltyBonusTrip);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetLoyaltyBonusType(int n, int& nTarget)
{
	SetInt(n, nTarget, DeptInfo::LoyaltyBonusType);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetLoyaltyBonusPoints(int n, int& nTarget)
{
	SetInt(n, nTarget, DeptInfo::LoyaltyBonusPoints);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetLoyaltyBonusValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, DeptInfo::LoyaltyBonusValue);
}

//**********************************************************************

void CSQLFieldValidatorDeptInfo::SetLoyaltyBonusPercent(int n, int& nTarget)
{
	SetInt(n, nTarget, DeptInfo::LoyaltyBonusPercent);
}

//**********************************************************************




