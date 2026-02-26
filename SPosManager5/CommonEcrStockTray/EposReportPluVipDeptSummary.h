#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolDeptSummary.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPluVipDeptSummary : public CEposReport
{
public:
	CEposReportPluVipDeptSummary( CEposSelectArray& SelectArray, int nReportType );
	~CEposReportPluVipDeptSummary();

	virtual void Consolidate();
	virtual bool CreateReport();
	virtual bool CreateGraph();
	virtual bool CheckGraph();

private:
	void CreateSalesSection( CEposReportDeptSummaryBlock& Block );

private:
	void WriteBlockTotals( CEposReportDeptSummaryBlock& Block );
	void WriteConsolidatedTotals( CEposReportConsolDeptSummaryTax& Block, const char* szType, bool bSystemTotal );
	void PostConsolidateSalesTax( CEposReportDeptSummaryBlock& Block, CArray<CEposReportConsolDeptSummaryTax,CEposReportConsolDeptSummaryTax>& arraySales );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bAllowGlobal;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	bool m_bAllowValue;
	bool m_bVIPReport;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportDeptSummaryBlock,CEposReportDeptSummaryBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/
