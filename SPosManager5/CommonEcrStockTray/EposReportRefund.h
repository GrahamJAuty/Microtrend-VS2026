#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolRefundVoid.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportRefund : public CEposReport
{
public:
	CEposReportRefund( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void WriteTotals( const char* szType, double dModQty, double dPluQty, double dValue );
	
private:
	CReportConsolidationArray<CEposReportConsolRefundVoid> m_ConsolArray;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/

