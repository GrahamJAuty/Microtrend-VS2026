#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
#include "EposReportPluSalesInfo.h"
#include "TaxArray.h"
/**********************************************************************/

struct CEposReportConsolTaxSummaryPrice : public CEposReportConsolBase
{
public:
	CEposReportConsolTaxSummaryPrice();
	void Reset();

public:
	int Compare(CEposReportConsolTaxSummaryPrice& source, int nHint = 0);
	void Add(CEposReportConsolTaxSummaryPrice& source);

public:
	int m_nPriceLevel;
	int m_nTaxBand;
	int m_nTaxDateCode;

public:
	double m_dQty;
};

/**********************************************************************/

struct CEposReportConsolTaxSummaryPriceItem : public CEposReportConsolBase
{
public:
	CEposReportConsolTaxSummaryPriceItem();
	void Reset();

public:
	int Compare ( CEposReportConsolTaxSummaryPriceItem& source, int nHint = 0 );
	void Add	( CEposReportConsolTaxSummaryPriceItem& source );

public:
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nPriceLevel;
	int m_nTaxDateCode;
	
public:
	double m_dQty;
};

/**********************************************************************/

struct CEposReportTaxSummaryPriceBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportTaxSummaryPriceBlock() {
		m_pSales = NULL;
		m_pItem = NULL;
	}

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolTaxSummaryPrice>* m_pSales;
	CReportConsolidationArray<CEposReportConsolTaxSummaryPriceItem>* m_pItem;
	CEposReportPluSalesInfo m_infoCurrentPriceLevelTotals;
	CEposReportPluSalesInfo m_infoBlockTotals;
};

/**********************************************************************/
