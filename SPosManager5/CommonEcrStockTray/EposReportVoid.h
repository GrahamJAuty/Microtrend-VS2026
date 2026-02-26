#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolRefundVoid.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportVoidTotals
{
public:
	CEposReportVoidTotals();

public:
	void Reset();
	void Consolidate( double dModQty, double dPluQty, double dValue );

public:
	bool m_bGotTotal;
	double m_dModQty;
	double m_dPluQty;
	double m_dValue;
};

/**********************************************************************/

class CEposReportVoid : public CEposReport
{
public:
	CEposReportVoid( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void WriteServerTotals( int nBlock, CEposReportVoidTotals& Sales, CEposReportVoidTotals& Refunds, CEposReportVoidTotals& Discounts );
	void WriteVoidTotals( const char* szType, CEposReportVoidTotals& Totals, bool bDiscount, bool bUnderline );
	
private:
	CReportConsolidationArray<CEposReportConsolRefundVoid> m_ConsolArray;
};

/**********************************************************************/
