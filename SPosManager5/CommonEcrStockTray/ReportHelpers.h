#pragma once
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/
#define REPORT_TAXBAND_UNKNOWN_TAXBAND 1000
#define REPORT_TAXBAND_UNKNOWN_PLU 1001
#define REPORT_TAXBAND_NONTAX_DISCOUNT 1002
/**********************************************************************/

class CReportHelpers  
{
public:
	double CalcGP ( double dSalesValue, double dTaxRate, double dCostPrice );
	double CalcGPPercent ( double dSalesValue, double dTaxRate, double dCostPrice );	
	double CalcNonTax ( double dSalesValue, double dTaxRate );
	double CalcAverage ( double dQty, double dValue );
	
	const char* GetDisplayPluNo ( __int64 nPluNo, bool bAllowZero = FALSE );
	const char* GetReportText ( CPluCSVRecord& PluRecord );

	bool CanUseSystemPluFilter( int nConType, int nPluFilterNo );
	bool CheckFilterPluNo( bool bSystem, int nFilterNo, CSQLPluNoInfo& infoPlu );

	void CreateTabRuler( CReportFileDoc& ReportFileDoc, const char* szParamsFilename, const char* szKey, CTabRuler& TabRuler );

	int GetNumericTaxBandFromString( CString strTaxBand );
	CString GetTaxBandString( int nTaxBand );

#ifdef STOCKMAN_SYSTEM
	const char* GetDisplayStockCode ( const char* szCode );
#endif

private:
	static UINT HashString(LPCTSTR key);

private:
	CString m_strDisplayPluNo;
	CString m_strDisplayStockCode;
	CString m_strReportText;
};

/**********************************************************************/
extern CReportHelpers ReportHelpers;
/**********************************************************************/
