#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditTransfer : public CStockAuditBaseReport
{
public:
	CStockAuditTransfer();	

public:
	void SetPageBreakFlag ( bool bFlag ) { m_bPageBreakFlag = bFlag; }
	
public:
	void AddColumns( CReportFile& ReportFile );
	void WriteHeader( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void AppendTotals( CReportFile& ReportFile );
	
private:
	bool m_bPageBreakFlag;

private:
	CString m_strLineStockCode;
	CString m_strLineDescription;
	CString m_strLineCategory;
	int m_nLineSubUnits;
	double m_dLineDUQty;
	double m_dLineSUQty;
};

/**********************************************************************/
#endif
/**********************************************************************/
