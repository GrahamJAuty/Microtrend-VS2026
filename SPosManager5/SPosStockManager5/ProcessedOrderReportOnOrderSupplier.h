#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderBaseReport.h"
#include "ProcessedOrderArray.h"
/**********************************************************************/

class CProcessedOrderReportOnOrderSupplier : public CProcessedOrderBaseReport
{
public:
	CProcessedOrderReportOnOrderSupplier( CWordArray& wSuppSelectArray, CProcessedOrderArray& OrderArray );

public:
	void SetCheckAllOrderFoldersFlag( bool b ){ m_bCheckAllOrderFolders = b; }
	void SetCategoryFilter( int n ){ m_nCategoryFilter = n; }

public:
	virtual bool CreateReport( int nDbIdx );

private:
	virtual void WriteOrders( CReportFile& ReportFile, int nDbIdx, int nSingleSpIdx );
	void WriteOrderTotal ( CReportFile& ReportFile, double dTotal );
	
private:
	CWordArray m_wSuppSelectArray;

private:
	bool m_bCheckAllOrderFolders;
	int m_nCategoryFilter;
	
private:
	CProcessedOrderArray& m_OrderArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
