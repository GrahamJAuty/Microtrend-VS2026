//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorBromComBalanceChanges.h"
//**********************************************************************

void CSQLFieldValidatorBromComBalanceChanges::SetPersonID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComBalanceChanges::PersonID);
}

//**********************************************************************

void CSQLFieldValidatorBromComBalanceChanges::SetAccountID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, BromComBalanceChanges::AccountID);
}

//**********************************************************************

