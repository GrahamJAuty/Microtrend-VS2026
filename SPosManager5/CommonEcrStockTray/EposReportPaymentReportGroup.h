#pragma once
/**********************************************************************/
#include "EposReportPaymentBase.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CEposReportPaymentReportGroup : public CEposReportPaymentBase
{
public:
	CEposReportPaymentReportGroup( CEposSelectArray& SelectArray );

public:
	virtual bool CreateGraph();
	virtual bool CheckGraph();
	virtual bool CreateReport();

	virtual bool WritePaymentLines( CReportConsolidationArray<CEposReportConsolPaySum>* pPayments, CEposReportConsolPaySum& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate );
	virtual bool WriteNetLines( CReportConsolidationArray<CEposReportConsolPaySumOneVal>* pPayments, CEposReportConsolPaySumOneVal& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate );

private:
	void GetTotalText( int nGroupNo, CString& strTotalText );

private:
	virtual void InitialiseConsolidationBlocks();
};

/**********************************************************************/
