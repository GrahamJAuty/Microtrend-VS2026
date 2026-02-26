#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolCoversHourly.h"
#include "EposReportConsolTran.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCoversHourly : public CEposReport
{
public:
	CEposReportCoversHourly( CEposSelectArray& SelectArray );
	CEposReportCoversHourly( CEposSelectArray& SelectArray, const char* szCustomSettings );
	~CEposReportCoversHourly();

	virtual void Consolidate();
	virtual bool CreateReport();
	virtual bool CreateGraph();
	virtual bool CheckGraph();

private:
	void CreateSalesSection( CEposReportCoversHourlyBlock& Block );

private:	
	void WriteBlockTotals( CEposReportCoversHourlyBlock& Block );

private:
	void InitialiseConsolidationBlocks();
	void PostConsolidateSalesTax( CEposReportCoversHourlyBlock& Block, CArray<CEposReportConsolCoversHourlyTax,CEposReportConsolCoversHourlyTax>& arraySales );

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversHourlyBlock,CEposReportCoversHourlyBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolTranTimeCode> m_arrayTranTimeCode;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;

private:
	CTimeMap m_TimeMap;
	int m_nTimeSliceType;

private:
	CEposReportCustomSettingsHourly m_ReportSettings;
};

/**********************************************************************/
