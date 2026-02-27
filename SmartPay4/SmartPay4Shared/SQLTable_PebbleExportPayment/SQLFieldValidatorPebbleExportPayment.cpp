//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebbleExportPayment.h"
//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleExportPayment::UserID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportPayment::Date);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportPayment::Time);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetCreateDateTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportPayment::CreateDateTime);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetCCNo(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleExportPayment::CCNo);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetPaymentType(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleExportPayment::PaymentType1);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportPayment::SetPaymentAmount(double d, double& dTarget)
{
	SetDouble(d, dTarget, PebbleExportPayment::PaymentAmount1);
}

//**********************************************************************

