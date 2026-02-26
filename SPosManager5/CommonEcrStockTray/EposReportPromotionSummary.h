#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolPromotionSummary.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPromotionSummary : public CEposReport
{
public:
	CEposReportPromotionSummary( CEposSelectArray& SelectArray );
	~CEposReportPromotionSummary();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportPromotionSummaryBlock& Block );
	void AddDiscountType( int nDiscType, int nDiscAction, CCSV& csvOut );
	
private:
	void InitialiseConsolidationBlocks();

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPromotionSummaryBlock,CEposReportPromotionSummaryBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/
