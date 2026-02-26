#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

struct CApparentStockInfoNew
{
public:
	int m_nStockIdx;
	double m_dOpening;
	double m_dDelivery;
	double m_dFree;
	double m_dReturns;
	double m_dTransferIn;
	double m_dSales;
	double m_dWastage;
	double m_dTransferOut;
	double m_dAdjustment;
	double m_dStocktake;
	double m_dTotalQty;
	double m_dTotalValue;
};

/**********************************************************************/

class CStockTreeReportNewTop100Apparent : public CStockTreeReportNew
{
public:
	CStockTreeReportNewTop100Apparent( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:	
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
		
private:
	void AddReportColumns();
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( int nDepth );

private:
	int GetChartInsertPos( CApparentStockInfoNew& info );

private:
	CArray<CApparentStockInfoNew,CApparentStockInfoNew> m_StockLevelChart;

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
};

/**********************************************************************/
#endif
/**********************************************************************/
