//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPurchaseControlRule.h"
//**********************************************************************

void CSQLFieldValidatorPurchaseControlRule::SetRuleID(int n, int& nTarget)
{
	SetInt(n, nTarget, PurchaseControlRule::RuleID);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlRule::SetThreshold(int n, int& nTarget)
{
	SetInt(n, nTarget, PurchaseControlRule::Threshold);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlRule::SetExpiryDays(int n, int& nTarget)
{
	SetInt(n, nTarget, PurchaseControlRule::Expiry);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlRule::SetDescription(CString str, CString& strTarget)
{
	SetString(str, strTarget, PurchaseControlRule::Description);
}

//**********************************************************************


