//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorStampOfferInfo.h"
//**********************************************************************

void CSQLFieldValidatorStampOfferInfo::SetStampOfferID(int n, int& nTarget)
{
	SetInt(n, nTarget, StampOfferInfo::StampOfferID);
}

//**********************************************************************

void CSQLFieldValidatorStampOfferInfo::SetEdition(int n, int& nTarget)
{
	SetInt(n, nTarget, StampOfferInfo::Edition);
}

//**********************************************************************

void CSQLFieldValidatorStampOfferInfo::SetThreshold(int n, int& nTarget)
{
	SetInt(n, nTarget, StampOfferInfo::Threshold);
}

//**********************************************************************

void CSQLFieldValidatorStampOfferInfo::SetExpiry(int n, int& nTarget)
{
	SetInt(n, nTarget, StampOfferInfo::Expiry);
}

//**********************************************************************

void CSQLFieldValidatorStampOfferInfo::SetDescription(CString str, CString& strTarget)
{
	SetString(str, strTarget, StampOfferInfo::Description);
}

//**********************************************************************


