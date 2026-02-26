#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewSupplier : public CStockTreeReportNew
{
public:
	CStockTreeReportNewSupplier( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
	
private:
	void AddReportColumns();
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( int nDepth ){}

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;

private:
	bool m_bGotStockpointField;
};

/**********************************************************************/
#endif
/**********************************************************************/

