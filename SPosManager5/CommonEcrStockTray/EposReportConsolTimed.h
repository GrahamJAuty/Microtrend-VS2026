#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolTimed : CEposReportConsolBase
{
public:
	CEposReportConsolTimed();
	void Reset();

public:
	int Compare(CEposReportConsolTimed& source, int nHint = 0);
	void Add(CEposReportConsolTimed& source);

public:
	CString m_strDateOrTime;
	int m_nTaxDateCode;
	int m_nTaxBand;

public:
	int m_nTranCount;
};

/**********************************************************************/

struct CEposReportConsolTimedTax : CEposReportConsolBase
{
public:
	CEposReportConsolTimedTax();
	void Reset();

public:
	void Add(CEposReportConsolTimed& source);

public:
	CString m_strDateOrTime;

public:
	int m_nTranCount;
	double m_dTaxAmount;
};

/**********************************************************************/

struct CEposReportTimedBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportTimedBlock() {
		m_pSales = NULL;
		m_nTranCount = 0;
	}

public:
	bool GotData();

public:
	int m_nTranCount;
	CReportConsolidationArray<CEposReportConsolTimed>* m_pSales;
	CEposReportConsolTimedTax m_BlockTotal;
};

/**********************************************************************/
