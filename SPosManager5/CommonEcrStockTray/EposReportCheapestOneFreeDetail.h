#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolCheapestOneFreeDetail
{
public:
	CEposReportConsolCheapestOneFreeDetail();

public:
	void Reset();
	int Compare ( CEposReportConsolCheapestOneFreeDetail& source, int nHint = 0 );
	void Add	( CEposReportConsolCheapestOneFreeDetail& source );

public:
	int m_nDbIdx;
	int m_nDbTotalFlag;
	int m_nMixMatchNo;
	int m_nSubtotalFlag;
	CString m_strDate;
	CString m_strTime;
	__int64 m_nLineNo;
	__int64 m_nPluNo;
	
public:
	int m_nQty;
	double m_dVal;
};

/**********************************************************************/

class CEposReportCheapestOneFreeDetail : public CEposReport
{
public:
	CEposReportCheapestOneFreeDetail( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	CReportConsolidationArray<CEposReportConsolCheapestOneFreeDetail> m_ConsolArray;
};

/**********************************************************************/

