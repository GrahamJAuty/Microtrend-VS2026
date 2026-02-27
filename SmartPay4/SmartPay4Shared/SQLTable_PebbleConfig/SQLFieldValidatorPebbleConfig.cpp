//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPebbleConfig.h"
//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetLineID(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::LineID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetInstallationID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleConfig::InstallationID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetOrganisationID(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleConfig::OrganisationID);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetMISInfoIndex(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::MISInfoIndex);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetUPNInfoIndex(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::UPNInfoIndex);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetLastImportDateTopup(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleConfig::LastImportDateTopup);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetLastImportTimeTopup(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleConfig::LastImportTimeTopup);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetLastImportDateSale(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleConfig::LastImportDateSale);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetLastImportTimeSale(CString str, CString& strTarget)
{
	SetString(str, strTarget, PebbleConfig::LastImportTimeSale);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetRecentCacheDays(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::RecentCacheDays);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetPaymentCacheDays(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::PaymentCacheDays);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetVATRate(double d, double& dTarget)
{
	SetDouble(d, dTarget, PebbleConfig::VAT_A);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetVATCode(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::VATCODE_A);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetOfficeMealPluNo(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PebbleConfig::OfficeMealPluNo);
}

//**********************************************************************

void CSQLFieldValidatorPebbleConfig::SetOfficeMealVATBand(int n, int& nTarget)
{
	SetInt(n, nTarget, PebbleConfig::OfficeMealVATBand);
}

//**********************************************************************
