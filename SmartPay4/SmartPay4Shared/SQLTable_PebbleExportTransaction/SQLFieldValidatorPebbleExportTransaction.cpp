//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebbleExportTransaction.h"
//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleExportTransaction::LineID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleExportTransaction::UserID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetXFerUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleExportTransaction::XFerUserID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetMemberID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportTransaction::MemberID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportTransaction::Date);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportTransaction::Time);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetCCNo(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleExportTransaction::CCNo);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetSourceType(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleExportTransaction::SourceType);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetAppNo(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleExportTransaction::AppNo);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetAmount(double d, double& dTarget)
{
	SetDouble(d, dTarget, PebbleExportTransaction::Amount);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetPurse1(double d, double& dTarget)
{
	SetDouble(d, dTarget, PebbleExportTransaction::Purse1);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetPurse2(double d, double& dTarget)
{
	SetDouble(d, dTarget, PebbleExportTransaction::Purse2);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetCash(double d, double& dTarget)
{
	SetDouble(d, dTarget, PebbleExportTransaction::Cash);
}

//**********************************************************************

void CSQLFieldValidatorPebbleExportTransaction::SetWebComment(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleExportTransaction::WebComment);
}

//**********************************************************************

