#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
#include "EposReportPluSalesInfo.h"
#include "PluCSVArray.h"
#include "ReportConsolidationArray.h"
#include "SportsBookerPriceBandCSVArray.h"
/**********************************************************************/

struct CEposReportConsolPluSalesBase : public CEposReportConsolBase
{
public:
	CEposReportConsolPluSalesBase();
	void Reset();

public:
	int Compare(CEposReportConsolPluSalesBase& source, int nHint = 0);
	void Add(CEposReportConsolPluSalesBase& source);

public:
	bool ComparePluDeptTax(CEposReportConsolPluSalesBase& source);
	
public:
	__int64 m_nPluNo;
	bool m_bDummyItem;
	int m_nDeptNo;
	int m_nTaxBand;
	int m_nPriceBand;
	int m_nTaxDateCode;
	int m_nLocIdx;

public:
	double m_dQty;
	double m_dItemCost;
};

/**********************************************************************/

struct CEposReportPluSalesBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportPluSalesBlock();

public:
	bool GotData();

public:
	__int64 m_nCurrentPluNo;
	int m_nCurrentDeptNo;
	CReportConsolidationArray<CEposReportConsolPluSalesBase>* m_pSales;
	CReportConsolidationArray<CEposReportConsolPluSalesBase>* m_pItems;
	CEposReportPluSalesInfoArray m_arrayCurrentPluTotals;
	CEposReportPluSalesInfoArray m_arrayBlockTotals;
	int m_nCovers;
};

/**********************************************************************/

struct CEposReportConsolPluByPriceBandBase : public CEposReportConsolBase
{
public:
	CEposReportConsolPluByPriceBandBase();
	void Reset();

public:
	int Compare(CEposReportConsolPluByPriceBandBase& source, int nHint = 0);
	void Add(CEposReportConsolPluByPriceBandBase& source);

public:
	bool ComparePluDeptTax(CEposReportConsolPluByPriceBandBase& source);

public:
	__int64 m_nPluNo;
	bool m_bDummyItem;
	int m_nDeptNo;
	int m_nTaxBand;
	int m_nTaxDateCode;
	int m_nLocIdx;

public:
	double m_dPriceBandQty[10];
	double m_dPriceBandVal[10];
	double m_dTotalQty;
	double m_dItemCost;
};

/**********************************************************************/

struct CEposReportPluByPriceBandBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportPluByPriceBandBlock();

public:
	bool GotData();

public:
	__int64 m_nCurrentPluNo;
	int m_nCurrentDeptNo;
	CReportConsolidationArray<CEposReportConsolPluByPriceBandBase>* m_pSales;
	CReportConsolidationArray<CEposReportConsolPluByPriceBandBase>* m_pItems;
	CEposReportPluByPriceBandInfo m_CurrentPluTotals;
	CEposReportPluByPriceBandInfo m_BlockTotals;
	int m_nCovers;
};

/**********************************************************************/

struct CEposReportVIPSalesBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();

public:
	__int64 m_nCurrentPluNo;
	int m_nCurrentDeptNo;
	CReportConsolidationArray<CEposReportConsolPluSalesBase>* m_pSales;
	CReportConsolidationArray<CEposReportConsolPluSalesBase>* m_pItems;
	CEposReportPluSalesInfo m_infoCurrentPluTotals;
	CEposReportPluSalesInfo m_infoBlockTotals;
};

/**********************************************************************/

struct CEposReportConsolPluSalesCustomerID : public CEposReportConsolBase
{
public:
	CEposReportConsolPluSalesCustomerID();
	void Reset();

public:
	int Compare ( CEposReportConsolPluSalesCustomerID& source, int nHint = 0 );
	void Add	( CEposReportConsolPluSalesCustomerID& source );

public:
	int m_nLocIdx;
	int m_nSortType;	//0 = CUSTOMER, 1 = NON CUSTOMER
	int m_nCustomerID;
	__int64 m_nPluNo;
	int m_nDeptNo;
	int m_nTaxBand;
	int m_nPriceBand;
	int m_nTaxDateCode;

public:
	double m_dQty;
};

/**********************************************************************/

struct CEposReportConsolPluSalesCustomerSortIdx : public CEposReportConsolBase
{
public:
	CEposReportConsolPluSalesCustomerSortIdx();
	void Reset();

public:
	int Compare ( CEposReportConsolPluSalesCustomerSortIdx& source, int nHint = 0 );
	void Add	( CEposReportConsolPluSalesCustomerSortIdx& source );

public:
	bool CompareLocation ( CEposReportConsolPluSalesCustomerSortIdx& source );
	bool CompareCustomer ( CEposReportConsolPluSalesCustomerSortIdx& source );
	bool ComparePluDeptTax ( CEposReportConsolPluSalesCustomerSortIdx& source );

public:
	int m_nLocSortIdx;
	int m_nSortIdxType;
	int m_nSortIdxNum;
	int m_nCustomerID;
	__int64 m_nPluNo;
	int m_nDeptNo;
	int m_nTaxBand;
	int m_nPriceBand;
	int m_nTaxDateCode;
	int m_nLocIdx;
	
public:
	double m_dQty;
	double m_dItemCost;
};

/**********************************************************************/
