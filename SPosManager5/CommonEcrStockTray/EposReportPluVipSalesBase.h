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

class CEposReportPluVipSalesBase : public CEposReport
{
public:
	CEposReportPluVipSalesBase( CEposSelectArray& SelectArray );
	~CEposReportPluVipSalesBase();

	virtual void Consolidate();
	virtual bool CreateReport(){ return FALSE; }

protected:
	void CreateSalesSection( CEposReportPluSalesBlock& Block );
	
protected:
	bool CalculatePluItemTotals( CEposReportPluSalesBlock& Block, int nTaxBand, int nStart, int nEnd );
	void WritePluItemTotals( CEposReportPluSalesBlock& Block, int nTaxBand );
	void WriteBlockTotals( CEposReportPluSalesBlock& Block );
	void WriteConsolidatedTotals( CEposReportPluSalesInfoArray& arrayTotals, const char* szType, int nCovers, bool bGrandTotal );

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
	bool m_bAllowValue;
	bool m_bVIPReport;
	int m_nServerBlockIdxStart;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPluSalesBlock,CEposReportPluSalesBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;

protected:
	CEposReportPluSalesInfoArray m_arrayTotals1;
	CReportConsolidationArray<CEposReportConsolTranServer> m_arrayTranServer;
	CReportPluInfoFinder m_PluInfoFinder;
	int m_nTotalCovers;
};

/**********************************************************************/
