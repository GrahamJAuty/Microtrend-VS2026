#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolAgeVerifyNoSale.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportAgeVerify : public CEposReport
{
public:
	CEposReportAgeVerify( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	CReportConsolidationArray<CEposReportConsolAgeVerifyNoSale> m_ConsolArray;
};

/**********************************************************************/
