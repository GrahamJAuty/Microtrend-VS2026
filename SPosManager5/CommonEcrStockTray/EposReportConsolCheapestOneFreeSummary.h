#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolCheapestOneFreeSummary
{
public:
	CEposReportConsolCheapestOneFreeSummary();
	void Reset();
	
public:
	int Compare ( CEposReportConsolCheapestOneFreeSummary& source, int nHint = 0 );
	void Add	( CEposReportConsolCheapestOneFreeSummary& source );
	
public:
	int m_nMixMatchNo;
	int m_nSubtotalFlag;
	__int64 m_nPluNo;
	
public:
	int m_nQty;
	double m_dVal;
};

/**********************************************************************/

struct CEposReportCheapestOneFreeSummaryBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolCheapestOneFreeSummary>* m_pPromos;
	CReportConsolidationArray<CEposReportConsolCheapestOneFreeSummary>* m_pTotals;
};

/**********************************************************************/
