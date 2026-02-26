#pragma once
/**********************************************************************/
#include "EposReportConsolBase.h"
#include "EposReportConsolCoversDataPoint.h"
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolCoversTimedGroup : CEposReportConsolBase
{
public:
	CEposReportConsolCoversTimedGroup();
	void Reset();

public:
	int Compare ( CEposReportConsolCoversTimedGroup& source, int nHint = 0 );
	void Add	( CEposReportConsolCoversTimedGroup& source );
	
public:
	CString m_strDateOrTime;
	int m_nDeptOrGroupNo;
	int m_nLineType;
	int m_nTaxDateCode;
	int m_nTaxBand;
	
public:
	int m_nCoversCount;
};

/**********************************************************************/

struct CEposReportConsolCoversTimedGroupTax : CEposReportConsolBase
{
public:
	CEposReportConsolCoversTimedGroupTax();
	void Reset();

public:
	void Add( CEposReportConsolCoversTimedGroup& source );

public:
	CString m_strDateOrTime;
	int m_nDeptOrGroupNo;
	int m_nLineType;
	
public:
	int m_nCoversCount;
	double m_dTaxAmount;
};

/**********************************************************************/

struct CEposReportCoversTimedGroupBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportCoversTimedGroupBlock() {
		m_nCoversCount = 0;
		m_pSales = NULL;
		m_pAdjustPremium = NULL;
		m_pAdjustDiscount = NULL;
	}

public:
	bool GotData();

public:
	int m_nCoversCount;
	CReportConsolidationArray<CEposReportConsolCoversTimedGroup>* m_pSales;
	CReportConsolidationArray<CEposReportConsolCoversAdjust>* m_pAdjustPremium;
	CReportConsolidationArray<CEposReportConsolCoversAdjust>* m_pAdjustDiscount;
	CEposReportConsolCoversTimedGroupTax m_BlockTotal;
};

/**********************************************************************/
