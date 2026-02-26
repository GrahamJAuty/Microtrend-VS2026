#pragma once
/**********************************************************************/
#include "EposReportConsolCoversTimedGroup.h"
#include "EposReportConsolTran.h"
/**********************************************************************/
#include "EposReportBase.h"
/**********************************************************************/

class CEposReportCoversTimedGroup : public CEposReport
{
public:
	CEposReportCoversTimedGroup( CEposSelectArray& SelectArray );
	~CEposReportCoversTimedGroup();

	virtual void Consolidate();
	virtual bool CreateReport();

protected:
	virtual bool GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel ) = 0;
	virtual const char* GetDateOrTimeReport( CString& strLabel ) = 0;
	
protected:
	void CreateSalesSection( CEposReportCoversTimedGroupBlock& Block );

protected:	
	void WriteBlockTotals( CEposReportCoversTimedGroupBlock& Block, bool bAfterDiscount, double dAdjustSalesPremium, double dAdjustSalesDiscount, double dAdjustTaxPremium, double dAdjustTaxDiscount );
	void WriteTimeCodeTotals( CEposReportConsolCoversTimedGroupTax& TimeSlotTotals );

protected:
	void InitialiseConsolidationBlocks();
	void PostConsolidateSalesTax( CEposReportCoversTimedGroupBlock& Block, CArray<CEposReportConsolCoversTimedGroupTax,CEposReportConsolCoversTimedGroupTax>& arraySales );
	void PostConsolidateAdjustTax( CEposReportCoversTimedGroupBlock& Block, double& dAdjustSalesPremium, double& dAdjustSalesDiscount, double& dAdjustTaxPremium, double& dAdjustTaxDiscount  );

protected:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportCoversTimedGroupBlock,CEposReportCoversTimedGroupBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolTranDateOrTime> m_arrayTranDateOrTime;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;

protected:
	CString m_strDateOrTimeColumn;
	CString m_strDateOrTimeReport;
	bool m_bIsCoversReport;
	bool m_bDeptMode;
};

/**********************************************************************/
