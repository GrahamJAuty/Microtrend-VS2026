//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorChartwellExportPayment.h"
//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, ChartwellExportPayment::UserID);
}

//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetTransactionID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, ChartwellExportPayment::TransactionID);
}

//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetTerminalNo(int n, int& nTarget)
{
	SetInt(n, nTarget, ChartwellExportPayment::TerminalNo);
}

//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, ChartwellExportPayment::Date);
}

//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetCreateDateTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, ChartwellExportPayment::CreateDateTime);
}

//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, ChartwellExportPayment::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorChartwellExportPayment::SetPaymentPurse(double d, double& dTarget)
{
	SetDouble(d, dTarget, ChartwellExportPayment::PaymentPurse1);
}

//**********************************************************************

