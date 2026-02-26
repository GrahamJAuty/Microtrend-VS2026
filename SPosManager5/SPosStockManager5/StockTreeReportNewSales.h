#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
#include "StockReportSalesDlg.h"
/**********************************************************************/
#define REPORT_EPOS_LEVELS 10
/**********************************************************************/

class CStockTreeReportNewSales : public CStockTreeReportNew
{
public:
	CStockTreeReportNewSales( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport( CStockReportSalesDlg* pDlg );
	
public:
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ){ m_bStockUnit = b; }

private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );
	
private:
	bool m_bTotalSales;
	bool m_bEpos[ REPORT_EPOS_LEVELS ];
	bool m_bManual;
	bool m_bImport;
	bool m_bRecipe;
	bool m_bAverage;
	bool m_bTax;
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
	bool m_bZeroSkip;
};

/**********************************************************************/
#endif
/**********************************************************************/
