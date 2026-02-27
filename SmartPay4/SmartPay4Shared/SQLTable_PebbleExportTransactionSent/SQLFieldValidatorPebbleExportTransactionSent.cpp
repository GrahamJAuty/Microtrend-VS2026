//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebbleExportTransactionSent.h"
//**********************************************************************

void CSQLFieldValidatorPebbleExportTransactionSent::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleExportTransactionSent::LineID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransactionSent::SetTranLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleExportTransactionSent::TranLineID);
}

//**********************************************************************

