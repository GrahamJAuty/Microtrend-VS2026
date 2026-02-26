#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolAgeVerifyNoSale.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportNoSale : public CEposReport
{
public:
	CEposReportNoSale( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	CReportConsolidationArray<CEposReportConsolAgeVerifyNoSale> m_ConsolArray;
	CReportConsolidationArray<CSortedIntByString> m_arrayReasonLookup;
	CStringArray m_arrayReasonTexts;

};

/**********************************************************************/
