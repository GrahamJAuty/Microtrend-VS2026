//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorGroupTopupBonus.h"
//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, GroupTopupBonus::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetPurseNo(int n, int& nTarget)
{
	SetInt(n, nTarget, GroupTopupBonus::PurseNo);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetTopupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, GroupTopupBonus::TopupNo);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetBonusType(int n, int& nTarget)
{
	SetInt(n, nTarget, GroupTopupBonus::BonusType);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetBonusTrip(double d, double &dTarget)
{
	SetDouble(d, dTarget, GroupTopupBonus::BonusTrip);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetBonusPoints(int n, int& nTarget)
{
	SetInt(n, nTarget, GroupTopupBonus::BonusPoints);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetBonusValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, GroupTopupBonus::BonusValue);
}

//**********************************************************************

void CSQLFieldValidatorGroupTopupBonus::SetBonusPercent(int n, int& nTarget)
{
	SetInt(n, nTarget, GroupTopupBonus::BonusPercent);
}

//**********************************************************************
