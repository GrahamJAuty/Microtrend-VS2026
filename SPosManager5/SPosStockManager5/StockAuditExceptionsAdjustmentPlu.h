#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditExceptionsAdjustmentPlu : public CStockAuditBaseReport
{
public:
	CStockAuditExceptionsAdjustmentPlu();

public:
	const char* GetLinePluNo(){ return m_strLinePluNo; }
	double GetLineQty(){ return m_dLinePluQty; }
	double GetLineVal(){ return m_dLinePluValue; }
	const char* GetLineReason(){ return m_strLineReason; }
	const char* GetLineSaleDate(){ return m_strLineSaleDate; }
	const char* GetLineSaleTime(){ return m_strLineSaleTime; }
		
public:
	bool ProcessLine( const char* szLine );

private:
	void AddColumns( CReportFile& ReportFile );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

private:
	int m_nLineType;
	CString m_strLineType;
	CString m_strLinePluNo;
	CString m_strLineLoc;
	double m_dLinePluQty;
	double m_dLinePluValue;
	CString m_strLineReason;
	CString m_strLineStockCode;
	CString m_strLineProblem;
	CString m_strLineSaleDate;
	CString m_strLineSaleTime;
};

/**********************************************************************/
#endif
/**********************************************************************/
