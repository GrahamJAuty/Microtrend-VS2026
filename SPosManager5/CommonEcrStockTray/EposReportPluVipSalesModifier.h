#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolPluSalesModifier.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportPluVipSalesModifier : public CEposReport
{
public:
	CEposReportPluVipSalesModifier( CEposSelectArray& SelectArray, int nReportType );
	~CEposReportPluVipSalesModifier();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportPluSalesModifierBlock& Block );

private:
	void CalculatePluItemTotals( CEposReportPluSalesModifierBlock& Block, int nStart, int nEnd );
	void WritePluItemTotals( CEposReportPluSalesModifierBlock& Block );
	void WriteBlockTotals( CEposReportPluSalesModifierBlock& Block );
	void WriteConsolidatedTotals( CEposReportPluSalesInfo& Totals, const char* szType, bool bSystemTotal );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	bool m_bAllowValue;
	bool m_bVIPReport;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPluSalesModifierBlock,CEposReportPluSalesModifierBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;

private:
	CEposReportPluSalesInfo m_infoGrandTotals;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
