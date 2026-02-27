#pragma once
//**********************************************************************
#ifndef SYSTEMTYPE_SERVER
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
//**********************************************************************
#endif
//**********************************************************************
#include "TaxCodes.h"
//**********************************************************************

class CTLogDatabase : public CSSUnsortedDatabase
{
public:
	CTLogDatabase(void);
	int Open ( const char* szFilename );

	bool FindSalesText (  const char* szDate, const char* szTime, int nCCNo );
	CString GetSalesText() { return m_strSalesText; }
	
#ifndef SYSTEMTYPE_SERVER
	void GetPebbleTransactionItems(const char* szDate, const char* szTime, int nCCNo, CArray<CPebbleTransactionItem, CPebbleTransactionItem>& arrayItems);
#endif

	bool FindSalesTaxes ( const char* szDate, const char* szTime, int nCCNo );
	double m_dSales[nMAX_TAXCODES];
	double m_dSalesUnknown;

private:
	CString MakeKey ( const char* szDate, const char* szTime );
	bool LocateSale ( int nCCNo, const char* szTime );

private:
	CString m_strSalesText;
};

//**********************************************************************
