//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorOfferStamp.h"
//**********************************************************************

void CSQLFieldValidatorOfferStamp::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, OfferStamp::UserID);
}

//**********************************************************************

void CSQLFieldValidatorOfferStamp::SetStampOfferID(int n, int& nTarget)
{
	SetInt(n, nTarget, OfferStamp::StampOfferID);
}

//**********************************************************************

void CSQLFieldValidatorOfferStamp::SetEdition(int n, int& nTarget)
{
	SetInt(n, nTarget, OfferStamp::Edition);
}

//**********************************************************************

void CSQLFieldValidatorOfferStamp::SetAwardDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, OfferStamp::AwardDate);
}

//**********************************************************************

void CSQLFieldValidatorOfferStamp::SetExpireDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, OfferStamp::ExpireDate);
}

//**********************************************************************

void CSQLFieldValidatorOfferStamp::SetStampCount(int n, int& nTarget)
{
	SetInt(n, nTarget, OfferStamp::StampCount);
}

//**********************************************************************

