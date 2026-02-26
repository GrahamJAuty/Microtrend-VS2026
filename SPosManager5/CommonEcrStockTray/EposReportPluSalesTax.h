#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolPluSalesTax.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportPluSalesTax : public CEposReport
{
public:
	CEposReportPluSalesTax( CEposSelectArray& SelectArray );
	~CEposReportPluSalesTax();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportPluSalesTaxBlock& Block );

private:
	void CalculatePluItemTotals( CEposReportPluSalesTaxBlock& Block, int nStart, int nEnd );
	void WritePluItemTotals( CEposReportPluSalesTaxBlock& Block );
	void WriteTaxBandTotal ( CEposReportPluSalesTaxBlock& Block, int nTaxBand );
	void WriteBlockTotals( CEposReportPluSalesTaxBlock& Block );
	CString WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType, bool bTaxBandTotal, bool bGrandTotal );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPluSalesTaxBlock,CEposReportPluSalesTaxBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;

private:
	CReportPluInfoFinder m_PluInfoFinder;
	CEposReportPluSalesInfo m_infoGrandTotals;
	CStringArray m_arrayTaxBandTotals;
};

/**********************************************************************/
