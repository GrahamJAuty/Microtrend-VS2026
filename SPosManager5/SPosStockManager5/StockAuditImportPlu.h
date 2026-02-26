#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditImportPlu : public CStockAuditBaseReport
{
public:
	CStockAuditImportPlu();
	
private:
	void AddColumns( CReportFile& ReportFile );
	void WriteHeader( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

protected:
	CString m_strLineBarcodeNo;
	CString m_strLinePluNo;
	CString m_strLineDescription;
	CString m_strLineLevel;
	CString m_strLineQty;
	CString m_strLineValue;
	CString m_strLineProblem;
};

/**********************************************************************/
#endif
/**********************************************************************/
