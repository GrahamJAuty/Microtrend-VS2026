#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
#include "EposReportPluSalesInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolPluSalesTax : public CEposReportConsolBase
{
public:
	CEposReportConsolPluSalesTax();
	void Reset();

public:
	int Compare ( CEposReportConsolPluSalesTax& source, int nHint = 0 );
	void Add	( CEposReportConsolPluSalesTax& source );

public:
	bool ComparePluNoTax( CEposReportConsolPluSalesTax& source );
	bool CompareTaxBand	( CEposReportConsolPluSalesTax& source );
	
public:
	int m_nTaxBand;
	int m_nTaxDateCode;
	__int64 m_nPluNo;
	int m_nLocIdx;
	
public:
	double m_dQty;
	double m_dItemCost;
};

/**********************************************************************/

struct CEposReportPluSalesTaxBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportPluSalesTaxBlock();

public:
	bool GotData();

public:
	__int64 m_nCurrentPluNo;
	CReportConsolidationArray<CEposReportConsolPluSalesTax>* m_pSales;
	CReportConsolidationArray<CEposReportConsolPluSalesTax>* m_pItems;
	CEposReportPluSalesInfo m_infoCurrentPluTotals;
	CEposReportPluSalesInfo m_infoTaxBandTotals;
	CEposReportPluSalesInfo m_infoBlockTotals;
};

/**********************************************************************/

