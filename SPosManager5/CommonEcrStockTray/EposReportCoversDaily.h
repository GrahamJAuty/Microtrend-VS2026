#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolCoversDaily.h"
#include "EposReportConsolTran.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCoversDaily : public CEposReport
{
public:
	CEposReportCoversDaily( CEposSelectArray& SelectArray );
	~CEposReportCoversDaily();

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	void CreateSalesSection( CEposReportCoversDailyBlock& Block );

private:	
	void WriteBlockTotals( CEposReportCoversDailyBlock& Block );

private:
	void InitialiseConsolidationBlocks();
	void PostConsolidateSalesTax( CEposReportCoversDailyBlock& Block, CArray<CEposReportConsolCoversDailyTax,CEposReportConsolCoversDailyTax>& arraySales );

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversDailyBlock,CEposReportCoversDailyBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolTranBusinessDate> m_arrayTranBusinessDate;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
};

/**********************************************************************/
