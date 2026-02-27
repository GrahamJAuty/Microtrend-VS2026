//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPeriodRefresh.h"
//**********************************************************************

void CSQLFieldValidatorPeriodRefresh::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PeriodRefresh::UserID);
}

//**********************************************************************

void CSQLFieldValidatorPeriodRefresh::SetPeriodDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PeriodRefresh::Period1Date);
}

//**********************************************************************

void CSQLFieldValidatorPeriodRefresh::SetPeriodBalance(double d, double& dTarget)
{
	SetDouble(d, dTarget, PeriodRefresh::Period1Balance);
}

//**********************************************************************

