#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/
#define BUFFER_TYPE_LINE 0
#define BUFFER_TYPE_ORDER 1
#define BUFFER_TYPE_AUDIT 2
/**********************************************************************/

struct CStockAuditReturnsBuffer
{
public:
	CStockAuditReturnsBuffer( int nBufferType );
	void Reset();

public:
	void Consolidate( CStockAuditReturnsBuffer& source );
	
public:
	int m_nBufferType;
	CString m_strStockCode;
	CString m_strDescription;
	CString m_strCategory;
	CString m_strSuppRef;
	int m_nDUItems;
	double m_dDUItemSize;
	double m_dCaseQty;
	double m_dCaseCost;
	double m_dStockUnitQty;
	double m_dTotalCost;
	int m_nHighestDataVersion;
	bool m_bUnknownFormat;
};

/**********************************************************************/

class CStockAuditReturns : public CStockAuditBaseReport
{
public:
	CStockAuditReturns();

public:
	void SetPageBreakFlag ( bool bFlag ) { m_bPageBreakFlag = bFlag; }

private:
	void AddColumns( CReportFile& ReportFile );
	bool ProcessHeader( const char* szHeader );
	void WriteHeader( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void AppendTotals ( CReportFile& ReportFile );
	void AppendGrandTotals ( CReportFile& ReportFile );

private:
	void WriteBuffer( CReportFile& ReportFile, CStockAuditReturnsBuffer& buffer );
		
private:
	bool m_bDetail;
	bool m_bPageBreakFlag;	
	
private:
	CStockAuditReturnsBuffer m_bufferLine;
	CStockAuditReturnsBuffer m_bufferOrder;
	CStockAuditReturnsBuffer m_bufferAudit;

private:
	CString m_strHeaderBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
