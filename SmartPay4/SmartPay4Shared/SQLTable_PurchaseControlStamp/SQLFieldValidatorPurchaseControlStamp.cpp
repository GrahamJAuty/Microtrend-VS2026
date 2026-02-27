//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPurchaseControlStamp.h"
//**********************************************************************

void CSQLFieldValidatorPurchaseControlStamp::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PurchaseControlStamp::UserID);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlStamp::SetRuleID(int n, int& nTarget)
{
	SetInt(n, nTarget, PurchaseControlStamp::RuleID);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlStamp::SetAwardDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PurchaseControlStamp::AwardDate);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlStamp::SetExpireDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PurchaseControlStamp::ExpireDate);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseControlStamp::SetStampCount(int n, int& nTarget)
{
	SetInt(n, nTarget, PurchaseControlStamp::StampCount);
}

//**********************************************************************

