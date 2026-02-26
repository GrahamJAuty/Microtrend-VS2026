#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "EposReportBase.h"
#include "EposReportConsolCoversDataPoint.h"
#include "EposReportConsolTran.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCoversGroup : public CEposReport
{
public:
	CEposReportCoversGroup( CEposSelectArray& SelectArray, bool bDeptMode );
	~CEposReportCoversGroup();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void CreateSalesSection( CEposReportCoversDataPointBlock& Block );
	
private:	
	void WriteBlockTotals( CEposReportCoversDataPointBlock& Block, bool bAfterDiscount, double dAdjustSalesPremium, double dAdjustSalesDiscount, double dAdjustTaxPremium, double dAdjustTaxDiscount);

private:
	void InitialiseConsolidationBlocks();
	void PostConsolidateSalesTax( CEposReportCoversDataPointBlock& Block, CArray<CEposReportConsolCoversDataPointTax,CEposReportConsolCoversDataPointTax>& arraySales );
	void PostConsolidateAdjustTax( CEposReportCoversDataPointBlock& Block, double& dAdjustSalesPremium, double& dAdjustSalesDiscount, double& dAdjustTaxPremium, double& dAdjustTaxDiscount  );

private:
	CReportConsolidationArray<CConsolidatedDoubleByInt> m_ConsolBufferGroup;
	CReportConsolidationArray<CConsolidatedDoubleByInt64> m_ConsolBufferPluDiscount;
	CReportConsolidationArray<CSortedIntByInt64> m_ConsolBufferPluGroup;
	
private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversDataPointBlock,CEposReportCoversDataPointBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;	
	CReportConsolidationArray<CEposReportConsolTran> m_arrayTran;
	bool m_bDeptMode;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
};

/**********************************************************************/
