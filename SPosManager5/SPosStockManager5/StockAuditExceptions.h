#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

struct CStockAuditExceptionTypes
{
public:
	CStockAuditExceptionTypes();

public:
	bool m_bOpening;
	bool m_bDelivery;
	bool m_bReturns;
	bool m_bSales;
	bool m_bAdjustment;
	bool m_bTransfer;
	bool m_bStocktaking;
	bool m_bShowHeaders;
};

/**********************************************************************/

class CStockAuditExceptions : public CStockAuditBaseReport
{
public:
	CStockAuditExceptions( CStockAuditExceptionTypes types );
	CStockAuditExceptions( int nType );

public:
	void SetPageBreakFlag ( bool bFlag ) { m_bPageBreakFlag = bFlag; }

public:
	bool ProcessHeader( const char* szHeader );
	bool ProcessLine( const char* szLine );

public:
	const char* GetLineSaleDate(){ return m_strLineSaleDate; }
	const char* GetLineSaleTime(){ return m_strLineSaleTime; }
	const char* GetLinePluNo(){ return m_strLinePluNo; }

private:
	void AddColumns( CReportFile& ReportFile );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void WriteHeader( CReportFile& ReportFile );
	void AppendTotals ( CReportFile& ReportFile );

private:
	const char* GetHeaderTypeString();

private:
	CString m_strLinePluNo;
	double m_dLineQty;
	CString m_strLineStockCode;
	CString m_strLineProblem;
	CString m_strLineSaleDate;
	CString m_strLineSaleTime;
	bool m_bLineIsWrapper;
	
private:
	bool m_bPageBreakFlag;
	CStockAuditExceptionTypes m_Types;

private:
	int m_nHeaderType;
	int m_nLineType;
	
private:
	CString m_strHeaderType;
};

/**********************************************************************/
#endif
/**********************************************************************/
