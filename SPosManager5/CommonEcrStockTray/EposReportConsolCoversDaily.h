#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolCoversDaily : public CEposReportConsolBase
{
public:
	CEposReportConsolCoversDaily();
	void Reset();

public:
	int Compare(CEposReportConsolCoversDaily& source, int nHint = 0);
	void Add(CEposReportConsolCoversDaily& source);

public:
	CString m_strBusinessDate;
	int m_nTaxDateCode;
	int m_nTaxBand;

public:
	int m_nCoversCount;
};

/**********************************************************************/

struct CEposReportConsolCoversDailyTax : public CEposReportConsolBase
{
public:
	CEposReportConsolCoversDailyTax();
	void Reset();

public:
	void Add(CEposReportConsolCoversDaily& source);

public:
	CString m_strBusinessDate;

public:
	int m_nCoversCount;
	double m_dTaxAmount;
};

/**********************************************************************/

struct CEposReportCoversDailyBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportCoversDailyBlock();

public:
	bool GotData();

public:
	int m_nCoversCount;
	CReportConsolidationArray<CEposReportConsolCoversDaily>* m_pSales;
	CEposReportConsolCoversDailyTax m_BlockTotal;
};

/**********************************************************************/
