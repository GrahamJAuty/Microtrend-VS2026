#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolPluSalesDept.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportPluVipSalesDept : public CEposReport
{
public:
	CEposReportPluVipSalesDept( CEposSelectArray& SelectArray, int nReportType );
	~CEposReportPluVipSalesDept();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportPluSalesDeptBlock& Block );

private:
	void CalculatePluItemTotals( CEposReportPluSalesDeptBlock& Block, int nTaxBand, int nStart, int nEnd );
	void WritePluItemTotals( CEposReportPluSalesDeptBlock& Block, int nTaxBand );
	void WriteDepartmentTotal ( CEposReportPluSalesDeptBlock& Block, int nDeptNo );
	void WriteBlockTotals( CEposReportPluSalesDeptBlock& Block );
	void WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType, bool bDeptTotal, bool bGrandTotal );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	bool m_bAllowValue;
	bool m_bVIPReport;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPluSalesDeptBlock,CEposReportPluSalesDeptBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;

private:
	CReportPluInfoFinder m_PluInfoFinder;
	CEposReportPluSalesInfo m_infoGrandTotals;
};

/**********************************************************************/
