#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewStockSheet : public CStockTreeReportNew
{
public:
	CStockTreeReportNewStockSheet( CStockLevelTreeNew& StockLevelTree );
	virtual void PrepareArrays();
	void PrepareReport();

public:
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	
private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( int nDepth ){}

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
};

/**********************************************************************/
#endif
/**********************************************************************/
