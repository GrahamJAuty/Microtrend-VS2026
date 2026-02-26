#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewVariance : public CStockTreeReportNew
{
public:
	CStockTreeReportNewVariance( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:
	void SetZeroSkipFlag( bool b ) { m_bZeroSkip = b; }
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
	
private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );
	
private:
	bool m_bZeroSkip;
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
};

/**********************************************************************/
#endif
/**********************************************************************/
