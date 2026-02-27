#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebbleConfig.h"
//**********************************************************************

struct CPebbleVATInfo
{
public:
	CPebbleVATInfo()
	{
		m_nVATCode = 0;
		m_dVATRate = 0.0;
	}

public:
	int m_nVATCode;
	double m_dVATRate;
};

//**********************************************************************

class CSQLRowPebbleConfig
{
public:
	CSQLRowPebbleConfig();
	
public:
	void Reset();
	
public:
	int GetLineID() { return m_nLineID;  }
	CString GetProductionInstallationID() { return m_strProductionInstallationID; }
	CString GetProductionOrganisationID() { return m_strProductionOrganisationID; }
	int GetMISInfoIndex() { return m_nMISInfoIndex; }
	int GetUPNInfoIndex() { return m_nUPNInfoIndex; }
	CString GetLastImportDateTopup() { return m_strLastImportDateTopup; }
	CString GetLastImportTimeTopup() { return m_strLastImportTimeTopup; }
	CString GetLastImportDateSale() { return m_strLastImportDateSale; }
	CString GetLastImportTimeSale() { return m_strLastImportTimeSale; }
	bool GetFirstImportTopupFlag() { return m_bFirstImportTopup; }
	bool GetFirstImportSaleFlag() { return m_bFirstImportSale; }
	int GetRecentCacheDays() { return m_nRecentCacheDays; }
	int GetPaymentCacheDays() { return m_nPaymentCacheDays; }
	double GetVAT_A() { return m_VATInfo[0].m_dVATRate; }
	double GetVAT_B() { return m_VATInfo[1].m_dVATRate; }
	double GetVAT_C() { return m_VATInfo[2].m_dVATRate; }
	double GetVAT_D() { return m_VATInfo[3].m_dVATRate; }
	double GetVAT_E() { return m_VATInfo[4].m_dVATRate; }
	double GetVAT_F() { return m_VATInfo[5].m_dVATRate; }
	double GetVAT_G() { return m_VATInfo[6].m_dVATRate; }
	double GetVAT_H() { return m_VATInfo[7].m_dVATRate; }
	double GetVAT_I() { return m_VATInfo[8].m_dVATRate; }
	double GetVAT_J() { return m_VATInfo[9].m_dVATRate; }
	double GetVAT_K() { return m_VATInfo[10].m_dVATRate; }
	double GetVAT_L() { return m_VATInfo[11].m_dVATRate; }
	int GetVATCode_A() { return m_VATInfo[0].m_nVATCode; }
	int GetVATCode_B() { return m_VATInfo[1].m_nVATCode; }
	int GetVATCode_C() { return m_VATInfo[2].m_nVATCode; }
	int GetVATCode_D() { return m_VATInfo[3].m_nVATCode; }
	int GetVATCode_E() { return m_VATInfo[4].m_nVATCode; }
	int GetVATCode_F() { return m_VATInfo[5].m_nVATCode; }
	int GetVATCode_G() { return m_VATInfo[6].m_nVATCode; }
	int GetVATCode_H() { return m_VATInfo[7].m_nVATCode; }
	int GetVATCode_I() { return m_VATInfo[8].m_nVATCode; }
	int GetVATCode_J() { return m_VATInfo[9].m_nVATCode; }
	int GetVATCode_K() { return m_VATInfo[10].m_nVATCode; }
	int GetVATCode_L() { return m_VATInfo[11].m_nVATCode; }
	__int64 GetOfficeMealPluNo() { return m_nOfficeMealPluNo; }
	int GetOfficeMealVATBand() { return m_nOfficeMealVATBand; }
	
public:
	CString GetActiveOrganisationID();
	CString GetActiveInstallationID();

public:
	void SetLineID(int n) { vdtr.SetLineID(n, m_nLineID); }
	void SetProductionInstallationID(CString str) { vdtr.SetInstallationID(str, m_strProductionInstallationID); }
	void SetProductionOrganisationID(CString str) { vdtr.SetOrganisationID(str, m_strProductionOrganisationID); }
	void SetMISInfoIndex(int n) { vdtr.SetMISInfoIndex(n, m_nMISInfoIndex); }
	void SetUPNInfoIndex(int n) { vdtr.SetUPNInfoIndex(n, m_nUPNInfoIndex); }
	void SetLastImportDateTopup(CString str) { vdtr.SetLastImportDateTopup(str, m_strLastImportDateTopup); }
	void SetLastImportTimeTopup(CString str) { vdtr.SetLastImportTimeTopup(str, m_strLastImportTimeTopup); }
	void SetLastImportDateSale(CString str) { vdtr.SetLastImportDateSale(str, m_strLastImportDateSale); }
	void SetLastImportTimeSale(CString str) { vdtr.SetLastImportTimeSale(str, m_strLastImportTimeSale); }
	void SetFirstImportTopupFlag(bool b) { m_bFirstImportTopup = b; }
	void SetFirstImportSaleFlag(bool b) { m_bFirstImportSale = b; }
	void SetRecentCacheDays(int n) { m_nRecentCacheDays = n; }
	void SetPaymentCacheDays(int n) { m_nPaymentCacheDays = n; }
	void SetVAT_A(double d) { vdtr.SetVATRate(d, m_VATInfo[0].m_dVATRate); }
	void SetVAT_B(double d) { vdtr.SetVATRate(d, m_VATInfo[1].m_dVATRate); }
	void SetVAT_C(double d) { vdtr.SetVATRate(d, m_VATInfo[2].m_dVATRate); }
	void SetVAT_D(double d) { vdtr.SetVATRate(d, m_VATInfo[3].m_dVATRate); }
	void SetVAT_E(double d) { vdtr.SetVATRate(d, m_VATInfo[4].m_dVATRate); }
	void SetVAT_F(double d) { vdtr.SetVATRate(d, m_VATInfo[5].m_dVATRate); }
	void SetVAT_G(double d) { vdtr.SetVATRate(d, m_VATInfo[6].m_dVATRate); }
	void SetVAT_H(double d) { vdtr.SetVATRate(d, m_VATInfo[7].m_dVATRate); }
	void SetVAT_I(double d) { vdtr.SetVATRate(d, m_VATInfo[8].m_dVATRate); }
	void SetVAT_J(double d) { vdtr.SetVATRate(d, m_VATInfo[9].m_dVATRate); }
	void SetVAT_K(double d) { vdtr.SetVATRate(d, m_VATInfo[10].m_dVATRate); }
	void SetVAT_L(double d) { vdtr.SetVATRate(d, m_VATInfo[11].m_dVATRate); }
	void SetVATCode_A(int n) { vdtr.SetVATCode(n, m_VATInfo[0].m_nVATCode); }
	void SetVATCode_B(int n) { vdtr.SetVATCode(n, m_VATInfo[1].m_nVATCode); }
	void SetVATCode_C(int n) { vdtr.SetVATCode(n, m_VATInfo[2].m_nVATCode); }
	void SetVATCode_D(int n) { vdtr.SetVATCode(n, m_VATInfo[3].m_nVATCode); }
	void SetVATCode_E(int n) { vdtr.SetVATCode(n, m_VATInfo[4].m_nVATCode); }
	void SetVATCode_F(int n) { vdtr.SetVATCode(n, m_VATInfo[5].m_nVATCode); }
	void SetVATCode_G(int n) { vdtr.SetVATCode(n, m_VATInfo[6].m_nVATCode); }
	void SetVATCode_H(int n) { vdtr.SetVATCode(n, m_VATInfo[7].m_nVATCode); }
	void SetVATCode_I(int n) { vdtr.SetVATCode(n, m_VATInfo[8].m_nVATCode); }
	void SetVATCode_J(int n) { vdtr.SetVATCode(n, m_VATInfo[9].m_nVATCode); }
	void SetVATCode_K(int n) { vdtr.SetVATCode(n, m_VATInfo[10].m_nVATCode); }
	void SetVATCode_L(int n) { vdtr.SetVATCode(n, m_VATInfo[11].m_nVATCode); }
	void SetOfficeMealPluNo(__int64 n) { vdtr.SetOfficeMealPluNo(n, m_nOfficeMealPluNo); }
	void SetOfficeMealVATBand(int n) { vdtr.SetOfficeMealVATBand(n, m_nOfficeMealVATBand); }

public:
	CPebbleVATInfo GetVATInfo(int n)
	{
		if ((n >= 0) && (n < 12))
		{
			return m_VATInfo[n];
		}
		else
		{
			return CPebbleVATInfo();
		}
	}
	
	void SetVATInfo(int n, CPebbleVATInfo& VATInfo)
	{
		if ((n >= 0) && (n < 12))
		{
			m_VATInfo[n] = VATInfo;
		}
	}

private:
	int m_nLineID;
	CString m_strProductionInstallationID;
	CString m_strProductionOrganisationID;
	int m_nMISInfoIndex;
	int m_nUPNInfoIndex;
	CString m_strLastImportDateTopup;
	CString m_strLastImportTimeTopup;
	CString m_strLastImportDateSale;
	CString m_strLastImportTimeSale;
	bool m_bFirstImportTopup;
	bool m_bFirstImportSale;
	int m_nRecentCacheDays;
	int m_nPaymentCacheDays;
	__int64 m_nOfficeMealPluNo;
	int m_nOfficeMealVATBand;
	
private:
	CPebbleVATInfo m_VATInfo[12];

private:
	CSQLFieldValidatorPebbleConfig vdtr;
};

//**********************************************************************

