#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolCoversPayment
{
public:
	CEposReportConsolCoversPayment();
	void Reset();

public:
	int Compare ( CEposReportConsolCoversPayment& source, int nHint = 0 );
	void Add	( CEposReportConsolCoversPayment& source );
	
public:
	int m_nPaymentNo;
	
public:
	int m_nCovers;
	int m_nAmount;
};

/**********************************************************************/

struct CEposReportCoversPaymentBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportCoversPaymentBlock();

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolCoversPayment>* m_pSales;
	CEposReportConsolCoversPayment m_BlockTotal;
};

/**********************************************************************/
