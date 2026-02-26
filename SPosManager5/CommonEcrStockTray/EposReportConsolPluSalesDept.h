#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
#include "EposReportPluSalesInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolPluSalesDept : public CEposReportConsolBase
{
public:
	CEposReportConsolPluSalesDept();
	void Reset();

public:
	int Compare(CEposReportConsolPluSalesDept& source, int nHint = 0);
	void Add(CEposReportConsolPluSalesDept& source);

public:
	bool ComparePluNoTax(CEposReportConsolPluSalesDept& source);
	bool CompareDeptNo(CEposReportConsolPluSalesDept& source);

public:
	int m_nDeptNo;
	__int64 m_nPluNo;
	int m_nTaxBand;
	int m_nTaxDateCode;
	int m_nLocIdx;

public:
	double m_dQty;
	double m_dItemCost;
};

/**********************************************************************/

struct CEposReportPluSalesDeptBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportPluSalesDeptBlock();

public:
	bool GotData();

public:
	__int64 m_nCurrentPluNo;
	CReportConsolidationArray<CEposReportConsolPluSalesDept>* m_pSales;
	CReportConsolidationArray<CEposReportConsolPluSalesDept>* m_pItems;
	CEposReportPluSalesInfo m_infoCurrentPluTotals;
	CEposReportPluSalesInfo m_infoDeptTotals;
	CEposReportPluSalesInfo m_infoBlockTotals;
};

/**********************************************************************/

