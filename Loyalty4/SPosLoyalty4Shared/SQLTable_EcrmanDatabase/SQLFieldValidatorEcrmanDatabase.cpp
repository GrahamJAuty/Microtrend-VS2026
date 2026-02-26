//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorEcrmanDatabase.h"
//**********************************************************************

void CSQLFieldValidatorEcrmanDatabase::SetDbNo(int n, int& nTarget)
{
	SetInt(n, nTarget, EcrmanDatabase::DbNo);
}

//**********************************************************************

void CSQLFieldValidatorEcrmanDatabase::SetDbName(CString str, CString& strTarget)
{
	SetString(str, strTarget, EcrmanDatabase::DbName);
}

//**********************************************************************


