//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorAuditPeriod.h"
//**********************************************************************

void CSQLFieldValidatorAuditPeriod::SetPeriodID(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPeriod::PeriodID);
}

//**********************************************************************

void CSQLFieldValidatorAuditPeriod::SetPeriodName(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPeriod::PeriodName);
}

//**********************************************************************

void CSQLFieldValidatorAuditPeriod::SetStartTime(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPeriod::StartTime);
}

//**********************************************************************

