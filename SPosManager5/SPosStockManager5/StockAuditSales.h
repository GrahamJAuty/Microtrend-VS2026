#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditSales : public CStockAuditBaseReport
{
public:
	CStockAuditSales();

public:
	void SetE1SalesFlag ( bool bFlag ) { m_bE1Sales = bFlag; }
	void SetManualSalesFlag ( bool bFlag ) { m_bManualSales = bFlag; }
	void SetImportSalesFlag ( bool bFlag ) { m_bImportSales = bFlag; }
	void SetHidePluNoFlag( bool bFlag ) { m_bHidePluNo = bFlag; }
	
private:
	void AddColumns( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

private:
	bool m_bIncludeRecipeItems;
	bool m_bE1Sales;
	bool m_bManualSales;
	bool m_bImportSales;
	bool m_bHidePluNo;
	bool m_bIsStockOnlyLine;

private:
	int m_nLineType;
	CString m_strLineType;
	CString m_strLinePluNo;
	CString m_strLineLoc;
	CString m_strLineDescription;
	double m_dLinePluQty;
	double m_dLinePluValue;
	CString m_strLineStockCode;
	int m_nLineSubUnits;
	double m_dLineStockQty;
	double m_dLineStockValue;
	CString m_strLineSaleDate;
	CString m_strLineSaleTime;
};

/**********************************************************************/
#endif
/**********************************************************************/
