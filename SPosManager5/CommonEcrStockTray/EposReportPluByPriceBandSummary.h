#pragma once
/**********************************************************************/
#include "EposReportPluByPriceBandBase.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CEposReportPluByPriceBandSummary : public CEposReportPluByPriceBandBase
{
public:
	CEposReportPluByPriceBandSummary( CEposSelectArray& SelectArray, CString strCustomSettings);

public:
	virtual bool CreateReport();

private:
	virtual void InitialiseConsolidationBlocks();
};

/**********************************************************************/
