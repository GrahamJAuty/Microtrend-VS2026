#pragma once
/**********************************************************************/
#include "EposReportConsolEODStatus.h"
/**********************************************************************/
#include "EposReportBase.h"
/**********************************************************************/

class CEposReportEODStatus : public CEposReport
{
public:
	CEposReportEODStatus( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	CReportConsolidationArray<CEposReportConsolEODStatus> m_arrayEODStatus;
	bool m_bGuestAccountWarn;
};

/**********************************************************************/
