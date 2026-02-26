#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolCheapestOneFreeSummary.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCheapestOneFreeSummary : public CEposReport
{
public:
	CEposReportCheapestOneFreeSummary( CEposSelectArray& SelectArray );
	~CEposReportCheapestOneFreeSummary();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportCheapestOneFreeSummaryBlock& Block );
	void ShowPromos( CEposReportCheapestOneFreeSummaryBlock& Block, bool bTotals );
	
private:
	void InitialiseConsolidationBlocks();

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCheapestOneFreeSummaryBlock,CEposReportCheapestOneFreeSummaryBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/

