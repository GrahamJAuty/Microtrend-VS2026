#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewTransferClosing : public CStockTreeReportNew
{
public:
	CStockTreeReportNewTransferClosing( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();

public:
	void PrepareStockpointList();
	void SetSupplierNameFlag( bool b ) { m_bSupplierName = b; }
	void SetSupplierRefFlag( bool b ) { m_bSupplierRef = b; }
	void SetStockUnitFlag( bool b ) { m_bStockUnit = b; }

private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord& TransferDetail );
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth );

private:
	bool m_bSupplierName;
	bool m_bSupplierRef;
	bool m_bStockUnit;

private:
	CArray<int,int> m_arraySpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
