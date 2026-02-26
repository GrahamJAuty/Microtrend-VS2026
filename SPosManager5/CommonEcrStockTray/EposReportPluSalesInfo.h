#pragma once
/**********************************************************************/
#include "PluCSVArray.h"
#include "SportsBookerPriceBandCSVArray.h"
/**********************************************************************/

struct CEposReportPluSalesInfo
{
public:
	CEposReportPluSalesInfo();
	
public:
	void Clear();
	void Consolidate( CEposReportPluSalesInfo& PluSalesInfo );
	
public:
	double m_dQty;
	double m_dTotalQty;
	double m_dValBeforeDiscount;
	double m_dDiscountAmount;
	double m_dPremiumAmount;
	double m_dTax;
	double m_dCost;
	bool m_bGotData;
};

/**********************************************************************/

struct CEposReportPluByPriceBandInfo
{
public:
	CEposReportPluByPriceBandInfo();

public:
	void Clear();
	void Consolidate(CEposReportPluByPriceBandInfo& PluSalesInfo);

public:
	double m_dPriceBandQty[MAX_PLU_PRICES];
	double m_dPriceBandVal[MAX_PLU_PRICES];
	double m_dQty;
	double m_dTotalQty;
	double m_dValBeforeDiscount;
	double m_dDiscountAmount;
	double m_dPremiumAmount;
	double m_dTax;
	double m_dCost;
	bool m_bGotData;
};

/**********************************************************************/

class CEposReportPluSalesInfoArray
{
public:
	CEposReportPluSalesInfoArray();
	~CEposReportPluSalesInfoArray();

public:
	void Clear();
	void Clear( int nLevel );
	void Consolidate( int nLevel, CEposReportPluSalesInfo& PluSalesInfo );
	void Consolidate( CEposReportPluSalesInfoArray& other );
	bool GetSalesInfo( int nLevel, CEposReportPluSalesInfo& PluSalesInfo );

public:
	CEposReportPluSalesInfo* m_arrayInfo[MAX_PLU_PRICES + MAX_SBPLU_PRICES + 1];
};

/**********************************************************************/

