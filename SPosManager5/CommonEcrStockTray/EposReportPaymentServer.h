#pragma once
/**********************************************************************/
#include "EposReportConsolPaymentSummary.h"
#include "EposReportPaymentBase.h"
#include "EposSelectArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPaymentServer : public CEposReportPaymentBase
{
public:
	CEposReportPaymentServer( CEposSelectArray& SelectArray );

private:
	virtual void InitialiseConsolidationBlocks();
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo );

public:
	virtual bool CreateReport();

private:
	CReportConsolidationArray<CEposReportConsolServerBlockMap> m_ServerBlockMap;
};

/**********************************************************************/
