#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "DefTextAllowance.h"
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewReconcileCash : public CStockTreeReportNew
{
public:
	CStockTreeReportNewReconcileCash( CStockLevelTreeNew& StockLevelTree );
	virtual void PrepareArrays();
	void PrepareReport();

public:
	void SetAllowanceValue ( int nIndex, double dValue );  
	void SetAllowanceTax ( double dValue ) { m_dAllowanceTax = dValue; }
	void SetCashTakings ( double dValue ) { m_dCashTakings = dValue; }
	void SetCashTakingsTax ( double dValue ) { m_dCashTakingsTax = dValue; }
		
private:
	void AddReportColumns();
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	virtual void CreateTotalsLine( int nDepth );

private:
	double m_dAllowanceArray [ MAX_ALLOWANCES + 1 ];
	double m_dAllowanceTax;
	double m_dCashTakings;
	double m_dCashTakingsTax;
};

/**********************************************************************/
#endif
/**********************************************************************/
