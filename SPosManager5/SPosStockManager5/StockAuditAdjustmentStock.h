#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditAdjustmentStock : public CStockAuditBaseReport
{
public:
	CStockAuditAdjustmentStock();

public:
	void SetPageBreakFlag ( bool bFlag ) { m_bPageBreakFlag = bFlag; }
	
private:
	void AddColumns( CReportFile& ReportFile );
	bool ProcessHeader( const char* szHeader );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void WriteHeader( CReportFile& ReportFile );
	void AppendTotals( CReportFile& ReportFile );

private:
	CString m_strLineStockCode;
	CString m_strLineDescription;
	CString m_strLineCategory;
	int m_nLineSubUnits;
	double m_dLineStockQty;
	double m_dLineStockValue;
	double m_dLineStockCost;
	CString m_strLineReason;

private:
	double m_dSessionStockQty;
	double m_dSessionStockValue;
	double m_dSessionStockCost;
	bool m_bPageBreakFlag;
};

/**********************************************************************/
#endif
/**********************************************************************/
