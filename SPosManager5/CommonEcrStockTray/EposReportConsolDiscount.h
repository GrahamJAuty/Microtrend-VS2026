#pragma once
/**********************************************************************/
#define DISCOUNT_TYPE_MARKDOWN 0
#define DISCOUNT_TYPE_DISCOUNT 1
#define DISCOUNT_TYPE_PREMIUM 2
#define DISCOUNT_TYPE_UNKNOWN 3
#define DISCOUNT_TYPE_PMS 4
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolDiscount
{
public:
	CEposReportConsolDiscount();
	void Reset();
	
public:
	int Compare ( CEposReportConsolDiscount& source, int nHint = 0 );
	void Add	( CEposReportConsolDiscount& source );

public:
	bool CompareServer( CEposReportConsolDiscount& source );
	
public:
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	int m_nLineType;
	int m_nDbIdx;
	CString m_strDate;
	CString m_strTime;
	int m_nDiscountType;
	int m_nDiscountMode;
	int m_nPromoNo;
	__int64 m_nPluNo;
	int m_nLineNo;
	
public:
	CString m_strText;
	CString m_strReason;
	int m_nAmount;
	int m_nRate;
};

/**********************************************************************/

struct CEposReportConsolDiscountNew
{
public:
	CEposReportConsolDiscountNew();
	void Reset();
	
public:
	int Compare ( CEposReportConsolDiscountNew& source, int nHint = 0 );
	void Add	( CEposReportConsolDiscountNew& source );

public:
	bool CompareServer( CEposReportConsolDiscountNew& source );
	
public:
	CString m_strDate;
	CString m_strTime;
	int m_nMixMatchType;
	int m_nDiscountType;
	int m_nDiscountMode;
	int m_nOfferNo;
	__int64 m_nPluNo;
	int m_nLineNo;
	
public:
	CString m_strText;
	int m_nRate;
	double m_dValue;
};

/**********************************************************************/

struct CEposReportDiscountBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolDiscountNew>* m_pDiscount;
	CReportConsolidationArray<CEposReportConsolDiscountNew>* m_pTotals;
};

/**********************************************************************/