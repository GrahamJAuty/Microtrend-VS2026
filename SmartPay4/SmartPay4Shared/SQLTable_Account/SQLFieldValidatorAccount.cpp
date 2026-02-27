//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

void CSQLFieldValidatorAccount::SetLeaverYear(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::LeaverYear);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, Account::UserID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetUsername(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Username);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetForename(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Forename);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetSurname(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Surname);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetDOB(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::DOB);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetExpiryDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::ExpiryDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGender(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Gender);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetDayGroups(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::DayGroups);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGroupShiftStatus(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::GroupShiftStatus);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGroupShiftStartDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::GroupShiftStartDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGroupShiftEndDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::GroupShiftEndDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetGroupShiftFutureGroup(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::GroupShiftFutureGroup);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetEmail(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Email);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo1(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info1);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo2(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info2);
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

void CSQLFieldValidatorAccount::SetInfo5(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info5);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo6(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info6);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo7(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info7);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo8(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info8);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetAllergyCodes(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::AllergyCodes);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetDietary(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Dietary);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetMemberID(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::MemberID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetLegacyPebblePurseID(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::PebblePurseID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetExternalRef1(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::ExternalRef1);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetExternalRef2(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::ExternalRef2);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetNextUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, Account::NextUserID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPreviousUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, Account::PreviousUserID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1Allow(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::Purse1Allow);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1Credit(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse1Credit);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1Liability(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse1Liability);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1LastSpendDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse1LastSpendDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1LastSpendTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse1LastSpendTime);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1LastSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse1LastSpend);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse1SpendToDate(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse1SpendToDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2Allow(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::Purse2Allow);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2Balance(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse2Balance);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2LastSpendDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse2LastSpendDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2LastSpendTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse2LastSpendTime);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2LastSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse2LastSpend);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2SpendToDate(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse2SpendToDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2RefreshFlags(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::Purse2RefreshFlags);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2RefreshedDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse2RefreshedDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse2RefreshedTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse2RefreshedTime);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3Allow(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::Purse3Allow);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3Credit(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse3Credit);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3Liability(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse3Liability);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3LastSpendDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse3LastSpendDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3LastSpendTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Purse3LastSpendTime);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3LastSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse3LastSpend);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPurse3SpendToDate(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse3SpendToDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetCashSpendToDate(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::CashSpendToDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPoints(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::Points);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPointsToDate(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::PointsToDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetMaxSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::MaxSpendPurse1Period1);
}

//**********************************************************************//**********************************************************************

void CSQLFieldValidatorAccount::SetMaxOverdraft(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::MaxOverdraftPurse1Period1);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetAlertCode(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::AlertCode);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetAlertDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::AlertDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetAlertTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::AlertTime);
}

//**********************************************************************