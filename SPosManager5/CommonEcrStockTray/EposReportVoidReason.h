#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolVoidReason.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportVoidReason : public CEposReport
{
public:
	CEposReportVoidReason( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void WriteTotalUnderline( int nType );
	void WriteDummyTotal( int nType );
	void WriteTotal( CEposReportConsolVoidReason& block );
	
private:
	CReportConsolidationArray<CEposReportConsolVoidReason> m_ConsolArray;
};

/**********************************************************************/
