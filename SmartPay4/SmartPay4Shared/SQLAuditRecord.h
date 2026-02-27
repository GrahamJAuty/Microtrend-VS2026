#pragma once
//*******************************************************************
#include "SQLAuditRecord_base.h"
#include "TaxCodes.h"
#include "TLogDatabase.h"
//*******************************************************************

class CSQLAuditRecord : public CSQLAuditRecord_base
{
public:
	CSQLAuditRecord();
	CString ExtractPurchaseText(CTLogDatabase* pDatabase, bool bNewDatabase);
	bool ExtractSalesTaxes ( const char* szFilename = "" );

	bool IsDeleteAppNo();
	bool IsWebPaymentAppNo();
	bool IsEODEntry();

public:
	double m_dSales[nMAX_TAXCODES];
	double m_dSalesUnknown;
	double m_dNotFoundSales;

public:
	static CString GetHistoryFilenameStatic (CString strUserID);

private:
	CString GetHistoryFilename();
};

//*******************************************************************
