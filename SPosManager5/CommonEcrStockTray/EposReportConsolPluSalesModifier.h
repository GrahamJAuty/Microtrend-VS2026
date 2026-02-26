#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
#include "EposReportPluSalesInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolPluSalesModifier : CEposReportConsolBase
{
public:
	CEposReportConsolPluSalesModifier();
	void Reset();

public:
	int Compare(CEposReportConsolPluSalesModifier& source, int nHint = 0);
	void Add(CEposReportConsolPluSalesModifier& source);

public:
	bool ComparePlu(CEposReportConsolPluSalesModifier& source);

public:
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nTaxBand;
	int m_nTaxDateCode;
	int m_nPriceBand;
	int m_nLocIdx;

public:
	double m_dQty;
	double m_dItemCost;
};

/**********************************************************************/

struct CEposReportPluSalesModifierBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportPluSalesModifierBlock();

public:
	bool GotData();

public:
	__int64 m_nCurrentPluNo;
	CReportConsolidationArray<CEposReportConsolPluSalesModifier>* m_pSales;
	CReportConsolidationArray<CEposReportConsolPluSalesModifier>* m_pItems;
	CEposReportPluSalesInfo m_infoCurrentPluTotals[10][7];
	CEposReportPluSalesInfo m_infoCurrentPluGrandTotal;
	CEposReportPluSalesInfo m_infoBlockTotals;
};

/**********************************************************************/

