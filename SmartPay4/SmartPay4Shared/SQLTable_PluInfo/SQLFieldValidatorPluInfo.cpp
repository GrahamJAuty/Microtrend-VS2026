//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPluInfo.h"
//**********************************************************************

void CSQLFieldValidatorPluInfo::SetPluNo(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PluInfo::PluNo);
}

//**********************************************************************

void CSQLFieldValidatorPluInfo::SetDescription(CString str, CString& strTarget)
{
	SetString(str, strTarget, PluInfo::Description);
}

//**********************************************************************

void CSQLFieldValidatorPluInfo::SetDeptNo(int n, int& nTarget)
{
	SetInt(n, nTarget, PluInfo::DeptNo);
}

//**********************************************************************

void CSQLFieldValidatorPluInfo::SetTaxCode(CString str, CString& strTarget)
{
	SetString(str, strTarget, PluInfo::TaxCode);
}

//**********************************************************************

void CSQLFieldValidatorPluInfo::SetModType(int n, int& nTarget)
{
	SetInt(n, nTarget, PluInfo::ModType);
}

//**********************************************************************
void CSQLFieldValidatorPluInfo::SetPoints(int n, int& nTarget)
{
	SetInt(n, nTarget, PluInfo::Points);
}

//**********************************************************************

void CSQLFieldValidatorPluInfo::SetPCtrlRuleID(int n, int& nTarget)
{
	SetInt(n, nTarget, PluInfo::PCtrlRuleID);
}

//**********************************************************************

void CSQLFieldValidatorPluInfo::SetStampWeight(int n, int& nTarget)
{
	SetInt(n, nTarget, PluInfo::StampWeight);
}

//**********************************************************************