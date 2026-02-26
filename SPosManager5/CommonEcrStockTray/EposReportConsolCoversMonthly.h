#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolCoversMonthly
{
public:
	CEposReportConsolCoversMonthly();
	void Reset();
	
public:
	int Compare ( CEposReportConsolCoversMonthly& source, int nHint = 0 );
	void Add	( CEposReportConsolCoversMonthly& source );
	
public:
	int m_nYear;
	int m_nMonth;

public:
	int m_nQty;
};

/**********************************************************************/

struct CEposReportCoversMonthlyBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportCoversMonthlyBlock();

public:
	bool GotData();

public:
	int m_nCovers;
	CReportConsolidationArray<CEposReportConsolCoversMonthly>* m_pSales;
	CEposReportConsolCoversMonthly m_BlockTotal;
};

/**********************************************************************/
