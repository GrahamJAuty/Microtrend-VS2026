#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditStocktake : public CStockAuditBaseReport
{
public:
	CStockAuditStocktake();

private:
	void AddColumns( CReportFile& ReportFile );
	void WriteHeader( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void AppendTotals( CReportFile& ReportFile );

private:
	CString m_strLineStockCode;
	CString m_strLineDescription;
	CString m_strLineCategory;
	int m_nLineSubUnits;
	double m_dLineApparentQty;
	double m_dLineActualQty;
};

/**********************************************************************/
#endif
/**********************************************************************/
