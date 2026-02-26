#pragma once
/**********************************************************************/
#include <math.h>
/**********************************************************************/
#define DISCOUNT_MODE_NONE 0
#define DISCOUNT_MODE_ITEM 1
#define DISCOUNT_MODE_SUBTOTAL 2
#define DISCOUNT_MODE_LOYALTY 3
#define DISCOUNT_MODE_DIFFER 4
#define DISCOUNT_MODE_MIXMATCH 5
/**********************************************************************/

struct CCashRSPImporterDiscountShareItem
{
public:
	 CCashRSPImporterDiscountShareItem();

public:
	int m_nIndex;
	double m_dSales;
	double m_dDiscount;
	bool m_bAllowDiscount;
};

/**********************************************************************/

class CCashRSPImporterDiscountShareList
{
public:
	CCashRSPImporterDiscountShareList();
	void Reset();

public:
	void AddItem( int nIndex, double dSales, bool bAllowDiscount );
	void FilterList();
	void ShareDiscount( double dDiscountAmount, bool bForceFractions = FALSE );

public:
	bool GotDiscountableSales();

public:
	int GetItemCount(){ return m_arrayItems.GetSize(); }
	int GetIndex( int n ){ return m_arrayItems[n].m_nIndex; }
	double GetDiscountShare( int n ){ return m_arrayItems[n].m_dDiscount; } 

private:
	CArray<CCashRSPImporterDiscountShareItem,CCashRSPImporterDiscountShareItem> m_arrayItems;
	double m_dTotalSales;
	double m_dDiscountableSales;
};

/**********************************************************************/

struct CCashRSPImporterDiscountHelperSales
{
public:
	CCashRSPImporterDiscountHelperSales();
	void Reset();

public:
	int Compare ( CCashRSPImporterDiscountHelperSales& source, int nHint = 0 );
	void Add	( CCashRSPImporterDiscountHelperSales& source );
	
public:
	__int64 m_nMMPluNo;
	__int64 m_nPluNo;
	int m_nAnalysisCategory;
	int m_nItemServerNo;
	int m_nPriceBand;
	int m_nMixMatchType;
	int m_nMixMatchNo;
	int m_nVIPStatus;
	CString m_strTaxBand;
	CString m_strSaleDate;
	CString m_strSaleTime;
	CString m_strDescription;
	CString m_strAttributeText;

public:
	int m_nDeptNo;
	int m_nGroupNo;
	
public:
	double m_dQty;
	double m_dVal;
};

/**********************************************************************/

struct CCashRSPImporterDiscountHelperPendingItem
{
public:
	CCashRSPImporterDiscountHelperPendingItem();
	void Reset();

public:
	int Compare ( CCashRSPImporterDiscountHelperPendingItem& source, int nHint = 0 );
	void Add	( CCashRSPImporterDiscountHelperPendingItem& source );
	
public:
	int m_nDiscountServerNo;
	__int64 m_nPluNo;
	int m_nAnalysisCategory;
	int m_nPromoNo;
	CString m_strDiscountType;
	int m_nDiscountRate;
	int m_nReasonCode;
	CString m_strReasonText;
	
public:
	double m_dAmountAsDouble;
	int m_nAmountAsInt;
};

/**********************************************************************/

struct CCashRSPImporterDiscountHelperPendingSubtotal
{
public:
	CCashRSPImporterDiscountHelperPendingSubtotal();
	void Reset();

public:
	int Compare ( CCashRSPImporterDiscountHelperPendingSubtotal& source, int nHint = 0 );
	void Add	( CCashRSPImporterDiscountHelperPendingSubtotal& source );
	
public:
	CString m_strCategories;
	int m_nPromoNo;
	int m_nDiscountMode;
	CString m_strDiscountType;
	int m_nDiscountRate;
	bool m_bVATable;

public:
	double m_dVal;
};

/**********************************************************************/

struct CCashRSPImporterDiscountHelperToWrite
{
public:
	CCashRSPImporterDiscountHelperToWrite();
	void Reset();

public:
	int Compare ( CCashRSPImporterDiscountHelperToWrite& source, int nHint = 0 );
	void Add	( CCashRSPImporterDiscountHelperToWrite& source );
	
public:
	int m_nSaleIdx;
	__int64 m_nPluNo;
	CString m_strItemDescription;
	CString m_strAttributeText;
	int m_nAnalysisCategory;
	int m_nPromoNo;
	int m_nDiscountMode;
	CString m_strDiscountType;
	int m_nDiscountRate;
	int m_nVIPStatus;
	bool m_bVATable;
	
public:
	double m_dDiscountedItemQty;
	double m_dVal;
};

/**********************************************************************/

struct CCashRSPMixMatchTotals
{
public:
	CCashRSPMixMatchTotals();
	void Reset();

public:
	int Compare(CCashRSPMixMatchTotals& source, int nHint = 0);
	void Add(CCashRSPMixMatchTotals& source);

public:
	int GetAmountAsInt() { return m_nAmountAsInt; }
	double GetAmountAsDouble() { return m_dAmountAsDouble; }
	void SetAmountAsDouble(double d);
	void SetAmountAsInt(int n);

public:
	int m_nPromoNo;
	int m_nOfferNo;
	__int64 m_nPluNo;

private:
	double m_dAmountAsDouble;
	int m_nAmountAsInt;
	bool m_bConsolidateDouble;
};

/**********************************************************************/

struct CCashRSPMixMatchItems
{
public:
	CCashRSPMixMatchItems();
	void Reset();

public:
	int Compare(CCashRSPMixMatchItems& source, int nHint = 0);
	void Add(CCashRSPMixMatchItems& source);

public:
	int m_nPromoNo;
	int m_nOfferNo;
	__int64 m_nPluNo;
	CString m_strTaxBand;
	int m_nOfferType;
	int m_nDiscountRate;
	double m_dAmount;
};

/**********************************************************************/

struct CCashRSPPromoTotals
{
public:
	CCashRSPPromoTotals();
	void Reset();

public:
	int Compare(CCashRSPPromoTotals& source, int nHint = 0);
	void Add(CCashRSPPromoTotals& source);

public:
	int m_nPromoNo;
	int m_nPromoTotal;
	int m_nPromoMMTotal;
	bool m_bCanUsePromoRules;
};

/**********************************************************************/

struct CCashRSPPromoItems
{
public:
	CCashRSPPromoItems();
	void Reset();

public:
	int Compare(CCashRSPPromoItems& source, int nHint = 0);
	void Add(CCashRSPPromoItems& source);

public:
	int m_nPromoNo;
	__int64 m_nPluNo;
	CString m_strDiscountMode;
	CString m_strDiscountType;
	int m_nDiscountRate;
	bool m_bMicrotrendTaxFlag;
	double m_dAmount;
};

/**********************************************************************/

