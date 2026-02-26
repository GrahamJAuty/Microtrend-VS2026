#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewPriceList : public CStockTreeReportNew
{
public:
	CStockTreeReportNewPriceList( CStockLevelTreeNew& StockLevelTree );
	virtual void PrepareArrays();
	void PrepareReport();

public:
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }

private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
};

/**********************************************************************/
#endif
/**********************************************************************/

