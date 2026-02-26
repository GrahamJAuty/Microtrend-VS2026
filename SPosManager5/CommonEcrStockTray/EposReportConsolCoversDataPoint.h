#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolCoversDataPoint : CEposReportConsolBase
{
public:
	CEposReportConsolCoversDataPoint();
	void Reset();

public:
	int Compare ( CEposReportConsolCoversDataPoint& source, int nHint = 0 );
	void Add	( CEposReportConsolCoversDataPoint& source );
	
public:
	int m_nDataPoint;
	int m_nTaxDateCode;
	int m_nTaxBand;
	
public:
	int m_nCoversCount;
};

/**********************************************************************/

struct CEposReportConsolCoversDataPointTax : CEposReportConsolBase
{
public:
	CEposReportConsolCoversDataPointTax();
	void Reset();

public:
	void Add( CEposReportConsolCoversDataPoint& source );

public:
	int m_nDataPoint;
	
public:
	int m_nCoversCount;
	double m_dTaxAmount;
};

/**********************************************************************/

struct CEposReportConsolCoversAdjust
{
public:
	CEposReportConsolCoversAdjust();
	void Reset();

public:
	int Compare ( CEposReportConsolCoversAdjust& source, int nHint = 0 );
	void Add	( CEposReportConsolCoversAdjust& source );
	
public:
	int m_nTaxDateCode;
	int m_nTaxBand;
	
public:
	double m_dValue;
};

/**********************************************************************/

struct CEposReportCoversDataPointBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportCoversDataPointBlock();

public:
	bool GotData();

public:
	int m_nCoversCount;
	CReportConsolidationArray<CEposReportConsolCoversDataPoint>* m_pSales;
	CReportConsolidationArray<CEposReportConsolCoversAdjust>* m_pAdjustPremium;
	CReportConsolidationArray<CEposReportConsolCoversAdjust>* m_pAdjustDiscount;
	CEposReportConsolCoversDataPointTax m_BlockTotal;
};

/**********************************************************************/
