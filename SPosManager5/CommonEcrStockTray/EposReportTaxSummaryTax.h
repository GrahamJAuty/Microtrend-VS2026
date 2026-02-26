#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolTaxSummaryTax.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportTaxSummaryTax : public CEposReport
{
public:
	CEposReportTaxSummaryTax( CEposSelectArray& SelectArray );
	~CEposReportTaxSummaryTax();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportTaxSummaryTaxBlock& Block );

private:
	void CalculateTaxBandTotals( CEposReportTaxSummaryTaxBlock& Block, int nStart, int nEnd );
	void WriteTaxBandTotals( CEposReportTaxSummaryTaxBlock& Block, int nTaxBand );
	void WriteBlockTotals( CEposReportTaxSummaryTaxBlock& Block );
	void WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType, bool bGrandTotal );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bAllowGlobal;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportTaxSummaryTaxBlock,CEposReportTaxSummaryTaxBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;

private:
	CEposReportPluSalesInfo m_infoGrandTotals;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
