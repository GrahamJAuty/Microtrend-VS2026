#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditOpeningClosing : public CStockAuditBaseReport
{
public:
	CStockAuditOpeningClosing();

private:
	void AddColumns( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

private:
	CString m_strLineStockCode;
	CString m_strLineDescription;
	CString m_strLineCategory;
	int m_nLineSubUnits;
	int m_nLineType;
	double m_dLineDUQty;
	double m_dLineSUQty;
	double m_dLineNewQty;
};

/**********************************************************************/
#endif
/**********************************************************************/
