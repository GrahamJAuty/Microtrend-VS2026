//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebbleRecentTransaction.h"
//**********************************************************************

void CSQLFieldValidatorPebbleRecentTransaction::SetTransactionID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleRecentTransaction::TransactionID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleRecentTransaction::SetDateTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleRecentTransaction::DateTime);
}

//**********************************************************************
