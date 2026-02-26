#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditImportAdjustmentPlu : public CStockAuditBaseReport
{
public:
	CStockAuditImportAdjustmentPlu( int nLineType );
	
private:
	void AddColumns( CReportFile& ReportFile );
	void WriteHeader( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

protected:
	int m_nLineType;

protected:
	CString m_strLineBarcodeNo;
	CString m_strLinePluNo;
	CString m_strLineDescription;
	CString m_strLineLevel;
	CString m_strLineQty;
	CString m_strLineValue;
	CString m_strLineReason;
	CString m_strLineProblem;
};

/**********************************************************************/
#endif
/**********************************************************************/
