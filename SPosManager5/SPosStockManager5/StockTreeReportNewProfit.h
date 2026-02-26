#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewProfit : public CStockTreeReportNew
{
public:
	CStockTreeReportNewProfit( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
		
private:
	void AddReportColumns();
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( int nDepth ){}
	virtual void ShowStockpoints(){}

private:
	void ProcessStockRecord( int nStockSuppIdx );
};

/**********************************************************************/
#endif
/**********************************************************************/
