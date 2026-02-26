#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewTop100Sales : public CStockTreeReportNew
{
public:
	CStockTreeReportNewTop100Sales( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:
	void SetTimeType( int n ) { m_nTimeType = n; }
	
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
		
private:
	void AddReportColumns();
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( int nDepth );

private:
	int GetChartInsertPos( CPeriodChartItemNew& info );

private:
	CArray<CPeriodChartItemNew,CPeriodChartItemNew> m_SalesChart;

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;

private:
	int m_nTimeType;
};

/**********************************************************************/
#endif
/**********************************************************************/
