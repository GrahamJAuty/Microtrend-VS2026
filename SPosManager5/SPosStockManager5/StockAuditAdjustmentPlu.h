#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditAdjustmentPlu : public CStockAuditBaseReport
{
public:
	CStockAuditAdjustmentPlu();

public:
	void SetPageBreakFlag ( bool bFlag ) { m_bPageBreakFlag = bFlag; }
	
private:
	void AddColumns( CReportFile& ReportFile );
	bool ProcessHeader( const char* szHeader );
	bool ProcessLine( const char* szLine );
	void WriteHeader( CReportFile& ReportFile );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void AppendTotals( CReportFile& ReportFile );

private:
	int m_nLineType;
	CString m_strLinePluNo;
	CString m_strLineDescription;
	double m_dLinePluQty;
	double m_dLinePluValue;
	CString m_strLineStockCode;
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
