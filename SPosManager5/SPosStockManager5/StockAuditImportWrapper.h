#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditImportWrapper : public CStockAuditBaseReport
{
public:
	CStockAuditImportWrapper();
	
private:
	void AddColumns( CReportFile& ReportFile );
	void WriteHeader( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

protected:
	CString m_strLineWrapper;
	CString m_strLineQty;
	CString m_strLineValue;
	CString m_strLineProblem;
};

/**********************************************************************/
#endif
/**********************************************************************/
