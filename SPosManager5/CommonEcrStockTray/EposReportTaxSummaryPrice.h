#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolTaxSummaryPrice.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportTaxSummaryPrice : public CEposReport
{
public:
	CEposReportTaxSummaryPrice( CEposSelectArray& SelectArray );
	~CEposReportTaxSummaryPrice();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportTaxSummaryPriceBlock& Block );

private:
	void CalculatePriceLevelTotals( CEposReportTaxSummaryPriceBlock& Block, int nStart, int nEnd );
	void WritePriceLevelTotals( CEposReportTaxSummaryPriceBlock& Block, int nTaxBand );
	void WriteBlockTotals( CEposReportTaxSummaryPriceBlock& Block );
	void WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportTaxSummaryPriceBlock,CEposReportTaxSummaryPriceBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;

private:
	CEposReportPluSalesInfo m_infoGrandTotals;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
