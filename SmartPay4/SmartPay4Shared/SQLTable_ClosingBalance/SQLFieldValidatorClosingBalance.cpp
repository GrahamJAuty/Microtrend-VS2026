//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorClosingBalance.h"
//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetKeyDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, ClosingBalance::KeyDate);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, ClosingBalance::Date);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, ClosingBalance::Time);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetArchiveDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, ClosingBalance::ArchiveDate);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetArchiveTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, ClosingBalance::ArchiveTime);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse1CreditAudit(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse1CreditAudit);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse1LiabilityAudit(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse1LiabilityAudit);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse2BalanceAudit(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse2BalanceAudit);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse3CreditAudit(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse3CreditAudit);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse3LiabilityAudit(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse3LiabilityAudit);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse1CreditAdjust(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse1CreditAdjust);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse1LiabilityAdjust(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse1LiabilityAdjust);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse3CreditAdjust(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse3CreditAdjust);
}

//**********************************************************************

void CSQLFieldValidatorClosingBalance::SetPurse3LiabilityAdjust(double d, double& dTarget)
{
	SetDouble(d, dTarget, ClosingBalance::Purse3LiabilityAdjust);
}

//**********************************************************************
