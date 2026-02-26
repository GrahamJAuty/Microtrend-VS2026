#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewReconcileCategory : public CStockTreeReportNew
{
public:
	CStockTreeReportNewReconcileCategory( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
		
private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );
};

/**********************************************************************/
#endif
/**********************************************************************/
