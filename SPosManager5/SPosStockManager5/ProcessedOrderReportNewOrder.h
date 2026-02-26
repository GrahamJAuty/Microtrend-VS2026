#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderBaseReport.h"
#include "ProcessedOrder.h"
/**********************************************************************/

class CProcessedOrderReportNewOrder : public CProcessedOrderBaseReport
{
public:
	CProcessedOrderReportNewOrder( CProcessedOrder& Order );

public:
	virtual bool CreateReport( int nDbIdx );

private:
	virtual void WriteOrder( CReportFile& ReportFile, int nDbIdx );
	
private:
	CProcessedOrder& m_Order;
};

/**********************************************************************/
#endif
/**********************************************************************/
