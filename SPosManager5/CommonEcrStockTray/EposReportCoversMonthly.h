#pragma once
/**********************************************************************/
#include "BusinessDateInfo.h"
#include "EposReportBase.h"
#include "EposReportConsolCoversMonthly.h"
#include "ReportConsolidationArray.h"
#include "SalesHistoryDateInfo.h"
/**********************************************************************/

class CEposReportCoversMonthly : public CEposReport
{
public:
	CEposReportCoversMonthly( CEposSelectArray& SelectArray );
	~CEposReportCoversMonthly();

	virtual void Consolidate();
	virtual bool CreateReport();
	virtual bool CreateGraph();
	virtual bool CheckGraph();

private:
	void CreateSalesSection( CEposReportCoversMonthlyBlock& Block );
	
private:	
	void WriteBlockTotals( CEposReportCoversMonthlyBlock& Block );

private:
	void InitialiseConsolidationBlocks();
	void GetYearAndMonth( int nLocIdx, CString& strDateToCheck, CString& strTimeToCheck, int& nYear, int& nMonth );

private:
	CBusinessDateInfo m_BusinessDateInfo;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversMonthlyBlock,CEposReportCoversMonthlyBlock> m_arrayReportBlocks;
	CReportConsolidationArray<CEposReportConsolTranMonth> m_arrayTranMonth;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/

