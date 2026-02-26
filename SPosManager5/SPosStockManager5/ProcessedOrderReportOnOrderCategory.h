#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderArray.h"
#include "ProcessedOrderBaseReport.h"
#include "ProcessedOrderEntity.h"
/**********************************************************************/

struct CReportConsolOnOrderCategory
{
public:
	CReportConsolOnOrderCategory();

public:
	void Reset();
	int Compare ( CReportConsolOnOrderCategory& source, int nHint = 0 );
	void Add	( CReportConsolOnOrderCategory& source );
	
public:
	int m_nCategory;
	int m_nOrderLine;

public:
	CString m_strData;
};

/**********************************************************************/

class CProcessedOrderReportOnOrderCategory : public CProcessedOrderBaseReport
{
public:
	CProcessedOrderReportOnOrderCategory( CWordArray& wSuppSelectArray );

public:
	void SetCategoryFilter( int n ){ m_nCategoryFilter = n; }

public:
	virtual bool CreateReport( int nDbIdx );

private:
	virtual void WriteOrders( CReportFile& ReportFile, int nDbIdx, int nSingleSpIdx );
	void WriteOrderTotal ( CReportFile& ReportFile, double dTotal );
	
private:
	CWordArray m_wSuppSelectArray;
	CProcessedOrderEntity m_HeaderEntity;

private:
	int m_nCategoryFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/
