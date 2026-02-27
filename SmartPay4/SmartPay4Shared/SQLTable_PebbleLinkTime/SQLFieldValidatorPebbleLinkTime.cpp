//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebbleLinkTime.h"
//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleLinkTime::LineID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::Date);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::Time);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleLinkTime::UserID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetUsername(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::Username);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetForename(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::Forename);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetSurname(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::Surname);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleLinkTime::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetPebbleFirstName(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::PebbleFirstName);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetPebbleLastName(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::PebbleLastName);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetProcessType(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleLinkTime::ProcessType);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetMatchType(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleLinkTime::MatchType);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetMIS(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::MIS);
}

//**********************************************************************

void CSQLFieldValidatorPebbleLinkTime::SetUPN(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleLinkTime::UPN);
}

//**********************************************************************
