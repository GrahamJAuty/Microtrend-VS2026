#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolServer : public CEposReportConsolBase
{
public:
	CEposReportConsolServer();
	void Reset();

public:
	int Compare(CEposReportConsolServer& source, int nHint = 0);
	void Add(CEposReportConsolServer& source);

public:
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	int m_nTaxBand;
	int m_nTaxDateCode;

public:
	int m_nTranCount;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

struct CEposReportConsolServerTax : public CEposReportConsolBase
{
public:
	CEposReportConsolServerTax();
	void Reset();

public:
	void Add(CEposReportConsolServer& source);

public:
	int m_nServerLocOrDbIdx;
	int m_nServerNo;

public:
	int m_nTranCount;
	double m_dTaxAmount;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

struct CEposReportServerBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportServerBlock() {
		m_pSales = NULL;
	}

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolServer>* m_pSales;
	CEposReportConsolServerTax m_BlockTotal;
};

/**********************************************************************/

