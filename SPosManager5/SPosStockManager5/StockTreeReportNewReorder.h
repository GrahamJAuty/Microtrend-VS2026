#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewReorder : public CStockTreeReportNew
{
public:
	CStockTreeReportNewReorder( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:
	void SetDateFrom( const char* sz ) { m_strDateFrom = sz; }
	void SetDateTo( const char* sz ) { m_strDateTo = sz; }
	void SetSeparateStockpointsFlag( bool b ) { m_bSeparateStockpoints = b; }

public:
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
	
private:
	void AddReportColumns();
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );

private:
	CString m_strDateFrom;
	CString m_strDateTo;
	bool m_bSeparateStockpoints;

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;

private:
	CString m_strItemTotalsUnderline;
	int m_nGrandTotalColumn;
};

/**********************************************************************/
#endif
/**********************************************************************/
