//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorBromComTransaction.h"
//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, BromComTransaction::LineID);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetPersonID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComTransaction::PersonID);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetDetails(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComTransaction::Details);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetAmount(double d, double& dTarget)
{
	SetDouble(d, dTarget, BromComTransaction::Amount);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetBalance(double d, double& dTarget)
{
	SetDouble(d, dTarget, BromComTransaction::Balance);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetDateTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComTransaction::DateTime);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetType(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComTransaction::Type);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetTransactionID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComTransaction::TransactionID);
}

//**********************************************************************

void CSQLFieldValidatorBromComTransaction::SetWebComment(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComTransaction::WebComment);
}

//**********************************************************************

