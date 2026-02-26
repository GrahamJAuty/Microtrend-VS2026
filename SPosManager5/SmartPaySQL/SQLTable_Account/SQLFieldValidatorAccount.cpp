//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

void CSQLFieldValidatorAccount::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, Account::UserID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo3(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info3);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo4(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info4);
}

//**********************************************************************
