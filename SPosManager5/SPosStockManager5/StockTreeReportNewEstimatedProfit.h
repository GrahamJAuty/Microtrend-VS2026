#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewEstimatedProfit : public CStockTreeReportNew
{
public:
	CStockTreeReportNewEstimatedProfit( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();

public:
	void SetTimeType( int n ) { m_nTimeType = n; }
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }

private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CSalesCostCSVRecord& SalesCost );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );
	
private:
	int m_nTimeType;
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
};

/**********************************************************************/
#endif
/**********************************************************************/
