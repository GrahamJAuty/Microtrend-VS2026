#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewAdjustment : public CStockTreeReportNew
{
public:
	CStockTreeReportNewAdjustment( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();

public:
	void SetTimeType( int n ) { m_nTimeType = n; }
	void SetSalesFlag( bool b ) { m_bSales = b; }
	void SetBreakdownFlag( bool b ) { m_bBreakdown = b; }
	void SetAdjustFlag( bool b ) { m_bAdjust = b; }
	void SetWasteEcrFlag( bool b ) { m_bWasteEcr = b; }
	void SetWasteAutoFlag( bool b ) { m_bWasteAuto = b; }
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
	
private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );

private:
	int m_nTimeType;
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
	bool m_bSales;
	bool m_bBreakdown;
	bool m_bAdjust;
	bool m_bWasteEcr;
	bool m_bWasteAuto;
};

/**********************************************************************/
#endif
/**********************************************************************/
