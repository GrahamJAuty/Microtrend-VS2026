#pragma once
/**********************************************************************/
#include "EposReportConsolPaymentSummary.h"
#include "EposReportPluVipSalesBase.h"
#include "EposSelectArray.h"
#include "PluCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPluVipSalesServer : public CEposReportPluVipSalesBase
{
public:
	CEposReportPluVipSalesServer( CEposSelectArray& SelectArray, int nInitialReportType );

private:
	virtual void InitialiseConsolidationBlocks();
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo );

public:
	virtual bool CreateReport();

private:
	CReportConsolidationArray<CEposReportConsolServerBlockMap> m_ServerBlockMap;
	CEposReportPluSalesInfoArray m_arrayTotals2;
};

/**********************************************************************/
