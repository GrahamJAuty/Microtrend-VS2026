#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolCoversDataPoint.h"
#include "EposReportConsolTran.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCoversPriceLevel : public CEposReport
{
public:
	CEposReportCoversPriceLevel( CEposSelectArray& SelectArray );
	~CEposReportCoversPriceLevel();

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	void CreateSalesSection( CEposReportCoversDataPointBlock& Block );

private:	
	void WriteBlockTotals( CEposReportCoversDataPointBlock& Block, bool bAfterDiscount, double dAdjustSalesPremium, double dAdjustSalesDiscount, double dAdjustTaxPremium, double dAdjustTaxDiscount );

private:
	void InitialiseConsolidationBlocks();
	void PostConsolidateSalesTax( CEposReportCoversDataPointBlock& Block, CArray<CEposReportConsolCoversDataPointTax,CEposReportConsolCoversDataPointTax>& arraySales );
	void PostConsolidateAdjustTax( CEposReportCoversDataPointBlock& Block, double& dAdjustSalesPremium, double& dAdjustSalesDiscount, double& dAdjustTaxPremium, double& dAdjustTaxDiscount  );

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversDataPointBlock,CEposReportCoversDataPointBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolTranPriceLevel> m_arrayTranPriceLevel;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
};

/**********************************************************************/
