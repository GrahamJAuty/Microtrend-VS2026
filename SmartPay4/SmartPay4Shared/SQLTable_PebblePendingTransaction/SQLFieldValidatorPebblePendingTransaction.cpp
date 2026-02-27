//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebblePendingTransaction.h"
//**********************************************************************

void CSQLFieldValidatorPebblePendingTransaction::SetTransactionID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebblePendingTransaction::TransactionID);
}

//**********************************************************************

void CSQLFieldValidatorPebblePendingTransaction::SetMemberID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebblePendingTransaction::MemberID);
}

//**********************************************************************

void CSQLFieldValidatorPebblePendingTransaction::SetDateTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebblePendingTransaction::DateTime);
}

//**********************************************************************

void CSQLFieldValidatorPebblePendingTransaction::SetName(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebblePendingTransaction::Name);
}

//**********************************************************************

void CSQLFieldValidatorPebblePendingTransaction::SetAmount(int n, int& nTarget)
{
	SetInt(n, nTarget, PebblePendingTransaction::Amount);
}

//**********************************************************************
