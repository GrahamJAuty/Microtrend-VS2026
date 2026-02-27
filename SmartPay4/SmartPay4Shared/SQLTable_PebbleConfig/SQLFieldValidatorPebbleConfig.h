#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebbleConfig : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(int n, int& nTarget);
	static void SetInstallationID(CString str, CString& strTarget);
	static void SetOrganisationID(CString str, CString& strTarget);
	static void SetUPNInfoIndex(int n, int& nTarget);
	static void SetMISInfoIndex(int n, int& nTarget);
	static void SetLastImportDateTopup(CString str, CString& strTarget);
	static void SetLastImportTimeTopup(CString str, CString& strTarget);
	static void SetLastImportDateSale(CString str, CString& strTarget);
	static void SetLastImportTimeSale(CString str, CString& strTarget);
	static void SetRecentCacheDays(int n, int& nTarget);
	static void SetPaymentCacheDays(int n, int& nTarget);
	static void SetVATRate(double d, double& dTarget);
	static void SetVATCode(int n, int& nTarget);
	static void SetOfficeMealPluNo(__int64 n, __int64& nTarget);
	static void SetOfficeMealVATBand(int n, int& nTarget);
};

//**********************************************************************

