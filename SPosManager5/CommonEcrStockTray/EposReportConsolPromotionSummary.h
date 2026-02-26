#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolPromotionSummary
{
public:
	CEposReportConsolPromotionSummary();
	void Reset();
	
public:
	int Compare ( CEposReportConsolPromotionSummary& source, int nHint = 0 );
	void Add	( CEposReportConsolPromotionSummary& source );
	
public:
	int m_nSection;
	int m_nMixMatchType;
	int m_nOfferNo;
	bool m_bOfferTotal;
	int m_nDiscType;
	int m_nDiscAction;
	
public:
	double m_dAmount;
};

/**********************************************************************/

struct CEposReportConsolPromotionSummaryTotal
{
public:
	CEposReportConsolPromotionSummaryTotal();
	void Reset();
	
public:
	int Compare ( CEposReportConsolPromotionSummaryTotal& source, int nHint = 0 );
	void Add	( CEposReportConsolPromotionSummaryTotal& source );

public:
	void Copy( CEposReportConsolPromotionSummary& source ); 
	
public:
	int m_nSection;
	int m_nMixMatchType;
	int m_nDiscType;
	int m_nDiscAction;
	
public:
	double m_dAmount;
};

/**********************************************************************/

struct CEposReportPromotionSummaryBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolPromotionSummary>* m_pPromos;
};

/**********************************************************************/
