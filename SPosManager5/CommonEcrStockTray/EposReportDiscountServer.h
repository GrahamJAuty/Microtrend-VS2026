#pragma once
/**********************************************************************/
#include "EposReportConsolPaymentSummary.h"
#include "EposReportDiscountBase.h"
/**********************************************************************/

class CEposReportDiscountServer : public CEposReportDiscountBase
{
public:
	CEposReportDiscountServer( CEposSelectArray& SelectArray );
	CEposReportDiscountServer( CEposSelectArray& SelectArray, const char* szCustomSettings );

private:
	virtual void InitialiseConsolidationBlocks();
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo );

public:
	virtual bool CreateReport();

private:
	CReportConsolidationArray<CEposReportConsolServerBlockMap> m_ServerBlockMap;
};

/**********************************************************************/
