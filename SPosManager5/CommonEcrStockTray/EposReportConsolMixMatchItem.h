#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/
#define MMITEMCONSOL_OFFER_HEADER 0
#define MMITEMCONSOL_OFFER_ITEM 1
#define MMITEMCONSOL_OFFER_TOTAL 2
/**********************************************************************/

struct CEposReportConsolMMTranByDateTime
{
public:
	CEposReportConsolMMTranByDateTime();
	void Reset();

public:
	int Compare ( CEposReportConsolMMTranByDateTime& source, int nHint = 0 );
	void Add	( CEposReportConsolMMTranByDateTime& source );
	
public:
	CString m_strTranDate;
	CString m_strTranTime;
	
public:
	int m_nTranID;
};

/**********************************************************************/

struct CEposReportConsolMMCoversByTran
{
public:
	CEposReportConsolMMCoversByTran();
	void Reset();

public:
	int Compare ( CEposReportConsolMMCoversByTran& source, int nHint = 0 );
	void Add	( CEposReportConsolMMCoversByTran& source );
	
public:
	int m_nTranID;
	int m_nCovers;
};

/**********************************************************************/

struct CEposReportConsolMMCoversByOffer
{
public:
	CEposReportConsolMMCoversByOffer();
	void Reset();

public:
	int Compare ( CEposReportConsolMMCoversByOffer& source, int nHint = 0 );
	void Add	( CEposReportConsolMMCoversByOffer& source );
	
public:
	int m_nMixMatchType;
	int m_nOfferNo;
	int m_nTranID;
	int m_nCovers;
};

/**********************************************************************/

struct CEposReportConsolMMTranByOfferItem
{
public:
	CEposReportConsolMMTranByOfferItem();
	void Reset();

public:
	int Compare ( CEposReportConsolMMTranByOfferItem& source, int nHint = 0 );
	void Add	( CEposReportConsolMMTranByOfferItem& source );
	
public:
	int m_nOfferItemID;
	int m_nTranID;
};

/**********************************************************************/

struct CEposReportConsolMMDiscountedItem
{
public:
	CEposReportConsolMMDiscountedItem();
	void Reset();

public:
	int Compare ( CEposReportConsolMMDiscountedItem& source, int nHint = 0 );
	void Add	( CEposReportConsolMMDiscountedItem& source );

public:
	bool m_bBlockTotal;
	int m_nMixMatchType;
	int m_nOfferNo;
	int m_nOfferLineType;
	__int64 m_nItemNo;

public:
	int m_nOfferCovers;
	
public:
	int m_nOfferItemID;
	int m_nTranQty;
	double m_dItemQty;
	double m_dVal;
};

/**********************************************************************/

struct CEposReportConsolMMUsageCount
{
public:
	CEposReportConsolMMUsageCount();
	void Reset();

public:
	int Compare(CEposReportConsolMMUsageCount& source, int nHint = 0);
	void Add(CEposReportConsolMMUsageCount& source);

public:
	int m_nOfferNo;
	int m_nOfferType;
	CString m_strUniqueID;
	int m_nCount;
};

/**********************************************************************/

struct CEposReportConsolMMUniqueID
{
public:
	CEposReportConsolMMUniqueID();
	void Reset();

public:
	int Compare(CEposReportConsolMMUniqueID& source, int nHint = 0);
	void Add(CEposReportConsolMMUniqueID& source);

public:
	int m_nLocIdx;
	CString m_strUniqueID;
};

/**********************************************************************/

struct CEposReportMixMatchItemBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolMMDiscountedItem>* m_pSales;
	CReportConsolidationArray<CEposReportConsolMMUsageCount>* m_pMMUsageCount;
	CReportConsolidationArray<CEposReportConsolMMUniqueID>* m_pMMUniqueID;
};

/**********************************************************************/
