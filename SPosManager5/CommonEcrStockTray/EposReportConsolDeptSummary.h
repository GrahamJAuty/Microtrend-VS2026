#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolDeptSummary : public CEposReportConsolBase
{
public:
	CEposReportConsolDeptSummary();
	void Reset();

public:
	int Compare(CEposReportConsolDeptSummary& source, int nHint = 0);
	void Add(CEposReportConsolDeptSummary& source);

public:
	int m_nDeptNo;
	int m_nTaxBand;
	int m_nTaxDateCode;

public:
	double m_dQty;
};

/**********************************************************************/

struct CEposReportConsolDeptSummaryTax : public CEposReportConsolBase
{
public:
	CEposReportConsolDeptSummaryTax();
	void Reset();

public:
	void Add(CEposReportConsolDeptSummary& source);

public:
	int m_nDeptNo;

public:
	double m_dQty;
	double m_dTaxAmount;
};

/**********************************************************************/

struct CEposReportDeptSummaryBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportDeptSummaryBlock();

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolDeptSummary>* m_pSales;
	CEposReportConsolDeptSummaryTax m_BlockTotal;
};

/**********************************************************************/
