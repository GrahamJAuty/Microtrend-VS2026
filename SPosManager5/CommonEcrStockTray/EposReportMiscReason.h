#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolMiscReason.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportMiscReason : public CEposReport
{
public:
	CEposReportMiscReason( int nReportType, CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void WriteTotalUnderline();
	void WriteTotal( CEposReportConsolMiscReason& block );
	
private:
	CReportConsolidationArray<CEposReportConsolMiscReason> m_ConsolArray;
	
private:
	int m_nReasonType;
	CString m_strEventDate;
	CString m_strEventTime;

private:
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
