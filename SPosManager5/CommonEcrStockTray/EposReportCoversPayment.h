#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolCoversPayment.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCoversPayment : public CEposReport
{
public:
	CEposReportCoversPayment( CEposSelectArray& SelectArray );
	~CEposReportCoversPayment();

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	void CreateSalesSection( CEposReportCoversPaymentBlock& Block );

private:	
	void WriteBlockTotals( CEposReportCoversPaymentBlock& Block );

private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bAllowGlobal;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversPaymentBlock,CEposReportCoversPaymentBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/
