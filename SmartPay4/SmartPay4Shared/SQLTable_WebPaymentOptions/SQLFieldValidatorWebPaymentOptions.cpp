//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorWebPaymentOptions.h"
//**********************************************************************

void CSQLFieldValidatorWebPaymentOptions::SetLineID(int n, int& nTarget)
{
	SetInt(n, nTarget, WebPaymentOption::LineID);
}

//**********************************************************************

void CSQLFieldValidatorWebPaymentOptions::SetDayFlags(int n, int& nTarget)
{
	SetInt(n, nTarget, WebPaymentOption::AccountLinkDays);
}

//**********************************************************************

void CSQLFieldValidatorWebPaymentOptions::SetDateString(CString str, CString& strTarget)
{
	SetString(str, strTarget, WebPaymentOption::AccountLinkLastDate);
}

//**********************************************************************

void CSQLFieldValidatorWebPaymentOptions::SetTimeString(CString str, CString& strTarget)
{
	SetString(str, strTarget, WebPaymentOption::AccountLinkLastTime);
}

//**********************************************************************
