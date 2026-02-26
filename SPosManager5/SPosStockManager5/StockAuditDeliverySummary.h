#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

class CStockAuditDeliverySummary : public CStockAuditBaseReport
{
public:
	CStockAuditDeliverySummary();
	
private:
	void AddColumns( CReportFile& ReportFile );
	bool ProcessLine( const char* szLine );
	bool ProcessHeader( const char* szLine );
	void AppendTotals ( CReportFile& ReportFile );
	void AppendStockpointTotals ( CReportFile& ReportFile );
	void AppendGrandTotals ( CReportFile& ReportFile );

private:
	double m_dSessionCaseQty;
	double m_dSessionStockUnitQty;
	double m_dSessionOrderCost;
	double m_dSpCaseQty;
	double m_dSpStockUnitQty;
	double m_dSpOrderCost;
	double m_dTotalCaseQty;
	double m_dTotalStockUnitQty;
	double m_dTotalOrderCost;
};

/**********************************************************************/
#endif
/**********************************************************************/
