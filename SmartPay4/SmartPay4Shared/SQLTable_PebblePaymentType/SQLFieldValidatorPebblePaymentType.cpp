//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebblePaymentType.h"
//**********************************************************************

void CSQLFieldValidatorPebblePaymentType::SetSPOSPaymentType(int n, int& nTarget)
{
	SetInt(n, nTarget, PebblePaymentType::SPOSPaymentType);
}

//**********************************************************************

void CSQLFieldValidatorPebblePaymentType::SetPebblePaymentName(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebblePaymentType::PebblePaymentName);
}

//**********************************************************************

void CSQLFieldValidatorPebblePaymentType::SetPebbleTranType(int n, int& nTarget)
{
	SetInt(n, nTarget, PebblePaymentType::PebbleTranType);
}

//**********************************************************************