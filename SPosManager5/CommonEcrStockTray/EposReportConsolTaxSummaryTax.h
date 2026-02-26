#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
#include "EposReportPluSalesInfo.h"
#include "TaxArray.h"
/**********************************************************************/

struct CEposReportConsolTaxSummaryTax : public CEposReportConsolBase
{
public:
	CEposReportConsolTaxSummaryTax();
	void Reset();

public:
	int Compare(CEposReportConsolTaxSummaryTax& source, int nHint = 0);
	void Add(CEposReportConsolTaxSummaryTax& source);

public:
	int m_nTaxBand;
	int m_nTaxDateCode;

public:
	double m_dQty;
};

/**********************************************************************/

struct CEposReportConsolTaxSummaryTaxItem : public CEposReportConsolBase
{
public:
	CEposReportConsolTaxSummaryTaxItem();
	void Reset();

public:
	int Compare(CEposReportConsolTaxSummaryTaxItem& source, int nHint = 0);
	void Add(CEposReportConsolTaxSummaryTaxItem& source);

public:
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nTaxDateCode;

public:
	double m_dQty;
};

/**********************************************************************/

struct CEposReportTaxSummaryTaxBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportTaxSummaryTaxBlock() {
		m_pSales = NULL;
		m_pItem = NULL;
	}

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolTaxSummaryTax>* m_pSales;
	CReportConsolidationArray<CEposReportConsolTaxSummaryTaxItem>* m_pItem;
	CEposReportPluSalesInfo m_infoCurrentTaxBandTotals;
	CEposReportPluSalesInfo m_infoBlockTotals;
};

/**********************************************************************/
