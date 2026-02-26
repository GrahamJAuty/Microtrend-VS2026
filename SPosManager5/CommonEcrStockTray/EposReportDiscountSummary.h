#pragma once
/**********************************************************************/
#include "EposReportDiscountBase.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CEposReportDiscountSummary : public CEposReportDiscountBase
{
public:
	CEposReportDiscountSummary( CEposSelectArray& SelectArray, const char* szCustomSettings );

public:
	virtual bool CreateReport();

private:
	virtual void InitialiseConsolidationBlocks();
};

/**********************************************************************/
