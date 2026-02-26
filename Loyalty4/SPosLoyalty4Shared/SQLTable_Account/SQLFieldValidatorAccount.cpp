//**********************************************************************
#include "..\AccountFieldValidator.h"
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************

void CSQLFieldValidatorAccount::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, Account::UserID);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetFullname(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Fullname);
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
	bool bCanSet = TRUE;

	if (str != "")
	{
		bCanSet = CAccountFieldValidator::ValidateStringDate(str);
	}

	if (TRUE == bCanSet)
	{
		SetString(str, strTarget, Account::DOB);
	}
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetExpiryDate(CString str, CString& strTarget)
{
	bool bCanSet = TRUE;

	if (str != "")
	{
		bCanSet = CAccountFieldValidator::ValidateStringDate(str);
	}

	if (TRUE == bCanSet)
	{
		SetString(str, strTarget, Account::ExpiryDate);
	}
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

void CSQLFieldValidatorAccount::SetSchemeNo(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::SchemeNo);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetInfo(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Info1);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetQRInfo(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::QRInfo);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetAddress(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Address1);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetPhone(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::Phone1);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetExternalRef(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::ExternalRef);
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

void CSQLFieldValidatorAccount::SetPurse1Balance(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::Purse1Balance);
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

void CSQLFieldValidatorAccount::SetCashLastSpendDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::CashLastSpendDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetCashLastSpendTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::CashLastSpendTime);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetCashLastSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::CashLastSpend);
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

void CSQLFieldValidatorAccount::SetHotlistCode(int n, int& nTarget)
{
	SetInt(n, nTarget, Account::HotlistCode);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetHotlistDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::HotlistDate);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetHotlistTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::HotlistTime);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetMaxSpend(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::MaxSpend);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetMaxOverdraft(double d, double& dTarget)
{
	SetDouble(d, dTarget, Account::MaxOverdraft);
}

//**********************************************************************

void CSQLFieldValidatorAccount::SetAlertCodes(CString str, CString& strTarget)
{
	SetString(str, strTarget, Account::AlertCodes);
}

//**********************************************************************
