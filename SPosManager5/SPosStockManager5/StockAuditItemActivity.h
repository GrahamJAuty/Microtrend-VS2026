#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditItemActivity : public CStockAuditBaseReport
{
public:
	CStockAuditItemActivity();	

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
	int m_nLineSubUnits;
	double m_dLineSUQty;
	int m_nLineType;
	int m_nLineAction;
	
private:
	bool m_bShowNetChange;
	double m_dNetChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
