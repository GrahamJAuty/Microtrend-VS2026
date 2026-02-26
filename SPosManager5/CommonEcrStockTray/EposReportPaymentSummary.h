#pragma once
/**********************************************************************/
#include "EposReportPaymentBase.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CEposReportPaymentSummary : public CEposReportPaymentBase
{
public:
	CEposReportPaymentSummary( CEposSelectArray& SelectArray );

public:
	virtual bool CreateGraph();
	virtual bool CheckGraph();
	virtual bool CreateReport();

private:
	virtual void InitialiseConsolidationBlocks();
};

/**********************************************************************/
