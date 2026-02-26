#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewYtdSales : public CStockTreeReportNew
{
public:
	CStockTreeReportNewYtdSales( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:
	void SetPeriodSalesFlag( bool b ) { m_bPeriodSales = b; }
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
	
private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );

private:
	bool m_bPeriodSales;
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
};

/**********************************************************************/
#endif
/**********************************************************************/
