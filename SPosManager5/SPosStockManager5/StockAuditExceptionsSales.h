/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditExceptionsSales : public CStockAuditBaseReport
{
public:
	CStockAuditExceptionsSales();
	
public:
	void SetE1SalesFlag ( bool bFlag ) { m_bE1Sales = bFlag; }
	void SetImportSalesFlag ( bool bFlag ) { m_bImportSales = bFlag; }
	void SetManualSalesFlag ( bool bFlag ) { m_bManualSales = bFlag; }

public:
	const char* GetLineLoc(){ return m_strLineLoc; }
	const char* GetLinePluNo(){ return m_strLinePluNo; }
	double GetLineQty(){ return m_dLinePluQty; }
	double GetLineVal(){ return m_dLinePluValue; }
	const char* GetLineSaleDate(){ return m_strLineSaleDate; }
	const char* GetLineSaleTime(){ return m_strLineSaleTime; }

public:
	bool ProcessLine( const char* szLine );
	
private:
	void AddColumns( CReportFile& ReportFile );
	void WriteLine( CReportFile& ReportFile, int nSpIdx );

private:
	bool m_bE1Sales;
	bool m_bImportSales;
	bool m_bManualSales;

private:
	int m_nLineType;
	CString m_strLineType;
	CString m_strLinePluNo;
	CString m_strLineLoc;
	double m_dLinePluQty;
	double m_dLinePluValue;
	CString m_strLineStockCode;
	CString m_strLineProblem;
	CString m_strLineSaleDate;
	CString m_strLineSaleTime;
	bool m_bLineIsWrapper;
};

/**********************************************************************/
#endif
/**********************************************************************/
