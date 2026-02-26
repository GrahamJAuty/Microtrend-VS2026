#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolPluSales.h"
#include "EposReportPluSalesInfo.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportPluSalesCustomer : public CEposReport
{
public:
	CEposReportPluSalesCustomer( CEposSelectArray& SelectArraye );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void WritePluItemTotals( int nTaxBand, int nStart, int nEnd, CEposReportPluSalesInfo arrayConsolidatedTotals1[], CEposReportPluSalesInfo arrayConsolidatedTotals2[], bool bShowInReport );
	void WriteConsolidatedTotals( const char* szType, CEposReportPluSalesInfo arrayConsolidatedTotals[], bool bUnderline, bool bGrandTotalOnly );

private:
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	CReportConsolidationArray<CEposReportConsolPluSalesCustomerID> m_ConsolArrayID;
	CReportConsolidationArray<CEposReportConsolPluSalesCustomerSortIdx> m_ConsolArraySortIdx;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
