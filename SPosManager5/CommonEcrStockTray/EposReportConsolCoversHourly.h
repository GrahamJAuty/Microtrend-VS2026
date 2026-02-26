#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolCoversHourly : CEposReportConsolBase
{
public:
	CEposReportConsolCoversHourly();
	void Reset();

public:
	int Compare ( CEposReportConsolCoversHourly& source, int nHint = 0 );
	void Add	( CEposReportConsolCoversHourly& source );
	
public:
	int m_nDayShift;
	CString m_strTimeCode;
	int m_nTaxDateCode;
	int m_nTaxBand;
	
public:
	int m_nCoversCount;
};

/**********************************************************************/

struct CEposReportConsolCoversHourlyTax : CEposReportConsolBase
{
public:
	CEposReportConsolCoversHourlyTax();
	void Reset();

public:
	void Add(CEposReportConsolCoversHourly& source);

public:
	int m_nDayShift;
	CString m_strTimeCode;

public:
	int m_nCoversCount;
	double m_dTaxAmount;
};

/**********************************************************************/

struct CEposReportCoversHourlyBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportCoversHourlyBlock();

public:
	bool GotData();

public:
	int m_nCoversCount;
	CReportConsolidationArray<CEposReportConsolCoversHourly>* m_pSales;
	CEposReportConsolCoversHourlyTax m_BlockTotal;
};

/**********************************************************************/
