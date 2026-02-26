#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewApparentDU : public CStockTreeReportNew
{
public:
	CStockTreeReportNewApparentDU( bool bFull, CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();
	
public:
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }
	void SetHideAboveMinFlag( bool b ) { m_bHideAboveMin = b; }
	void SetPositiveFlag( bool b ) { m_bPositive = b; }
	void SetZeroFlag( bool b ) { m_bZero = b; }
	void SetNegativeFlag( bool b ) { m_bNegative = b; }
	
	bool GetHideAboveMinFlag(){ return m_bHideAboveMin; }
	bool GetPositiveFlag(){ return m_bPositive; }
	bool GetZeroFlag(){ return m_bZero; }
	bool GetNegativeFlag(){ return m_bNegative; }

private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );

private:
	void ProcessAmount( CStockReportLine& csv, double dAmount, double dYield, int nTotalPos );
	void ProcessTotal( CStockReportLine& csv, int nDepth, int nTotalPos );

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;
	bool m_bHideAboveMin;
	bool m_bPositive;
	bool m_bZero;
	bool m_bNegative;
	bool m_bFull;
};

/**********************************************************************/
#endif
/**********************************************************************/
