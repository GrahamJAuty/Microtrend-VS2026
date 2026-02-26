#pragma once
/**********************************************************************/
#include "EposReportPluVipSalesBase.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CEposReportPluVipSalesSummary : public CEposReportPluVipSalesBase
{
public:
	CEposReportPluVipSalesSummary( CEposSelectArray& SelectArray, int nReportType );

public:
	virtual bool CreateReport();

private:
	virtual void InitialiseConsolidationBlocks();
};

/**********************************************************************/
