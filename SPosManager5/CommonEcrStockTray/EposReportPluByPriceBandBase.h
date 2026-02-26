#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolPluSales.h"
#include "EposReportConsolTran.h"
#include "EposReportPluSalesInfo.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
#include "SportsBookerPriceBandCSVArray.h"
/**********************************************************************/

class CEposReportPluByPriceBandBase : public CEposReport
{
public:
	CEposReportPluByPriceBandBase( CEposSelectArray& SelectArray, CString strCustomSettings);
	~CEposReportPluByPriceBandBase();

	virtual void Consolidate();
	virtual bool CreateReport(){ return FALSE; }

protected:
	void CreateSalesSection( CEposReportPluByPriceBandBlock& Block );
	
protected:
	bool CalculatePluItemTotals(CEposReportPluByPriceBandBlock& Block, int nTaxBand, int nStart, int nEnd );
	void WritePluItemTotals(CEposReportPluByPriceBandBlock& Block, int nTaxBand );
	void WriteBlockTotals(CEposReportPluByPriceBandBlock& Block );
	void WriteConsolidatedTotals( CEposReportPluByPriceBandInfo& Totals, const char* szType, int nCovers, bool bGrandTotal );

protected:
	virtual void InitialiseConsolidationBlocks(){}
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo ){}

protected:
	void InitialiseConsolidationBlocksStageOne();
	void InitialiseConsolidationBlocksStageTwo();

protected:
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	bool m_bVIPReport;
	bool m_bAllowValue;
	int m_nServerBlockIdxStart;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPluByPriceBandBlock,CEposReportPluByPriceBandBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CEposReportCustomSettingsPluPriceBand m_ReportSettings;

protected:
	CEposReportPluByPriceBandInfo m_Totals1;
	CReportConsolidationArray<CEposReportConsolTranServer> m_arrayTranServer;
	CReportPluInfoFinder m_PluInfoFinder;
	int m_nTotalCovers;
};

/**********************************************************************/
