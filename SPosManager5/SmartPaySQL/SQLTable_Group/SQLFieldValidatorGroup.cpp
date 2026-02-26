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

void CSQLFieldValidatorGroup::SetPriceLevel(int n, int& nTarget)
{
	SetInt(n, nTarget, Group::PriceLevel);
}

//**********************************************************************
