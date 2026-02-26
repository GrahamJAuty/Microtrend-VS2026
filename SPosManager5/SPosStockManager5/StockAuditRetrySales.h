#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/
#define STOCK_XXX_SALES 1
#define STOCK_XXX_WASTAGE 2
#define STOCK_XXX_DELIVERY 3
/**********************************************************************/

class CStockAuditRetrySales : public CStockAuditBaseReport
{
public:
	CStockAuditRetrySales();
	
public:
	const char* GetHeaderLine( COleDateTime& date, const char* szDateOfSale, const char* szTimeOfSale, const char* szEcrName );

public:
	bool ProcessLine( const char* szLine );
	bool ProcessHeader( const char* szHeader );

public:
	const char* GetHeaderDateOfSale(){ return m_strHeaderDateOfSale; }
	const char* GetHeaderTimeOfSale(){ return m_strHeaderTimeOfSale; }
	const char* GetHeaderEcrName(){ return m_strHeaderEcrName; }

public:
	int GetLineActionType(){ return m_nLineActionType; }
	const char* GetLinePluNo(){ return m_strLinePluNo; }
	int GetLinePluLevel(){ return m_nLinePluLevel; }
	double GetLinePluQty(){ return m_dLinePluQty; }
	double GetLinePluValue(){ return m_dLinePluValue; }
	
private:
	void AddColumns( CReportFile& ReportFile );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );
	void AfterReport( CReportFile& ReportFile );
	
private:
	CString m_strHeaderDateOfSale;
	CString m_strHeaderTimeOfSale;
	CString m_strHeaderEcrName;

private:
	int m_nLineActionType;
	CString m_strLineActionType;
	CString m_strLinePluNo;
	int m_nLinePluLevel;
	double m_dLinePluQty;
	double m_dLinePluValue;

private:
	bool m_bSpecialOK;
	int m_nLastSpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
